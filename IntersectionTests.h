#pragma once
#include "hlsl.h"

class IntersectionTests
{
public:

	struct Triangle
	{
		std::array<float3, 3> m_positions;
	};

	// plane equation is ax+by+cz=d where the plane is stored as (a,b,c,d)
	static float3 RayPlane(const float3 rayStart, const float3 rayDirection, const float4 plane);
	// returns t value intersection distance, -1 if no hit
	static float RayTriangle(const float3 rayStart, const float3 rayDirection, const Triangle& triangle, float3* outBaryCoords = nullptr);
	static float3 RaySphere(const float3 rayStart, const float3 rayDirection, const float3 spherePos, const float sphereRadius);
	static bool DoesRayHitAABB(const float3 rayStart, const float3 rayDirection, const float3 aabbMin, const float3 aabbMax);
	// 2D test, given a point, return the closest point on the given line segment
	static float2 ClosestPointToLineSegment(const float2 c, const float2 a, const float2 b);

	static void Test();

private:

	static void TestClosestPointToLineSegment();
	static void TestRayPlane();
	static void TestRaySphere();
	static void	TestRayAABB();
	static void TestRayTriangle();
};