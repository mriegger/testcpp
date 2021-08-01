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
#include "LinearAlgebra.h"
#include "Matrix4x4.h"

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

struct MyOps
{
    static int Plus(int a, int b)
    {
        return a + b;
    }
};

template<typename T>
T my_upper_bound(T b, T e, int target)
{
    T iter = b;
    int count = distance(b, e);
    while (count > 0)
    {
        int step = count / 2;
        T curr = iter + step;
        if (*curr <= target)
        {
            iter = curr + 1;
            count -= step + 1;
        }
        else // curr > target
        {
            count = step;
        }
    }
    return iter;
}

template<typename T>
T my_lower_bound(T b, T e, int target)
{
    T iter = b;
    int count = distance(b, e);
    while (count > 0)
    {
        int step = count / 2;
        T test = iter + step;
        if (*test < target)
        {
            iter = test + 1;
            count -= step + 1;
        }
        else if (*test == target)
        {
            iter = test;
            count -= step;
        }
        else
        {
            count /= 2;
        }
    }
    return iter;
}

class Sum
{
public:

    void print(const vector<int>& v) {
        for (const auto elem : v)cout << elem << " ";
        cout << endl;
    }

    void combinationSum(vector<int>& nums, int target) {
        ranges::sort(nums);
        vector<int> partial;
        CombinationSum(nums, target, 0, partial, 0);
    }

    void CombinationSum(const vector<int>& nums, int target, int sum, vector<int>& partial, int start)
    {
        if (sum == target) {
            print(partial);
        }
        for (int i = start; i < nums.size(); ++i) {
            int c = nums[i];
            if (sum + c > target || i > start && nums[i] == nums[i - 1]) {
                continue;
            }
            partial.push_back(c);
            CombinationSum(nums, target, sum + c, partial, i + 1);
            partial.pop_back();
        }
    }

private:

    unordered_map<int, int> memo;
};

void rec(string s, int start, string partial)
{
    if (partial.size() >= s.size())
        cout << partial << endl;

    // dog, 0, ""
    //     dog, 1, d
     //         dog 2, do
    //                  dog, 3, dog
    //          dgo, 2, dg
     //                 dgo, 3, dgo
    //      odg, 1, o
    for (int i = start; i < s.size(); ++i)
    {
        swap(s[i], s[start]);
        rec(s, start + 1, partial + s[start]);
        swap(s[i], s[start]);
    }
}

void rec2(string s, int start, string partial, vector<bool> used)
{
    if (partial.size() >= s.size())
        cout << partial << endl;
    for (int i = 0; i < s.size(); ++i)
    {
        if (used[i])
            continue;

        if (i > 0 && s[i] == s[i - 1] && !used[i-1])
            continue;

        used[i] = true;
        rec2(s, start, partial + s[i], used);
        used[i] = false;
    }
}

void printAnagrams(string s)
{
    string p;
    ranges::sort(s);
    vector<bool> used(s.size(), false);
    rec2(s, 0, p, used);
}

void rec(string sentence, int start, string partial, const unordered_set<string>& dict)
{
    if (start >= sentence.size())
        cout << partial << endl;

    string curr;
    for (int i = start; i < sentence.size(); ++i)
    {
        curr += sentence[i];
        if (dict.count(curr) > 0)
        {
            rec(sentence, i + 1, partial + " " + curr, dict);
        }
    }
}

void breakinput(string sentence, vector<string> dict)
{
    unordered_set<string> usdict;
    for (const auto& elem : dict)usdict.emplace(elem);
    string partial;
    rec(sentence, 0, partial, usdict);
}

void breakinput_bu(string sentence, vector<string> vdict)
{
    //vector<string> dict = { "apple", "pen", "applepen", "pine", "pineapple" };
  //  breakinput("pineapplepenapple", dict);
//    breakinput_bu("pineapplepenapple", dict);

    unordered_set<string> dict;
    for(const auto& elem : vdict)
    {
        string rentry = elem;
        ranges::reverse(rentry);
        dict.emplace(rentry);
    }

    vector<vector<string>> memo(sentence.size());
    for (int i = sentence.size() - 2; i >= 0; --i)
    {
        string curr;
        for (int j = i; j >= 0; --j)
        {
            curr += sentence[j];
            if (dict.contains(curr))
            {
                string rcurr = curr;
                ranges::reverse(rcurr);
                memo[j].push_back(rcurr);
            }
        }
    }
    
}

vector<vector<int>> cache;


int RecursiveNChooseK(int n, int k)
{
    int dim = max(n + 1, k + 1);
    vector<vector<int>> cache(dim, vector<int>(dim, -1));
    for (int i = 0; i < dim; ++i)
    {
        cache[i][0] = 1;
    }
    for (int i = 0; i < dim; ++i)
    {
        cache[i][i] = 1;
    }

    for (int i = 2; i < dim; ++i)
    {
        for (int j = 1; j < i; ++j)
        {
            cache[i][j] = cache[i - 1][j - 1] + cache[i - 1][j];
        }
    }
    return cache[n][k];
    //     k
    // --------------------
    // 1
    // 01
    // 0 1
    // 0  1
    // 0   1
    // 0    1
    // -------------------

    // C(N,K) == C(N-1, K-1) + C(N-1, K)

    if (n == k)return 1;
    if (k == 0)return 1;
    if (cache[n][k] != -1)
    {
        return cache[n][k];
    }
    
    int res = RecursiveNChooseK(n - 1, k - 1) + RecursiveNChooseK(n - 1, k);
    cache[n][k] = res;
    return res;
}


