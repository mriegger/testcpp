#pragma once

#include <algorithm>    // std::min

using uint = uint32_t;

template<typename T>
T clamp(T val, T minVal, T maxVal)
{
    if (val <= minVal)
    {
        return minVal;
    }
    else if (val >= maxVal)
    {
        return maxVal;
    }
    return val;
}

inline int firstbitlow(uint32_t x)
{
    int res = 0;
    while (x)
    {
        if ((x & 1) == 1)
            return res;

        x = x >> 1;
        res++;
    }
    return -1;
}

inline float step(float y, float x)
{
    return (x >= y) ? 1.0f : 0.0f;
}

struct float2
{
    float2() = default;
    float2(float x, float y) :x(x), y(y) {}
    float x, y;

    float2& operator*=(float2 rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }
};

inline float2 floor(const float2 v)
{
    float2 result;
    result.x = floor(v.x);
    result.y = floor(v.y);
    return result;
}

inline float2 min(float2 a, float2 b)
{
    using namespace std;

    float2 res;
    res.x = min(a.x, b.x);
    res.y = min(a.y, b.y);
    return res;
}

inline float2 max(float2 a, float2 b)
{
    using namespace std;

    float2 res;
    res.x = max(a.x, b.x);
    res.y = max(a.y, b.y);
    return res;
}

struct float3
{
    float x, y, z;
    float3() = default;
    float3(float a, float b, float c) :x(a), y(b), z(c)
    {
    }
    float3(float scalar)
    {
        x = y = z = scalar;
    }

    float SquaredLength()const
    {
        return x * x + y * y + z * z;
    }
};

inline float3 operator/(float f, float3 v)
{
    float3 res = float3(f, f, f);
    res.x /= v.x;
    res.y /= v.y;
    res.z /= v.z;
    return res;
}

struct float4
{
    float x, y, z, w;
    float4() = default;
    float4(float a, float b, float c, float d) :x(a), y(b), z(c), w(d)
    {
    }
    float4(float scalar)
    {
        x = y = z = w = scalar;
    }

    float2 xy()const
    {
        return float2(x, y);
    }

    float2 zw()const
    {
        return float2(z, w);
    }

    float3 xyz()const
    {
        return float3(x, y, z);
    }

    void SetXY(float2 v)
    {
        x = v.x;
        y = v.y;
    }

    void SetYZ(float2 v)
    {
        y = v.x;
        z = v.y;
    }

    void SetZW(float2 v)
    {
        z = v.x;
        w = v.y;
    }
};

inline float saturate(float v)
{
    if (v < 0) return 0;
    if (v > 1) return 1;
    return v;
}

inline float2 saturate(float2 v)
{
    float2 res = v;
    res.x = saturate(res.x);
    res.y = saturate(res.y);
    return res;
}

inline float4 saturate(float4 v)
{
    float4 res = v;
    res.x = saturate(res.x);
    res.y = saturate(res.y);
    res.z = saturate(res.z);
    res.w = saturate(res.w);
    return res;
}

inline float rsqrt(float f)
{
    return 1.0f / sqrt(f);
}

