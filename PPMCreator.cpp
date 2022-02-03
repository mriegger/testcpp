#include "PPMCreator.h"
#include <fstream>
#include <string>
#include <iostream>

namespace
{
    static constexpr std::string_view MagicNumber = "P6\n";
    static constexpr std::string_view MaxValue = "255\n";
    static constexpr int NumColorChannels = 3;

    void WritePPMHeader(std::ofstream& f, const size_t width, const size_t height)
    {
        f.write(MagicNumber.data(), MagicNumber.size());
        const std::string widthHeightStr = std::to_string(width) + " " + std::to_string(height) + "\n";
        f.write(widthHeightStr.data(), widthHeightStr.size());
        f.write(MaxValue.data(), MaxValue.size());
    }
}

void PPMCreator::Create(const std::string_view filename, const int width, const std::span<uint8_t> rgbData)
{
    using namespace std;
    ofstream f;
    f.open(filename.data(), ios::out | ios::binary);
    const auto numPixels = rgbData.size();
    const auto height = numPixels / NumColorChannels / width;

    WritePPMHeader(f, width, height);

    f.write(reinterpret_cast<const char*>(rgbData.data()), rgbData.size());
    f.close();
}

void PPMCreator::SetImageData(const std::span<uint8_t> rgbData, const int width)
{ 
    m_width = width;
    const auto numPixels = rgbData.size();
    m_height = numPixels / NumColorChannels / m_width;
    m_imageData.resize(rgbData.size()); 
    std::copy(rgbData.begin(), rgbData.end(), m_imageData.begin()); 
}

std::vector<uint8_t> PPMCreator::GetImageData() const
{
    return m_imageData;
}

void PPMCreator::SetPixel(const int x, const int y, const uint8_t red, const uint8_t green, const uint8_t blue)
{
    if (x < 0 || y < 0 || x >= m_width || y >= m_height)
    {
        std::cerr << "PPMCreator::SetPixel() called with out of bounds data" << std::endl;
        return;
    }
    const auto index = (x + y * m_width) * NumColorChannels;
    m_imageData[index+0] = red;
    m_imageData[index+1] = green;
    m_imageData[index+2] = blue;
}

void PPMCreator::SetPixel(const int x, const int y, const float3 color)
{
    if (x < 0 || y < 0 || x >= m_width || y >= m_height)
    {
        std::cerr << "PPMCreator::SetPixel() called with out of bounds data" << std::endl;
        return;
    }
    const auto index = (x + y * m_width) * NumColorChannels;
    const uint8_t red = static_cast<uint8_t>(color.x * 255);
    const uint8_t green = static_cast<uint8_t>(color.y * 255);
    const uint8_t blue = static_cast<uint8_t>(color.z * 255);

    m_imageData[index + 0] = red;
    m_imageData[index + 1] = green;
    m_imageData[index + 2] = blue;
}

float3 PPMCreator::GetPixel(const int x, const int y) const
{
    if (x < 0 || y < 0 || x >= m_width || y >= m_height)
    {
        std::cerr << "PPMCreator::GetPixel() called with out of bounds data" << std::endl;
        return float3(0,0,0);
    }
    const auto index = (x + y * m_width) * NumColorChannels;
    const float red = m_imageData[index + 0];
    const float green = m_imageData[index + 1];
    const float blue = m_imageData[index + 2];
    return float3(red, green, blue) / 255.0f;
}


void PPMCreator::Write(const std::string_view filename)
{
    using namespace std;
    ofstream f;
    f.open(filename.data(), ios::out | ios::binary);
    WritePPMHeader(f, m_width, m_height);
    f.write(reinterpret_cast<const char*>(m_imageData.data()), m_imageData.size());

    f.close();
}

void PPMCreator::Rotate(const float degrees)
{
    const float2 translateXY(m_width / 2.0f, m_height / 2.0f);
    std::vector<uint8_t> dest(m_imageData.size(), 0);
    
    PPMCreator destPPM;
    std::span<uint8_t> sp = dest;
    destPPM.SetImageData(sp, m_width);

    const float rad = degrees * 3.14159f / 180.0f;
    const float3x3 invTranslate = { 1.0, 0.0f, -translateXY.x,
        0.0f, 1.0f, -translateXY.y,
        0,0,1 
    };

    const float3x3 rotate = {cos(rad), -sin(rad), 0.0f,
        sin(rad), cos(rad), 0.0f,
        0,0,1
    };

    const float3x3 translate = { 1.0, 0.0f, translateXY.x,
        0.0f, 1.0f, translateXY.y,
        0,0,1
    };

    float3x3 final = mul(rotate, invTranslate);
    final = mul(translate, final);
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            float3 color = GetPixel(x, y);
            float3 srcVertex(x, y, 1);
            float3 destVertex = mul(final, srcVertex);
            
            if (destVertex.x >= 0 && destVertex.y >= 0 && destVertex.x < m_width && destVertex.y < m_height)
            {
                destPPM.SetPixel(destVertex.x, destVertex.y, color);
            }
        }
    }
    *this = destPPM;
}