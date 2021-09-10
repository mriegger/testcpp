#pragma once
#include "hlsl.h"

class IntersectionTests
{
public:

	// plane equation is ax+by+cz=d where the plane is stored as (a,b,c,d)
	static float3 RayPlane(const float3 rayStart, const float3 rayDirection, const float4 plane);
	static float3 RaySphere(const float3 rayStart, const float3 rayDirection, const float3 spherePos, const float sphereRadius);
	static bool DoesRayHitAABB(const float3 rayStart, const float3 rayDirection, const float3 aabbMin, const float3 aabbMax);

	static void Test();

private:

	static void TestRayPlane();
	static void TestRaySphere();
	static void	TestRayAABB();
};