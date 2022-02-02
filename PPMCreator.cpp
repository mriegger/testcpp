#include "PPMCreator.h"
#include <fstream>
#include <string>
#include <iostream>

namespace
{
    static constexpr std::string_view MagicNumber = "P6\n";
    static constexpr std::string_view MaxValue = "255\n";
    static constexpr int NumColorChannels = 3;

    void WritePPMHeader(std::ofstream& f, const int width, const int height)
    {
        f.write(MagicNumber.data(), MagicNumber.size());
        const std::string widthHeightStr = std::to_string(width) + " " + std::to_string(height) + "\n";
        f.write(widthHeightStr.data(), widthHeightStr.size());
        f.write(MaxValue.data(), MaxValue.size());
    }
}

void PPMCreator::Create(const std::string_view filename, const int width, const int height, const std::span<char> rgbData)
{
    using namespace std;
    ofstream f;
    f.open(filename.data(), ios::out | ios::binary);

    WritePPMHeader(f, width, height);

    f.write(rgbData.data(), rgbData.size());
    f.close();
}

void PPMCreator::SetImageData(const std::span<char> rgbData, const int width)
{ 
    m_width = width;
    const int numPixels = rgbData.size();
    m_height = numPixels / NumColorChannels / m_width;
    m_imageData.resize(rgbData.size()); 
    std::copy(rgbData.begin(), rgbData.end(), m_imageData.begin()); 
}

void PPMCreator::SetPixel(const int x, const int y, const char red, const char green, const char blue)
{
    if (x < 0 || y < 0 || x >= m_width || y >= m_height)
    {
        std::cerr << "PPMCreator::SetPixel() called with out of bounds data" << std::endl;
        return;
    }
    const int index = (x + y * m_width) * NumColorChannels;
    m_imageData[index+0] = red;
    m_imageData[index+1] = green;
    m_imageData[index+2] = blue;
}

void PPMCreator::Write(const std::string_view filename)
{
    using namespace std;
    ofstream f;
    f.open(filename.data(), ios::out | ios::binary);
    WritePPMHeader(f, m_width, m_height);
    f.write(m_imageData.data(), m_imageData.size());

    f.close();
}