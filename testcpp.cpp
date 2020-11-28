// testcpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

import MyModuleName;
import ProducerConsumer;


#define D_REVERSED                                      1
#define PERMUTATION_DEPTH D_REVERSED
#include <iostream>
#include <ranges>
#include <concepts>
#include <cassert>
#include <map>
#include <mutex>
#include <set>
#include <type_traits>
#include <thread>
#include <vector>
#include <functional>
#include <array>
#include <semaphore>
#include <unordered_set>
#include <atomic>
#include "hlsl.h"
#include "TextureSampling.h"
#include "DiningPhilosophers.h"
#include "AsyncLinkedListTester.h"
#include "AtomicInstructions.h"

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
    S2(int a, int b)
        : m_a(a)
        , m_b(b)
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
#define Tile_UnitValueToBit(x)          (saturate(x) * asfloat(asuint(32.0) - 1))


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
} cb_perFrame;

struct sTileData
{
    float   zNear;
    float   zFar_scattering;
    float   zFar;
    uint    mask;
    uint    logMaxBins;
};

#if (PERMUTATION_DEPTH == D_REVERSED)

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

struct DefaultIbl
{
    int* m_fp = nullptr;
    DefaultIbl()
    {
        cout << "ctor" << endl;
    }
    DefaultIbl(const DefaultIbl& rhs)
    {
        cout << "copy ctor" << endl;
    };
    DefaultIbl& operator=(const DefaultIbl& rhs)
    {
        cout << "copy assignment" << endl;
        m_fp = rhs.m_fp;
        return *this;
    }
    ~DefaultIbl()
    {
        cout << "~dtor" << endl;
        if (m_fp)
        {
            std::cout << "releasing" << std::endl;
            m_fp = nullptr;
        }
    }
};


#define THREADS_Y 256

int mad(int a, int b, int c)
{
    return a * b + c;
}



#define THREADS_X 2     // Must be at least  2 or shader will produce errors
#define THREADS_Y 64    // Must be at least 16 or shader will produce errors

#define LDS_WIDTH (THREADS_X)

int GetLdsIndex(uint2 index)
{
    return mad(index.y, LDS_WIDTH, index.x);
}
#define LDS_SIZE 512

class Flatland
{
public:
    auto operator <=> (const Flatland&) const = default;
    int x;
    int y;
private:
    int z;
};

struct Point2D
{
    auto operator <=> (const Point2D&) const = default;
    float x, y;
};

struct RayRayIntersectOutput
{
    float t0;
    float t1;
    bool intersects = false;
};

RayRayIntersectOutput RayRayIntersect(float3 o0, float3 o1, float3 r0, float3 r1)
{
    RayRayIntersectOutput o;

    r0 = normalize(r0);
    r1 = normalize(r1);

    float r0_dot_r1 = dot(r0, r1);
    float3 a = o0 - o1;
    float a_dot_r0 = dot(a, r0);
    float a_dot_r1 = dot(a, r1);

    float denom0 = (a_dot_r1 * r0_dot_r1 - a_dot_r0);

    if (fabs(denom0) > 0.001)
    {
        float t0 = (1 - r0_dot_r1 * r0_dot_r1) / denom0;
        o.t0 = t0;
    }
    o.intersects = o.t0 >= 0;
    return o;
}

class Solution
{
    int m_result = 0;
    bool m_roundsPerfectly = false;

    bool divideByDoubling(unsigned int dividend, unsigned divisor)
    {
        if (dividend == 0)
        {
            m_roundsPerfectly = true;
            return true;
        }
        if (divisor > dividend)
        {
            return false;
        }
        unsigned int count = 1;
        auto currSubtractValue = divisor;
        while (currSubtractValue < (UINT_MAX >> 1) && currSubtractValue + currSubtractValue <= dividend)
        {
            currSubtractValue += currSubtractValue;
            count += count;
        }
        m_result += count;
        dividend -= currSubtractValue;
        return divideByDoubling(dividend, divisor);
    }

    int sign(int x)
    {
        if (x == 0)
        {
            return 0;
        }
        return x < 0 ? -1 : 1;
    }

public:
    int divide(int dividend, int divisor)
    {
        int numLessZero = dividend < 0 ? 1 : 0;
        numLessZero += divisor < 0 ? 1 : 0;

        if (dividend == INT_MIN && abs(divisor) == 1)
        {
            if (numLessZero == 1)
            {
                return INT_MIN;
            }
            else
            {
                return INT_MAX;
            }
        }


        unsigned int a = dividend != INT_MIN ? abs(dividend) : -INT_MAX;
        unsigned int b = divisor != INT_MIN ? abs(divisor) : -INT_MAX;


        divideByDoubling(a, b);
        auto res = numLessZero == 1 ? -m_result : m_result;
        if (m_roundsPerfectly == false && numLessZero == 1)
        {
            res--;
        }

        return res;
    }
};

vector<mutex> forks;
atomic<uint64_t> numPlatesEaten;

void philo(int id)
{
    chrono::milliseconds dur(20);
    while (true)
    {
        {
            int leftId = id;
            int rightId = (id + 1) % 5;


            if (id == 4)
            {
                swap(leftId, rightId);
            }

            forks[leftId].lock();
            this_thread::sleep_for(dur); //Leads to deadlock immediately


            forks[rightId].lock();
            numPlatesEaten++;
            cout << "philo" << id << "is eating" << endl;


            chrono::milliseconds dur2(rand() % 200 + 100);
            this_thread::sleep_for(dur2);


            forks[rightId].unlock();
            forks[leftId].unlock();
        }
    }
}



int main()
{
    AsyncLinkedListTester t;
    t.Run();



    return 0;
};

