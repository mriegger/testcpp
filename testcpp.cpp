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
#include <map>
#include <mutex>
#include <set>
#include <type_traits>
#include <thread>
#include <vector>
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

int NUM_BINS_PER_TILE = 32;

#define DEG_TO_RAD 0.017453292519943295769236907684886f

std::vector<uint32_t> tile_opaque;

void MarkLightAsVisibleInSharedMemory(uint lightIndex, uint base)
{
    const uint bucket_index = lightIndex / 32;
    const uint bucket_place = lightIndex % 32;
    //    InterlockedOr(tile_opaque[bucket_index], 1 << bucket_place);
    tile_opaque[bucket_index + base] |= 1 << bucket_place;
}

struct Point2D
{
    auto operator <=> (const Point2D&) const = default;
    float x, y;
};

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

vector<mutex> forks;
atomic<uint64_t> numPlatesEaten;

void philo(int id)
{
    chrono::milliseconds dur(20);
    while (true)
    {
        {
            int leftId = id;
            int rightId = (id + 1) % 5;


            if (id == 4)
            {
                swap(leftId, rightId);
            }

            forks[leftId].lock();
            this_thread::sleep_for(dur); //Leads to deadlock immediately


            forks[rightId].lock();
            numPlatesEaten++;
            cout << "philo" << id << "is eating" << endl;


            chrono::milliseconds dur2(rand() % 200 + 100);
            this_thread::sleep_for(dur2);


            forks[rightId].unlock();
            forks[leftId].unlock();
        }
    }
}

int MySqrt(unsigned long long xx)
{
    unsigned long long l = 0;
    unsigned long long r = xx;
    while (l <= r)
    {
        unsigned long long x = l + (r - l) / 2;
        unsigned long long possXX = x * x;
        if (possXX == xx)
        {
            return x;
        }
        else if (possXX > xx)
        {
            r = x - 1;
        }
        else
        {
            l = x + 1;
        }
    }
    return r;
}

double MySqrt(double x)
{
    double l = 1.0;
    double r = x;

    if (x < 1)
    {
        l = 0.0;
        r = 1.0;
    }

    while (true)
    {
        const double possibleAnswer = l + (r - l) / 2.0;
        const double testingAnswerAccuracy = possibleAnswer * possibleAnswer;
        const double diff = x - testingAnswerAccuracy;
        if (abs(diff) < 0.0001)
            return possibleAnswer;
        else if (diff > 0)
        {
            l = possibleAnswer + 0.0001;
        }
        else
        {
            r = possibleAnswer - 0.0001;
        }
    }
}

void testMySqrt()
{
    RandomNumberGenerator rng;
    constexpr int numIter = 1000000;
    for (int i = 0; i < numIter; ++i)
    {
        double squaredValue = rng.GetRandomDouble(0, 100000.0);
        double actual = sqrt(squaredValue);
        double test = MySqrt(squaredValue);
        double difference = abs(actual - test);
        assert(difference < 0.0001);
    }
    cout << "Done" << endl;
}
//
//class Solution {
//public:
//    int calculate(string s)
//    {
//        int length = s.length();
//        if (length == 0) return 0;
//        int currentNumber = 0, lastNumber = 0, result = 0;
//        char sign = '+';
//        for (int i = 0; i < length; i++) {
//            char currentChar = s[i];
//            if (isdigit(currentChar)) {
//                currentNumber = (currentNumber * 10) + (currentChar - '0');
//            }
//            if (!isdigit(currentChar) && !iswspace(currentChar) || i == length - 1) {
//                if (sign == '+' || sign == '-') {
//                    result += lastNumber;
//                    lastNumber = (sign == '+') ? currentNumber : -currentNumber;
//                }
//                else if (sign == '*') {
//                    lastNumber = lastNumber * currentNumber;
//                }
//                else if (sign == '/') {
//                    lastNumber = lastNumber / currentNumber;
//                }
//                sign = currentChar;
//                currentNumber = 0;
//            }
//        }
//        result += lastNumber;
//        return result;
//    }
//};
class Solution {
public:
    int calculate(string s)
    {
        int length = s.length();
        if (length == 0) return 0;
        int currentNumber = 0, lastNumber = 0, result = 0;
        char sign = '+';
        for (int i = 0; i < length; i++) {
            char currentChar = s[i];
            if (isdigit(currentChar)) {
                currentNumber = (currentNumber * 10) + (currentChar - '0');
            }
            if (!isdigit(currentChar) && !iswspace(currentChar) || i == length - 1) {
                if (sign == '+' || sign == '-') 
                {
                }
                else if (sign == '*') 
                {
                }
                else if (sign == '/') 
                {

                }
            }
        }
        result += lastNumber;
        return result;
    }
};

float3x3 MatrixFromQuaternion(float4 quaternion)
{
    float tx = quaternion.x * 2.0f;
    float ty = quaternion.y * 2.0f;
    float tz = quaternion.z * 2.0f;
    float twx = quaternion.w * tx;
    float twy = quaternion.w * ty;
    float twz = quaternion.w * tz;
    float txy = quaternion.x * ty;
    float txx = quaternion.x * tx;
    float txz = quaternion.x * tz;
    float tyy = quaternion.y * ty;
    float tyz = quaternion.y * tz;
    float tzz = quaternion.z * tz;

    float3x3 m = float3x3(1.0f - tyy - tzz, txy - twz, txz + twy,
        txy + twz, 1.0f - txx - tzz, tyz - twx,
        txz - twy, tyz + twx, 1.0f - txx - tyy);
    return m;
}

// SortedList
// 



int main()
{
    BasicCalculator bc;
    bc.Test();
    return 0;
};


