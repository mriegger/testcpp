#include "IntersectionTests.h"
#include "Math.h"
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

float2 IntersectionTests::ClosestPointToLineSegment(const float2 c, const float2 a, const float2 b)
{
	const auto ab = b - a;
	float t = dot(c - a, ab) / dot(ab, ab);
	if (t < 0) t = 0;
	if (t > 1) t = 1;
	return a + t * ab;
}

float IntersectionTests::RayTriangle(const float3 rayStart, const float3 rayDirection, const Triangle& triangle, float3* outBaryCoords)
{
	const float3 e0 = triangle.m_positions[2] - triangle.m_positions[0];
	const float3 e1 = triangle.m_positions[1] - triangle.m_positions[0];
	float3 normal = normalize(cross(e0, e1));
	
	// What if we hit the backside of the triangle?
	if (dot(normal, rayDirection) < 0)
		normal = -normal;

	const float d = dot(triangle.m_positions[0], normal);
	const float denom = dot(rayDirection, normal);
	if (fabs(denom) < 0.00001f)
	{
		return -1;
	}
	const float t = d - dot(rayStart, normal) / denom;
	const float3 p = rayStart + rayDirection * t;

	const float areaTriangle2 = length(cross(e0, e1));
	const float b0 = length(cross(p - triangle.m_positions[1], p - triangle.m_positions[2])) / areaTriangle2;
	const float b1 = length(cross(p - triangle.m_positions[0], p - triangle.m_positions[2])) / areaTriangle2;

	const float b2 = 1.0f - b0 - b1;
	if (b0 < 0 || b0 > 1 || b1 < 0 || b1 > 1)
		return -1;

	if (b2 < 0)
		return -1;

	if (outBaryCoords)
	{
		*outBaryCoords = float3(b0, b1, b2);
	}

	return t;
}

bool IntersectionTests::DoesRayHitAABB(const float3 rayStart, const float3 rayDirection, const float3 aabbMin, const float3 aabbMax)
{
	// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

	// Ray-plane intersection against the two planes oriented facing the x-axis
	float tmin = (aabbMin.x - rayStart.x) / rayDirection.x;
	float tmax = (aabbMax.x - rayStart.x) / rayDirection.x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (aabbMin.y - rayStart.y) / rayDirection.y;
	float tymax = (aabbMax.y - rayStart.y) / rayDirection.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	// if the closest near hit on a plane is further than the furthest hit on the other plane, then we missed the box
	if ((tmin > tymax) || (tymin > tmax))
		return false;

	// choose the furthest near and the nearest far hits
	tmin = std::max(tmin, tymin);
	tmax = std::min(tymax, tmax);

	float tzmin = (aabbMin.z - rayStart.z) / rayDirection.z;
	float tzmax = (aabbMax.z - rayStart.z) / rayDirection.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	tmin = std::max(tzmin, tmin);
	tmax = std::min(tmax, tzmax);

	return true;
}

void IntersectionTests::Test()
{
	TestRayPlane();
	TestRaySphere();
	TestRayAABB();
	TestRayTriangle();
	TestClosestPointToLineSegment();
}

void IntersectionTests::TestRayTriangle()
{
	Triangle tri{};
	tri.m_positions[0] = float3(0, 0, 100);
	tri.m_positions[1] = float3(0, 1, 100);
	tri.m_positions[2] = float3(1, 0, 100);

	const float shouldHit = RayTriangle(float3(0.1f, 0.1, 0), float3(0, 0, 1), tri);
	const float shouldMiss = RayTriangle(float3(1,1,0), float3(0,0,1), tri);
	const float backwards = RayTriangle(float3(0.1f, 0.1, 200), float3(0, 0, -1), tri);

	float3 baryCenterHit;
	RayTriangle(float3(1.0f / 3.0f, 1.0f / 3.0f, 0), float3(0, 0, 1), tri, &baryCenterHit);

	float3 baryHitFirstVertex;
	RayTriangle(float3(0.0000001f, 0.0000001f, 0), float3(0, 0, 1), tri, &baryHitFirstVertex);

	float3 baryHitSecondVertex;
	RayTriangle(float3(0.0000000, 0.99999999f, 0), float3(0, 0, 1), tri, &baryHitSecondVertex);


	float3 baryHitThirdVertex;
	RayTriangle(float3(0.99999999f, 0.0000001f, 0), float3(0, 0, 1), tri, &baryHitThirdVertex);

	assert(shouldHit > 0);
	assert(shouldMiss < 0);
	assert(backwards > 0);

	assert(Math::ApproxEqual(baryHitFirstVertex, float3(1,0,0)));
	assert(Math::ApproxEqual(baryHitSecondVertex, float3(0, 1, 0)));
	assert(Math::ApproxEqual(baryHitThirdVertex, float3(0, 0, 1)));
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

void IntersectionTests::TestRayAABB()
{
	const float3 aabbMin(-10, -10, -10);
	const float3 aabbMax(10, 10, 10);

	assert(DoesRayHitAABB(float3(-20,0,0), float3(1,0,0), aabbMin, aabbMax));
	assert(!DoesRayHitAABB(float3(-20, -11, 0), float3(1, 0, 0), aabbMin, aabbMax));
	assert(!DoesRayHitAABB(float3(-15, -15, 0), normalize(float3(10, 1, 0)), aabbMin, aabbMax));
}

void IntersectionTests::TestClosestPointToLineSegment()
{
	const float2 a(-1, 0);
	const float2 b(5, 0);
	
	const float2 c(0, 0.25f);

	auto closetPt = ClosestPointToLineSegment(c, a, b);
	assert(closetPt == float2(0, 0));

}