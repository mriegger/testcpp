// testcpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

import MyModuleName;
import ProducerConsumer;
import ReaderWriter;

#define D_REVERSED                                      1
#define PERMUTATION_DEPTH D_REVERSED
#include <iostream>
#include <ranges>
#include <concepts>
#include <cassert>
#include "Colors.h"
#include <numeric>
#include <map>
#include <coroutine>
#include <fstream>
#include <mutex>
#include <set>
#include <type_traits>
#include <thread>
#include <vector>
#include <xmmintrin.h>
#include "timer.h"
#include <functional>
#include <array>
#include <semaphore>
#include <unordered_set>
#include <atomic>
#include "hlsl.h"
#include "BinarySearch.h"
#include "TextureSampling.h"
#include "DiningPhilosophers.h"
#include "DivideIntegers.h"
#include "AsyncLinkedListTester.h"
#include "LinearAlgebra.h"
#include "matrix4x4.h"
#include "UnionFind.h"
#include "BestMeetingPoint.h"
#include "PPMCreator.h"
#include "Endianness.h"
#include "BasicCalculator.h"
#include "SpaceshipOperator.h"
#include "TopologicalSort.h"
#include "DivideIntegers.h"
#include "Sorting.h"
#include "RandomNumberGenerator.h"
#include "IntersectionTests.h"
#include "ShadowMapAtlas.h"

using namespace std;


#define DEG_TO_RAD 0.017453292519943295769236907684886f


struct RayRayIntersectOutput
{
    float t0;
    float t1;
    bool intersects = false;
};

RayRayIntersectOutput RayRayIntersect(float3 o0, float3 o1, float3 r0, float3 r1)
{
    RayRayIntersectOutput o;

    r0 = normalize(r0);
    r1 = normalize(r1);

    float r0_dot_r1 = dot(r0, r1);
    float3 a = o0 - o1;
    float a_dot_r0 = dot(a, r0);
    float a_dot_r1 = dot(a, r1);

    float denom0 = (a_dot_r1 * r0_dot_r1 - a_dot_r0);

    if (fabs(denom0) > 0.001)
    {
        float t0 = (1 - r0_dot_r1 * r0_dot_r1) / denom0;
        o.t0 = t0;
    }
    o.intersects = o.t0 >= 0;
    return o;
}


int main()
{
    IntersectionTests::Test();

    //PPMCreator ppm;

    //auto colVec = Colors::GetColorVector();

    //int numRowBlocks = 4;
    //int numColBlocks = 4;
    //int w = 4096;
    //int h = 4096;
    //int blockWidth = w / numRowBlocks;
    //int blockHeight = h / numColBlocks;
    //vector<uint8_t> data(w * h * 3, 50);
    //ppm.SetImageData(data, w);
  

    //for (int j = 0; j < h; ++j)
    //{
    //    for (int i = 0; i < w; ++i)
    //    {
    //        const int blockRow = j / blockHeight;
    //        const int blockCol = i / blockWidth;
    //        const int blockIndex = blockCol + blockRow * numColBlocks;

    //        const auto colorIndex = blockIndex % colVec.size();
    //        const float3 getColor = colVec[colorIndex];

    //        ppm.SetPixel(i, j, getColor);
    //    }
    //}
    //ppm.Rotate(22.5);

    //ppm.Write("mkrhello.ppm");
    return 0;
};


