#pragma once

#include <string_view>
#include <vector>
#include <span>

class PPMCreator
{
public:

	void Create(const std::string_view filename, const int width, const int height, const std::span<char> rgbData);

private:

};