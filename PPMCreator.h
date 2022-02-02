#pragma once

#include <string_view>
#include <vector>
#include <span>

class PPMCreator
{
public:

	// Write an image with a single function
	static void Create(const std::string_view filename, const int width, const int height, const std::span<char> rgbData);
	
	void SetImageData(const std::span<char> rgbData, const int width); 
	void SetPixel(const int x, const int y, const char red, const char green, const char blue);
	void Write(const std::string_view filename);

private:

	std::vector<char> m_imageData;
	int m_width = 128;
	int m_height = 128;
};