inline float dot(float3 a, float3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float2 operator-(float2 a, float2 b)
{
    float2 res = a;
    res.x -= b.x;
    res.y -= b.y;
    return res;
}

inline float3 operator-(float3 a, float3 b)
{
    float3 res = a;
    res.x -= b.x;
    res.y -= b.y;
    res.z -= b.z;
    return res;
}

inline float2 operator*(float2 v, float s)
{
    float2 res = v;
    res.x *= s;
    res.y *= s;
    return res;
}

inline float2 operator*(float2 a, float2 b)
{
    float2 res = a;
    res.x *= b.x;
    res.y *= b.y;
    return res;
}


inline float2 operator/(float2 a, float2 b)
{
    float2 res = a;
    res.x /= b.x;
    res.y /= b.y;
    return res;
}

inline float3 operator*(float3 v, float s)
{
    float3 res = v;
    res.x *= s;
    res.y *= s;
    res.z *= s;
    return res;
}

inline float3 operator*(float s, float3 v)
{
    float3 res = v;
    res.x *= s;
    res.y *= s;
    res.z *= s;
    return res;
}

inline float2 operator+(float2 a, float2 b)
{
    float2 res = a;
    res.x += b.x;
    res.y += b.y;
    return res;
}

inline float3 operator+(float3 a, float3 b)
{
    float3 res = a;
    res.x += b.x;
    res.y += b.y;
    res.z += b.z;
    return res;
}

inline float4 operator+(float4 v, float f)
{
    float4 res = v;
    res.x += f;
    res.y += f;
    res.z += f;
    res.w += f;
    return res;
}

inline float4 operator/(float f, float4 v)
{
    float4 res = float4(f, f, f, f);
    res.x /= v.x;
    res.y /= v.y;
    res.z /= v.z;
    res.w /= v.w;
    return res;
}

inline float4 operator*(float4 v, float f)
{
    float4 res = v;
    res.x *= f;
    res.y *= f;
    res.z *= f;
    res.w *= f;
    return res;
}

inline float4 step(float4 a, float4 b)
{
    float4 res;
    res.x = step(a.x, b.x);
    res.y = step(a.y, b.y);
    res.z = step(a.z, b.z);
    res.w = step(a.w, b.w);
    return res;
}

inline float4 max(float4 a, float4 b)
{
    using namespace std;
    float4 res;
    res.x = max(a.x, b.x);
    res.y = max(a.y, b.y);
    res.z = max(a.z, b.z);
    res.w = max(a.w, b.w);
    return res;
}

inline float4 step(float4 a, float f)
{
    float4 b = float4(f, f, f, f);
    return step(a, b);
}

struct uint2
{
    uint x, y;
    uint2() = default;
    uint2(uint x, uint y) :x(x), y(y)
    {

    }
};

inline uint2 operator*(uint2 a, uint2 b)
{
    uint2 res = a;
    res.x *= b.x;
    res.y *= b.y;
    return res;
}

inline uint2 operator|=(uint2 a, uint2 b)
{
    uint2 res = a;
    res.x |= b.x;
    res.y |= b.y;
    return res;
}

struct uint4
{
    uint x, y, z, w;

    uint4() = default;
    uint4(uint val)
    {
        x = y = z = w = val;
    }
    uint4(uint x, uint y, uint z, uint w) :x(x), y(y), z(z), w(w)
    {

    }
    uint4(float x, float y, float z, float w) 
    {
        this->x = static_cast<uint>(x);
        this->y = static_cast<uint>(y);
        this->z = static_cast<uint>(z);
        this->w = static_cast<uint>(w);
    }
    uint4(int x, int y, int z, int w) :x(x), y(y), z(z), w(w)
    {

    }
    uint4(float4 v)
    {
        this->x = static_cast<uint>(v.x);
        this->y = static_cast<uint>(v.y);
        this->z = static_cast<uint>(v.z);
        this->w = static_cast<uint>(v.w);
    }

    uint2 xy()const
    {
        return uint2(x, y);
    }

    uint2 zw()const
    {
        return uint2(z, w);
    }
};

inline uint4 operator&(uint s, uint4 rhs)
{
    uint4 res = uint4(s, s, s, s);
    res.x &= rhs.x;
    res.y &= rhs.y;
    res.z &= rhs.z;
    res.w &= rhs.w;

    return res;
}

inline uint4 operator<<(uint4 a, uint4 b)
{
    uint4 res = a;
    res.x <<= b.x;
    res.y <<= b.y;
    res.z <<= b.z;
    res.w <<= b.w;
    return res;
}

inline uint2 operator<<(uint2 a, uint2 b)
{
    uint2 res = a;
    res.x <<= b.x;
    res.y <<= b.y;
    return res;
}

struct bool4
{
    bool x, y, z, w;
};

inline bool4 operator<(bool4 a, bool4 b)
{
    bool4 res;
    res.x = a.x < b.x;
    res.y = a.y < b.y;
    res.z = a.z < b.z;
    res.w = a.w < b.w;
    return res;
}

inline bool4 operator!(bool4 b)
{
    bool4 res;
    res.x = !b.x;
    res.y = !b.y;
    res.z = !b.z;
    res.w = !b.w;

    return res;
}

inline bool4 operator<(float4 a, float4 b)
{
    bool4 res;
    res.x = a.x < b.x;
    res.y = a.y < b.y;
    res.z = a.z < b.z;
    res.w = a.w < b.w;
    return res;
}

inline bool4 operator>(float4 a, float4 b)
{
    bool4 res;
    res.x = a.x > b.x;
    res.y = a.y > b.y;
    res.z = a.z > b.z;
    res.w = a.w > b.w;
    return res;
}

inline bool4 operator<(float4 a, float f)
{
    float4 b = float4(f, f, f, f);
    return a < b;
}

inline bool4 operator>(float4 a, float f)
{
    float4 b = float4(f, f, f, f);
    return a > b;
}

inline float asfloat(uint u)
{
    union Convert
    {
        uint u;
        float f;
    }convert;

    convert.u = u;
    return convert.f;
}

inline uint asuint(float f)
{
    union Convert
    {
        uint u;
        float f;
    }convert;
    convert.f = f;
    return convert.u;
}