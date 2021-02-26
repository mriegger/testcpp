#include "TextureSampling.h"

TextureSampling::TextureSampling(const float2 textureDimensions) : m_textureDimensions(textureDimensions)
{
    FillTexture();
}

float TextureSampling::BilinearSampleReference(const float2 uv)
{
    // From Anki question How is bilinear and trilinear filtering performed?
    float u = uv.x * m_textureDimensions.x - 0.5f;
    float v = uv.y * m_textureDimensions.y - 0.5f;
    int x = (int)floor(u);
    int y = (int)floor(v);
    float u_ratio = u - x;
    float v_ratio = v - y;
    float u_opposite = 1 - u_ratio;
    float v_opposite = 1 - v_ratio;

    float result = (m_texture[x][y] * u_opposite + m_texture[x + 1][y] * u_ratio) * v_opposite +
        (m_texture[x][y+1] * u_opposite + m_texture[x+1][y+1] * u_ratio) * v_ratio;

    return result;
}

void TextureSampling::FillTexture()
{
    float startValue = 1;
    m_texture.resize((size_t)m_textureDimensions.y);
    for (auto& row : m_texture)
    {
        row.resize((size_t)m_textureDimensions.x);
        FillRow(row, startValue);
        startValue += m_textureDimensions.x;
    }
}

void TextureSampling::FillRow(std::vector<float>& row, const float startValue)
{
    float currValue = startValue;
    for (size_t i = 0; i < row.size(); ++i)
    {
        row[i] = currValue;
        currValue += 1.0f;
    }
}

