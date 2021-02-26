#pragma once

#include "hlsl.h"

namespace Matrix4x4
{
	float4x4 MakePerspectiveFovMatrixRH(float fovY, float aspectRatio, float nearDist, float farDist, bool reverseDepth);
}