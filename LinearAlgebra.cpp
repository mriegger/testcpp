#include "LinearAlgebra.h"

namespace LinearAlgebra
{
	float DistanceBetweenTwoRays(float3 s1, float3 v1, float3 s2, float3 v2)
	{
		// from Anki: How would you find the distance between two lines? (Game Math version)

		const float v1_dot_v2 = dot(v1, v2);
		const float v1v1 = dot(v1, v1);
		const float v2v2 = dot(v2, v2);
		const float3 s2_minus_s1 = s2 - s1;
		
		const float lhs = 1.0f / (v1_dot_v2 * v1_dot_v2 - v1v1 * v2v2);

		const float2 rhs = float2(dot(s2_minus_s1, v1),
			dot(s2_minus_s1, v2));

		const float2 row0 = float2(-v2v2, v1_dot_v2);
		const float2 row1 = float2(-v1_dot_v2, v1v1);

		float2 middle;
		middle.x = dot(row0, rhs);
		middle.y = dot(row1, rhs);

		float2 t = middle * lhs;

		t = clamp(t, 0.0f, FLT_MAX);

		float3 closest1 = s1 + v1 * t.x;
		float3 closest2 = s2 + v2 * t.y;

		return length(closest1 - closest2);
	}
}