// testcpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define D_REVERSED                                      1
#define PERMUTATION_DEPTH D_REVERSED

#include <iostream>
#include <cassert>
#include <map>
#include <type_traits>
#include <vector>
#include <array>
#include <unordered_set>
#include "hlsl.h"
#include "TextureSampling.h"

using namespace std;

int NUM_BINS_PER_TILE = 32;

#define DEG_TO_RAD 0.017453292519943295769236907684886f

std::vector<uint32_t> tile_opaque;

void MarkLightAsVisibleInSharedMemory(uint lightIndex, uint base)
{
    const uint bucket_index = lightIndex / 32;
    const uint bucket_place = lightIndex % 32;
//    InterlockedOr(tile_opaque[bucket_index], 1 << bucket_place);
    tile_opaque[bucket_index + base] |= 1 << bucket_place;
}

struct S
{
    unordered_set<string> us;

    unordered_set<string> GetSet()
    {
        return us;
    }

    unordered_set<string>& GetConstRef()
    {
        return us;
    }
};

struct S2
{
    int m_a, m_b;
    S2(int a, int b):m_a(a), m_b(b)
    {

    }
};

bool TestSphereVsCone(float3 sphereCenter, float sphereRadius, float3 conePos, float3 coneDir, float cosa0, float R, float Rmin)
{
    // IMPORTANT:
    // - move cone origin taking Rmin into account
    // - for area lights we can't trace sphere with radius = "R", we have to use worst case instead (i.e
    //   radius = "R + Rmin"), because for area lights light range can be reached from any point on the light disk

    float3 Xl = sphereCenter - conePos;

    float invSina0 = rsqrt(saturate(1.0f - cosa0 * cosa0));
    float shift = invSina0 * (Rmin * cosa0 + sphereRadius);
    float3 Xl_cone = Xl + coneDir * shift;
    float invLenSq = rsqrt(dot(Xl_cone, Xl_cone));
    bool isInsideCone = dot(Xl_cone, coneDir) * invLenSq > cosa0;

    float delta = R + Rmin + sphereRadius;
    bool isInsideSphere = dot(Xl, Xl) < delta * delta;

    return isInsideCone && isInsideSphere;
}

bool IntrSphere3Cone3(float3 sphereCenter, float sphereRadius, float coneSineAngle, float coneCosAngle, float3 conePos, float3 coneDir)
{
    float fInvSin = ((float)1.0) / coneSineAngle;
    float fCosSqr = coneCosAngle * coneCosAngle;

    float3 kCmV = sphereCenter - conePos;
    float3 kD = kCmV + (sphereRadius * fInvSin) * coneDir;
    float fDSqrLen = kD.SquaredLength();
    float fE = dot(kD, coneDir);////kD.Dot(coneDir);
    if (fE > (float)0.0 && fE * fE >= fDSqrLen * fCosSqr)
    {
        float fSinSqr = coneSineAngle * coneSineAngle;
        fDSqrLen = kCmV.SquaredLength();
        fE = -dot(kCmV, coneDir);///-kCmV.Dot(coneDir);
        if (fE > (float)0.0 && fE * fE >= fDSqrLen * fSinSqr)
        {
            float fRSqr = sphereRadius * sphereRadius;
            return fDSqrLen <= fRSqr;
        }
        return true;
    }
    return false;
}

// Bart Wronski version
bool TestConeVsSphere(float3 coneOrigin, float3 coneDir, float size, float angleRad, float3 spherePos, float sphereRad)
{
    const float3 V = spherePos - coneOrigin;
    const float  VlenSq = dot(V, V);
    const float  V1len = dot(V, coneDir);
    const float  distanceClosestPoint = cos(angleRad) * sqrt(VlenSq - V1len * V1len) - V1len * sin(angleRad);

    const bool angleCull = distanceClosestPoint > sphereRad;
    const bool frontCull = V1len > sphereRad + size;
    const bool backCull = V1len < -sphereRad;
    return !(angleCull || frontCull || backCull);
}


