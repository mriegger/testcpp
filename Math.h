#pragma once

#include "hlsl.h"

namespace Math
{
    static constexpr float Pi = 3.1415926535897932384626433832795028841971693993751058209749445923f;

    // projection of a onto b
    inline float ScalarProjection(const float3 a, const float3 b)
    {
        return dot(a, b) / length(b);
    }

    // projection of a onto b
    inline float3 VectorProjection(const float3 a, const float3 b)
    {
        return dot(a, b) / dot(b, b) * b;
    }

    inline bool ApproxEqual(const float a, const float b, const float ep = 0.0001f)
    {
        return abs(a - b) < ep;
    }

    inline bool ApproxEqual(const float3 a, const float3 b, const float ep = 0.0001f)
    {
        return abs(a.x - b.x) < ep && abs(a.y - b.y) < ep && abs(a.z - b.z) < ep;
    }
}

