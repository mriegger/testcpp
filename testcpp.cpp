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
#include <functional>
#include <array>
#include <semaphore>
#include <unordered_set>
#include <atomic>
#include "hlsl.h"
#include "TextureSampling.h"
#include "DiningPhilosophers.h"
#include "AsyncLinkedListTester.h"

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

class Solution
{
    int m_result = 0;
    bool m_roundsPerfectly = false;

    bool divideByDoubling(unsigned int dividend, unsigned divisor)
    {
        if (dividend == 0)
        {
            m_roundsPerfectly = true;
            return true;
        }
        if (divisor > dividend)
        {
            return false;
        }
        unsigned int count = 1;
        auto currSubtractValue = divisor;
        while (currSubtractValue < (UINT_MAX >> 1) && currSubtractValue + currSubtractValue <= dividend)
        {
            currSubtractValue += currSubtractValue;
            count += count;
        }
        m_result += count;
        dividend -= currSubtractValue;
        return divideByDoubling(dividend, divisor);
    }

    int sign(int x)
    {
        if (x == 0)
        {
            return 0;
        }
        return x < 0 ? -1 : 1;
    }

public:
    int divide(int dividend, int divisor)
    {
        int numLessZero = dividend < 0 ? 1 : 0;
        numLessZero += divisor < 0 ? 1 : 0;

        if (dividend == INT_MIN && abs(divisor) == 1)
        {
            if (numLessZero == 1)
            {
                return INT_MIN;
            }
            else
            {
                return INT_MAX;
            }
        }


        unsigned int a = dividend != INT_MIN ? abs(dividend) : -INT_MAX;
        unsigned int b = divisor != INT_MIN ? abs(divisor) : -INT_MAX;


        divideByDoubling(a, b);
        auto res = numLessZero == 1 ? -m_result : m_result;
        if (m_roundsPerfectly == false && numLessZero == 1)
        {
            res--;
        }

        return res;
    }
};

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



int main()
{
    ReaderWriter rw;
    rw.Run();


    return 0;
};

