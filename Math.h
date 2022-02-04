#pragma once

#include "hlsl.h"

namespace Math
{
    static constexpr float Pi = 3.1415926535897932384626433832795028841971693993751058209749445923f;

    // projection of a onto b
    float ScalarProjection(const float3 a, const float3 b)
    {
        return dot(a, b) / length(b);
    }

    // projection of a onto b
    float3 VectorProjection(const float3 a, const float3 b)
    {
        return dot(a, b) / dot(b, b) * b;
    }
}