// cost(N) = cost(N-1) + min(min(paint_a, paint_b), paint_c) where paint(n-1) 
// cost(-1) = 0
// paint(-1) = not painted

// returns cost of painting all the houses

vector<int> paintedColor;

int recurse(const vector<vector<int>>& paintCost, int n, int numHouses)
{
    if (n >= numHouses)return 0;
    int bestCost = INT_MAX;
    int prevColor = -1; // not painted
    if (n > 0)
        prevColor = paintedColor[n - 1];

    int bestColor = -1;
    for (int i = 0; i < 3; ++i)
    {
        int paintCostHere = paintCost[n][i];
        if (prevColor == i)
        {
            continue;
        }
        paintedColor[n] = i;
        int costHere = recurse(paintCost, n + 1, numHouses) + paintCostHere;
        if (costHere < bestCost)
        {
            bestColor = i;
            bestCost = costHere;
        }
    }
    paintedColor[n] = bestColor;
    return bestCost;
}

int bu(const vector<vector<int>>& paintCost, int numHouses)
{
    vector<int> cache(numHouses + 1, INT_MAX);
    paintedColor[0] = -1;
    cache[0] = 0;
    for (int i = numHouses ; i > 0 ; --i)
    {
        int prevColor = paintedColor[i - 1];
        for (int j = 0; j < 3; ++j)
        {
            if (prevColor == j)
                continue;
            int paintCostHere = paintCost[i - 1][j] + cache[i - 1];
            if (paintCostHere < cache[i])
            {
                cache[i] = paintCostHere;
                paintedColor[i] = j;
            }
        }
    }
    return cache.back();
}


int recurse(int l, const vector<int>& p)
{
    if (l == 0)
        return 0;
    int best = INT_MIN;
    for (int i = 0; i < l; ++i)
    {
        best = max(best, p[i] + recurse(l-i-1,p));
    }
    return best;
}














vector<int> memo;


int myRecurse(int l, const vector<int>& p)
{
    if (l == 0)
        return 0;

    if (memo[l] != -1)
    {
        return memo[l];
    }

    int best = -1;
    for (int i = 1; i <= l; ++i)
    {
        int priceForThis = p[i - 1];
        int priceForRest = myRecurse(l - i, p);
        int total = priceForRest + priceForThis;
        best = max(best, total);
    }
    memo[l] = best;
    return best;
}

int bu(int l, const vector<int>& p)
{
    vector<int> memo(l + 1, 0);
    memo[1] = p[0];
    for (int i = 2; i <= l; ++i)
    {
        int best = INT_MIN;
        for(int amountToCutHere = i ; amountToCutHere >= 1 ; --amountToCutHere)
        {
            int priceForThis = p[amountToCutHere - 1];
            int priceForRest = memo[i - amountToCutHere];
            int total = priceForRest + priceForThis;
            best = max(best, total);
        }
        memo[i] = best;
    }
    return memo.back();
}

class StoneJump
{
public:

    StoneJump()
    {
        memo = vector<int>(100, INT_MAX);
    }

    int recurse(const vector<int>& cost, int i, int x)
    {
        if (i < 0)
            return INT_MAX;

        if (i == 0)
            return 0;

        if (memo[i] != INT_MAX)
            return memo[i];

        // destCost(i) = cost[i] + min(recuse[i-j]) for j = 1 to X inclusive
        int result = cost[i];
        int best = INT_MAX;
        for (int j = 1; j <= x; ++j)
        {
            int prevJumpCost = recurse(cost, i - j, x);
            best = min(best, prevJumpCost);
        }
        best += cost[i];

        memo[i] = best;

        return best;
    }

    int bu(const vector<int>& v, int x)
    {
        vector<int> memo(v.size() + x, INT_MAX);
        memo[x] = 0;
        for (int i = x + 1; i < v.size()+ x; ++i) // N
        {
            int best = INT_MAX;
            for (int j = 1; j <= x; ++j) // X
            {
                int prevJumpCost = memo[i - j];
                best = min(prevJumpCost, best);
            }
            memo[i] = best + v[i-x];
        }
        return memo.back();
    }

private:
    vector<int> memo;

};

int countbitsrow(int x)
{
    int best = 0;
    int currRun = 0;
    while (x)
    {
        if (x & 1)
        {
            currRun++;
            best = max(best, currRun);
        }
        else
        {
            currRun = 0;
        }
        x = x >> 1;
    }
    return best;
}

int main()
{
    ReaderWriter rw;
    rw.Run();


    return 0;
};


