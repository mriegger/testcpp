#pragma once

#include <array>
#include <cassert>
#include <algorithm>    // std::min
#include "half.h"

using half = FLOAT16;
using uint = uint32_t;
using dword = uint32_t;

inline float clamp(const float val, const float minVal, const float maxVal)
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
    float2(float val) :x(val), y(val) {}
    float2(float x, float y) :x(x), y(y) {}
    float x, y;

    float2 xy()const
    {
        return float2(x, y);
    }

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
    result.x = floorf(v.x);
    result.y = floorf(v.y);
    return result;
}

inline float2 clamp(float2 val, float2 minVal, float2 maxVal)
{
    val.x = std::clamp(val.x, minVal.x, maxVal.x);
    val.y = std::clamp(val.y, minVal.y, maxVal.y);
    return val;
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

inline float dot(float2 a, float2 b)
{
    return a.x * b.x + a.y * b.y;
}

template<typename A, typename B>
inline float dot(const A a, const B b)
{
    return (float)a.x * (float)b.x + (float)a.y * (float)b.y;
}

struct float3
{
    float x, y, z;
    float3() = default;
    constexpr float3(float a, float b, float c) :x(a), y(b), z(c)
    {
    }
    float3(float scalar)
    {
        x = y = z = scalar;
    }

    float SquaredLength() const
    {
        return x * x + y * y + z * z;
    }

    float2 xy() const
    {
        return float2(x, y);
    }

    float3 xyz() const
    {
        return float3(x, y, z);
    }

    // overloaded minus(-) operator
    float3 operator-() 
    {
        return float3(-x, -y, -z);
    }

    float& operator[](std::size_t idx)
    {
        assert(idx < 3);
        switch (idx)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        }
    }

    const float& operator[](std::size_t idx) const
    {
        assert(idx < 3);
        switch (idx)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        }
    }
};

inline float3 cross(const float3 a, const float3 b)
{
    float x = a.y * b.z - a.z * b.y;
    float y = a.z * b.x - a.x * b.z;
    float z = a.x * b.y - a.y * b.x;
    return float3(x, y, z);
}

inline float dot(float3 a, float3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float3 operator/(float f, float3 v)
{
    float3 res = float3(f, f, f);
    res.x /= v.x;
    res.y /= v.y;
    res.z /= v.z;
    return res;
}

inline float3 operator/(float3 v, float f)
{
    float3 res = v;
    res.x /= f;
    res.y /= f;
    res.z /= f;
    return res;
}

inline float length(const float3 v)
{
    return sqrtf(dot(v,v));
}

inline float3 normalize(const float3 v)
{
    const float l = length(v);
    float3 r = v;
    r.x /= l;
    r.y /= l;
    r.z /= l;
    return r;
}

struct float4
{
    float x, y, z, w;
    float4() = default;
    constexpr float4(float a, float b, float c, float d) :x(a), y(b), z(c), w(d)
    {
    }

    float4(float3 v, float f):x(v.x), y(v.y), z(v.z), w(f)
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

    float& operator[](std::size_t idx) 
    {
        assert(idx < 4);
        switch (idx)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        }
    }

    const float& operator[](std::size_t idx) const 
    {
        assert(idx < 4);
        switch (idx)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        case 3:
            return w;
        }
    }
};

inline float length(const float4 v)
{
    return sqrtf(dot(v, v));
}

inline float4 normalize(const float4 v)
{
    const float l = length(v);
    float4 r = v;
    r.x /= l;
    r.y /= l;
    r.z /= l;
    r.w /= l;
    return r;
}

inline float saturate(float v)
{
    if (v < 0) return 0;
    if (v > 1) return 1;
    return v;
}

