#pragma once

#include "hlsl.h"
#include <vector>

class TextureSampling
{
public:

    explicit TextureSampling(const float2 textureDimensions);
    float BilinearSampleReference(const float2 uv);

private:

    std::vector<std::vector<float>> m_texture;
    float2 m_textureDimensions;
    void FillTexture();
    void FillRow(std::vector<float>& row, const float startValue);
};