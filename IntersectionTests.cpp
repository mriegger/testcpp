#include "IntersectionTests.h"
#include <cassert>

float3 IntersectionTests::RayPlane(const float3 rayStart, const float3 rayDirection, const float4 plane)
{
	// https://courses.cs.washington.edu/courses/cse557/09au/lectures/extras/triangle_intersection.pdf
	const float3 N = plane.xyz();
	const float rDotN = dot(rayDirection, N);
	const float oDotN = dot(rayStart, N);
	const float d = plane.w;
	const float t = (d - oDotN) / rDotN;
	return rayStart + rayDirection * t;
}

float3 IntersectionTests::RaySphere(const float3 rayStart, const float3 rayDirection, const float3 spherePos, const float sphereRadius)
{
	const float3 e = spherePos - rayStart;
	const float a = dot(e, rayDirection) / length(rayDirection);
	const float b = sqrtf(dot(e,e) - a*a);
	const float f = sqrtf(sphereRadius * sphereRadius - b*b);
	const float t = a - f;
	return rayStart + rayDirection * t;
}

void IntersectionTests::Test()
{
	TestRayPlane();
	TestRaySphere();
}

void IntersectionTests::TestRayPlane()
{
	IntersectionTests test;
	const float3 planeNormal = normalize(float3(1, 0, 1));
	const float3 pointOnPlane = float3(0, 0, 2);
	const float3 rayStart(10.0f, 0.0f, 2.0f);
	const float3 rayDirection(-1, 0, 0);
	const float3 intersectionPoint = test.RayPlane(rayStart, rayDirection, float4(planeNormal, dot(planeNormal, pointOnPlane)));

	const float3 expectedHitPoint(0, 0, 2);
	assert(length(expectedHitPoint - intersectionPoint) < 0.00001f);
}

void IntersectionTests::TestRaySphere()
{
	const float3 spherePos(0, 0, 0);
	const float sphereRadius = 10.0f;
	const float3 rayStart(-100, 0, 0);
	const float3 rayDir(1, 0, 0);
	const float3 hitPoint = RaySphere(rayStart, rayDir, spherePos, sphereRadius);
	const float3 expectedHitPoint(-sphereRadius, 0, 0);
	assert(length(expectedHitPoint - hitPoint) < 0.0001f);
}