inline float dot(const float4 a, const float4 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
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
    return 1.0f / sqrtf(f);
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

inline float4 operator-(const float4 a, const float4 b)
{
    float4 res = a;
    res.x -= b.x;
    res.y -= b.y;
    res.z -= b.z;
    res.w -= b.w;
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

inline float4 operator*(const float4 a, const float4 b)
{
    float4 res = a;
    res.x *= b.x;
    res.y *= b.y;
    res.z *= b.z;
    res.w *= b.w;
    return res;
}

inline float4 operator+(const float4 a, const float4 b)
{
    float4 res = a;
    res.x += b.x;
    res.y += b.y;
    res.z += b.z;
    res.w += b.w;
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

struct uint3
{
    uint x, y, z;
    uint3() = default;
    uint3(const uint x, const uint y, const uint z) :x(x), y(y), z(z)
    {

    }

    uint3(const float3 v) : x((uint)v.x), y((uint)v.y), z((uint)v.z)
    {

    }

    uint2 xy()const
    {
        return uint2(x, y);
    }
};

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

struct int2
{
    int x;
    int y;
};

inline int2 operator*(const int2 v, const int s)
{
    int2 res = v;
    res.x *= s;
    res.y *= s;
    return res;
}

struct int3
{
    int x;
    int y;
    int z;

    int3(const int2 v, const int s) :x(v.x), y(v.y), z(s)
    {

    }

    int3(const uint3 v) : x(v.x), y(v.y), z(v.z)
    {

    }
};

inline int3 operator+(const int3 a, const int3 b)
{
    int3 res = a;
    res.x += b.x;
    res.y += b.y;
    res.z += b.z;
    return res;
}

inline int3 operator+(const uint3 a, const int3 b)
{
    int3 res = a;
    res.x += b.x;
    res.y += b.y;
    res.z += b.z;
    return res;
}

struct float3x3
{
    std::array<float3, 3> m_rows;

    float3x3() = default;
    float3x3(const float a, const float b, const float c,
             const float d, const float e, const float f,
             const float g, const float h, const float i)
    {
        m_rows[0][0] = a;
        m_rows[0][1] = b;
        m_rows[0][2] = c;

        m_rows[1][0] = d;
        m_rows[1][1] = e;
        m_rows[1][2] = f;

        m_rows[2][0] = g;
        m_rows[2][1] = h;
        m_rows[2][2] = i;
    }

    void SetRow(int row, float a, float b, float c)
    {
        assert(row >= 0 && row < 3);
        m_rows[row] = float3(a, b, c);
    }

    float3& operator[](std::size_t idx)
    {
        return m_rows[idx];
    }

    const float3& operator[](std::size_t idx) const
    {
        return m_rows[idx];
    }
};

struct float4x4
{
    std::array<float4, 4> m_rows;

    float4x4() = default;
    float4x4(const float a, const float b, const float c, const float d, 
             const float e, const float f, const float g, const float h, 
             const float i, const float j, const float k, const float l,
            const float m, const float n, const float o, const float p)
    {
        m_rows[0][0] = a;
        m_rows[0][1] = b;
        m_rows[0][2] = c;
        m_rows[0][3] = d;

        m_rows[1][0] = e;
        m_rows[1][1] = f;
        m_rows[1][2] = g;
        m_rows[1][3] = h;

        m_rows[2][0] = i;
        m_rows[2][1] = j;
        m_rows[2][2] = k;
        m_rows[2][3] = l;

        m_rows[3][0] = m;
        m_rows[3][1] = n;
        m_rows[3][2] = o;
        m_rows[3][3] = p;
    }

    float4& operator[](std::size_t idx)
    {
        return m_rows[idx];
    }

    const float4& operator[](std::size_t idx) const
    {
        return m_rows[idx];
    }

    void SetRow(int row, float a, float b, float c, float d)
    {
        assert(row >= 0 && row < 4);
        m_rows[row] = float4(a,b,c,d);
    }
};

inline float3 mul(const float3x3 mat, const float3 v)
{
    float3 res;
    res.x = dot(mat.m_rows[0], v);
    res.y = dot(mat.m_rows[1], v);
    res.z = dot(mat.m_rows[2], v);
    return res;
}

inline float3x3 mul(const float3x3 l, const float3x3 r)
{
    float3x3 res;

    const float3 rcol0(r[0][0], r[1][0], r[2][0]);
    const float3 rcol1(r[0][1], r[1][1], r[2][1]);
    const float3 rcol2(r[0][2], r[1][2], r[2][2]);

    res.m_rows[0][0] = dot(l.m_rows[0], rcol0);
    res.m_rows[0][1] = dot(l.m_rows[0], rcol1);
    res.m_rows[0][2] = dot(l.m_rows[0], rcol2);

    res.m_rows[1][0] = dot(l.m_rows[1], rcol0);
    res.m_rows[1][1] = dot(l.m_rows[1], rcol1);
    res.m_rows[1][2] = dot(l.m_rows[1], rcol2);

    res.m_rows[2][0] = dot(l.m_rows[2], rcol0);
    res.m_rows[2][1] = dot(l.m_rows[2], rcol1);
    res.m_rows[2][2] = dot(l.m_rows[2], rcol2);

    return res;
}

inline float4x4 mul(const float4x4 l, const float4x4 r)
{
    float4x4 res;

    const float4 rcol0(r[0][0], r[1][0], r[2][0], r[3][0]);
    const float4 rcol1(r[0][1], r[1][1], r[2][1], r[3][1]);
    const float4 rcol2(r[0][2], r[1][2], r[2][2], r[3][2]);
    const float4 rcol3(r[0][3], r[1][3], r[2][3], r[3][3]);

    res.m_rows[0][0] = dot(l.m_rows[0], rcol0);
    res.m_rows[0][1] = dot(l.m_rows[0], rcol1);
    res.m_rows[0][2] = dot(l.m_rows[0], rcol2);
    res.m_rows[0][3] = dot(l.m_rows[0], rcol3);

    res.m_rows[1][0] = dot(l.m_rows[1], rcol0);
    res.m_rows[1][1] = dot(l.m_rows[1], rcol1);
    res.m_rows[1][2] = dot(l.m_rows[1], rcol2);
    res.m_rows[1][3] = dot(l.m_rows[1], rcol3);

    res.m_rows[2][0] = dot(l.m_rows[2], rcol0);
    res.m_rows[2][1] = dot(l.m_rows[2], rcol1);
    res.m_rows[2][2] = dot(l.m_rows[2], rcol2);
    res.m_rows[2][3] = dot(l.m_rows[2], rcol3);

    res.m_rows[3][0] = dot(l.m_rows[3], rcol0);
    res.m_rows[3][1] = dot(l.m_rows[3], rcol1);
    res.m_rows[3][2] = dot(l.m_rows[3], rcol2);
    res.m_rows[3][3] = dot(l.m_rows[3], rcol3);


    return res;
}


inline float3 mul(const float3 v, const float3x3 mat)
{
    float3 res;
    res.x = v.x * mat[0][0] + v.y * mat[1][0] + v.z * mat[2][0];
    res.y = v.x * mat[0][1] + v.y * mat[1][1] + v.z * mat[2][1];
    res.z = v.x * mat[0][2] + v.y * mat[1][2] + v.z * mat[2][2];
    return res;
}

inline float4 mul(const float4x4 mat, const float4 v)
{
    float4 res;
    res.x = dot(mat.m_rows[0], v);
    res.y = dot(mat.m_rows[1], v);
    res.z = dot(mat.m_rows[2], v);
    res.w = dot(mat.m_rows[3], v);
    return res;
}

template<typename T>
inline T mad(const T a, const T b, const T c)
{
    return a * b + c;
}

template<>
inline float4 mad(const float4 a, const float4 b, const float4 c)
{
    return a * b + c;
}

inline float4 operator/(const float4 v, const float f)
{
    float4 res = v;
    res.x /= f;
    res.y /= f;
    res.z /= f;
    res.w /= f;
    return res;
}

inline void sincos(const float f, float& s, float& c)
{
    s = sinf(f);
    c = cosf(f);
}

inline constexpr float degrees(const float radians)
{
    constexpr float Pi = 3.14159265358979323846f;
    return radians / Pi * 180.0f;
}

inline constexpr float radians(const float degrees)
{
    constexpr float Pi = 3.14159265358979323846f;
    return degrees * Pi / 180.0f;
}

template<typename T>
T sign(const T value)
{
    if (value == 0)
        return 0;
    else if (value > 0)
        return 1;
    else
        return -1;
}

inline float distance(const float3 a, const float3 b)
{
    const auto diff = a - b;
    return length(diff);
}
// Returns a smooth Hermite interpolation between 0 and 1, if x is in the range [edge0, edge1].
inline float smoothstep(const float edge0, const float edge1, const float x)
{
    // Scale, bias and saturate x to 0..1 range
    const float res = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    // Evaluate polynomial
    return res * res * (3 - 2 * res);
}

inline uint countbits(const uint value)
{
    uint count = 0;
    uint iter = value;
    while (iter)
    {
        count++;
        iter = iter & (iter - 1);
    }
    return count;
}

inline float rcp(const float value)
{
    return 1.0f / value;
}

inline float2 rcp(const float2 v)
{
    return float2(1.0f / v.x, 1.0f / v.y);
}

inline uint reversebits(uint n)
{
    unsigned int rev = 0;
    for(uint i = 0 ; i < 32 ; ++i)
    {
        rev <<= 1;

        if ((n & 1) == 1)
            rev ^= 1;

        n >>= 1;
    }
    return rev;
}

inline float2 abs(const float2 v)
{
    float2 res;
    res.x = abs(v.x);
    res.y = abs(v.y);
    return res;
}

// Blocks execution of all threads in a group until all memory accesses have been completed.
inline void AllMemoryBarrier() { }

// Blocks execution of all threads in a group until all memory accesses have been completed 
// and all threads in the group have reached this call.
inline void AllMemoryBarrierWithGroupSync() { }

// Blocks execution of all threads in a group until all device memory accesses have been completed.
inline void DeviceMemoryBarrier() { }

// Blocks execution of all threads in a group until all device memory accesses have been completed
// and all threads in the group have reached this call.
inline void DeviceMemoryBarrierWithGroupSync() { }

// Blocks execution of all threads in a group until all group shared accesses have been completed.
inline void GroupMemoryBarrier() { }

// Blocks execution of all threads in a group until all group shared accesses have been completed 
// and all threads in the group have reached this call.
inline void GroupMemoryBarrierWithGroupSync() { }

inline float3x3 transpose(const float3x3& m)
{
    float3x3 res{};
    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            res.m_rows[col][row] = m.m_rows[row][col];
        }
    }
    return res;
}

inline float4x4 transpose(const float4x4& m)
{
    float4x4 res{};
    for (int row = 0; row < 4; ++row)
    {
        for (int col = 0; col < 4; ++col)
        {
            res.m_rows[col][row] = m.m_rows[row][col];
        }
    }
    return res;
}

inline float3 exp(const float3 v)
{
    float3 res;
    res.x = expf(v.x);
    res.y = expf(v.y);
    res.z = expf(v.z);
    return res;
}
