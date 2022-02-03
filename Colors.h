#pragma once
#include "hlsl.h"
#include <vector>

namespace Colors
{
	static constexpr float3 Red(1, 0, 0);
	static constexpr float3 Green(0, 1, 0);
	static constexpr float3 Blue(0, 0, 1);
	static constexpr float3 White(1, 1, 1);
	static constexpr float3 Black(0, 0, 0);
	static constexpr float3 Yellow(1, 1, 0);
	static constexpr float3 Cyan(0, 1, 1);
	static constexpr float3 Purple(0.5f, 0, 0.5f);
	static constexpr float3 Maroon(0.5f, 0, 0);
	static constexpr float3 Teal(0, 0.5f, 0.5f);
	static constexpr float3 Gray(0.5f, 0.5f, 0.5f);
	static constexpr float3 Silver(0.75f, 0.75f, 0.75f);
	static constexpr float3 Navy(0, 0, 0.5f);
	static constexpr float3 Tan(210 / 255.0f, 180 / 255.0f, 140 / 255.0f);
	static constexpr float3 RoyalBlue(65 / 255.0f, 105 / 255.0f, 225 / 255.0f);
	static constexpr float3 Pink(255 / 255.0f, 192 / 255.0f, 203 / 255.0f);
	static constexpr float3 Brown(165 / 255.0f, 42 / 255.0f, 42 / 255.0f);

	inline static std::vector<float3> GetColorVector()
	{
		std::vector<float3> result;
		result.push_back(Red);
		result.push_back(Green);
		result.push_back(Blue);
		result.push_back(White);
		result.push_back(Black);
		result.push_back(Yellow);
		result.push_back(Cyan);
		result.push_back(Purple);
		result.push_back(Maroon);
		result.push_back(Teal);
		result.push_back(Gray);
		result.push_back(Silver);
		result.push_back(Navy);
		result.push_back(Tan);
		result.push_back(RoyalBlue);
		result.push_back(Pink);
		result.push_back(Brown);
		return result;
	}
}