void TestMiss0()
{
    float3 spherePos = float3(-1.000001f, 0, 0);
    float sphereRad = 1.0f;
    float3 conePos = float3(0, 0, 0);
    float3 coneDir = float3(1, 0, 0);
    float coneSize = 10.0f;
    float coneAngle = 0.78f; // 45 degrees

    bool hit = TestConeVsSphere(conePos, coneDir, coneSize, coneAngle, spherePos, sphereRad);
    assert(!hit);
}

void TestMiss1()
{
    float3 spherePos = float3(5, 6, 0);
    float sphereRad = 0.999f;

    float3 conePos = float3(0, 0, 0);
    float3 coneDir = float3(1, 0, 0);
    float coneSize = 2.5f;
    float coneAngle = DEG_TO_RAD * 45.0f;

    bool hit = TestConeVsSphere(conePos, coneDir, coneSize, coneAngle, spherePos, sphereRad);
    assert(!hit);
}


void TestHit1()
{
    float3 spherePos = float3(6.5, 0, 0);
    float sphereRad = 1;

    float3 conePos = float3(0, 0, 0);
    float3 coneDir = float3(1, 0, 0);
    float coneSize = 5.0f;
    float Rmin = 0.0f;

    float spotAngle = 90.0f;
    float coneAngle = DEG_TO_RAD * spotAngle  * 0.5f;
    float cosa = cos(coneAngle);

    bool bhit = TestConeVsSphere(conePos, coneDir, coneSize, coneAngle, spherePos, sphereRad);
    bool zhit = TestSphereVsCone(spherePos, sphereRad, conePos, coneDir, cosa, coneSize, Rmin);
    bool eHit = IntrSphere3Cone3(spherePos, sphereRad, sin(coneAngle), cos(coneAngle), conePos, coneDir);

    assert(bhit);
}

#define Depth_to_Z(d, vUnprojectZ)                                      (vUnprojectZ.x / (d + vUnprojectZ.y))

// IMPORTANT: we want to avoid getting exact value "32", so decrease by 1 ULP
#define Tile_UnitValueToBit(x)          (saturate(x) * asfloat( asuint(32.0) - 1 ) )


struct sPerFrame
{
    float4          vFrustum;

    float2          vUnprojectZ;
    float2          vUnprojectZ_internal;

    float2          vGridPixel;
    float2          vGridHalfPixel;

    float2          vInvGbSize;
    float           fNormBulbRadius;
    float           fFarDepth;

    uint            uiGroupsMinusOne;
    uint            uiMaxLightsPerBin;
    uint            uiLastSlice;
    float           fHalfTileDimMulPixelUnproject;

    float           fLogMaxBins;
    float           fRangeThreshold;
    uint            uiGridW;
    uint            _unused;
}cb_perFrame;

struct sTileData
{
    float   zNear;
    float   zFar_scattering;
    float   zFar;
    uint    mask;
    uint    logMaxBins;
};

#if( PERMUTATION_DEPTH == D_REVERSED )

#define DEPTH_Min                               max
#define DEPTH_Max                               min
#define DEPTH_InterlockedMin                    InterlockedMax
#define DEPTH_InterlockedMax                    InterlockedMin
#define DEPTH_NEAR                              1.0
#define DEPTH_FAR                               0.0

#define DepthNoInf(d)                           max(d, step(d, cb_perFrame.fFarDepth))
#define DepthLessThanInf(d)                     (d > cb_perFrame.fFarDepth)

#else

#define DEPTH_Min                               min
#define DEPTH_Max                               max
#define DEPTH_InterlockedMin                    InterlockedMin
#define DEPTH_InterlockedMax                    InterlockedMax
#define DEPTH_NEAR                              0.0
#define DEPTH_FAR                               1.0

#define DepthNoInf(d)                           (d - d * step(cb_perFrame.fFarDepth, d))
#define DepthLessThanInf(d)                     (d < cb_perFrame.fFarDepth)

#endif

float3 Regs_FindMinMaxMaxnoinf(float4 d)
{
    float4 t;

    t.SetXY(DEPTH_Min(d.xy(), d.zw()));
    t.x = DEPTH_Min(t.x, t.y);

    t.SetYZ(DEPTH_Max(d.xy(), d.zw()));
    t.y = DEPTH_Max(t.y, t.z);

    // IMPORTANT: force DEPTH_NEAR for values "logically >=" lights far plane. It means that if the entire
    // tile has only infinite values it will be placed on near plane to get free early-z rejection for all lights!
    d = DepthNoInf(d);

    t.SetZW(DEPTH_Max(d.xy(), d.zw()));
    t.z = DEPTH_Max(t.z, t.w);

    return t.xyz();
}


uint Regs_ConvertToMask(float4 d, float a, float b)
{
    float4 z = Depth_to_Z(d, cb_perFrame.vUnprojectZ);

    float4 bit = Tile_UnitValueToBit(z * a + b);
    uint4 mask = uint4(1, 1, 1, 1) << uint4(bit);
    bool4 cmp = DepthLessThanInf(d);

    mask.x = cmp.x ? mask.x : 0;
    mask.y = cmp.y ? mask.y : 0;
    mask.z = cmp.z ? mask.z : 0;
    mask.w = cmp.w ? mask.w : 0;

    mask.xy() |= mask.zw();
    mask.x |= mask.y;

    return mask.x;
}



void Reduction()
{
    cb_perFrame.fFarDepth = 0;// 0.00389951840f;
    cb_perFrame.fHalfTileDimMulPixelUnproject = 0.00962120388f;
    cb_perFrame.fLogMaxBins = 3; // 2^3 = 8 bins
    cb_perFrame.fNormBulbRadius = 0.0626223087f;
    cb_perFrame.fRangeThreshold = 1.05467284f;
    cb_perFrame.uiGridW = 101;
    cb_perFrame.uiGroupsMinusOne = 3;
    cb_perFrame.uiLastSlice = 255;
    cb_perFrame.uiMaxLightsPerBin = 256;
    cb_perFrame.vFrustum = float4(1.94348323f, -1.09260499f, -0.971741617f, 0.546302497f);
    cb_perFrame.vGridHalfPixel = float2(0.00497512426f, 0.00884955749f);
    cb_perFrame.vGridPixel = float2(0.00995024852f, 0.0176991150f);
    cb_perFrame.vInvGbSize = float2(0.000621890533f, 0.00110619469f);
    cb_perFrame.vUnprojectZ = float2(0.125000000f, -0.000000000f);
    cb_perFrame.vUnprojectZ_internal = float2(0.125000000f, -0.000000000f);

    float4 do1 = float4(0.00001f, 0.2f, 0.3f, 0.9f);

    float3 minmaxd = Regs_FindMinMaxMaxnoinf(do1);
    float3 minmaxz = Depth_to_Z(minmaxd, cb_perFrame.vUnprojectZ);



    float tileZ = minmaxz.z - minmaxz.x;
    float A = 1.0 / tileZ;
    float B = -minmaxz.x * A;

    uint maskOpaque = Regs_ConvertToMask(do1, A, B);

    //===============================================================================================
    // NOTE: Mask

 //   Shared_MergeMasks(S_MASK_OPAQUE, maskOpaque);

    //===============================================================================================
    // NOTE: Output

    if (true/*gl_LocalInvocationIndex == 0*/)
    {
    //    maskOpaque = shared_mem[S_MASK_OPAQUE];


        // NOTE: calculate number of bins

        float ratio = tileZ / cb_perFrame.fRangeThreshold;
        float logMaxBins = clamp(log2(ratio), 0.0f, cb_perFrame.fLogMaxBins);

        // FIXME: verify - minmaxz.x > minmaxz.w ? "sky" : "ground"?

        sTileData data;
        data.zNear = minmaxz.x;
        data.zFar_scattering = minmaxz.y;
        data.zFar = minmaxz.z;
        data.mask = maskOpaque;
        data.logMaxBins = (uint)(logMaxBins + 0.5);

        int x = 5;
        (void)x;
    //    uint4 pack = Tile_PackData(data);

  //      uav_TileData[int2(gl_WorkGroupID.xy)] = pack;
    }
}


int main()
{
    TextureSampling ts(float2(2,2));
    float res = ts.BilinearSampleReference(float2(0.5, 0.5));
    assert(res == 2.5);

    return 0;
}
