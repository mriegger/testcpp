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
#include "TextureSampling.h"
#include "DiningPhilosophers.h"
#include "AsyncLinkedListTester.h"
#include "LinearAlgebra.h"
#include "Matrix4x4.h"
#include "Endianness.h"

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

class Solution {
public:

    struct Trie
    {
        Trie() {
            fill(children.begin(), children.end(), nullptr);
        }
        bool wordEndsHere = false;
        string word;
        array<unique_ptr<Trie>, 26> children;

        Trie* AddChild(char c)
        {
            int index = c - 'a';
            children[index] = make_unique<Trie>();
            return children[index].get();
        }

        bool HasChild(char c)const
        {
            if (c == '#')
                return false;

            int charIndex = c - 'a';
            return children[charIndex].get() != nullptr;
        }

        Trie* GetChild(char c)const
        {

            int charIndex = c - 'a';
            return children[charIndex].get();
        }

        void AddWord(const string& s, int index = 0)
        {
            if (index >= s.size())
            {
                wordEndsHere = true;
                word = s;
                return;
            }

            char currChar = s.at(index);
            int charIndex = currChar - 'a';
            Trie* child = children[charIndex].get();
            if (child == nullptr)
            {
                child = AddChild(currChar);
            }
            child->AddWord(s, index + 1);
        }
    };

    unordered_set<string>  res;

    void find(vector<vector<char>>& board, Trie* trie, int row, int col)
    {
        if (trie->wordEndsHere)
        {
            res.emplace(trie->word);
            trie->wordEndsHere = false;
        }

        if (row < 0 || col < 0 || row >= board.size() || col >= board[0].size())return;


        char boardLetter = board[row][col];

        if (trie->HasChild(boardLetter) == false)return;

        board[row][col] = '#';

        find(board, trie->GetChild(boardLetter), row + 1, col);
        find(board, trie->GetChild(boardLetter), row - 1, col);
        find(board, trie->GetChild(boardLetter), row, col + 1);
        find(board, trie->GetChild(boardLetter), row, col - 1);

        board[row][col] = boardLetter;
    }

    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {

        vector<string> vres;
        if (board.empty() || board[0].empty())
        {
            return vres;
        }

        auto root = make_unique<Trie>();
        for (const auto& w : words)
        {
            root->AddWord(w);
        }

        // visited = vector<vector<bool>>(board.size(), vector<bool>(board[0].size(), false));
        for (int row = 0; row < board.size(); ++row)
        {
            for (int col = 0; col < board[0].size(); ++col)
            {
                find(board, root.get(), row, col);
            }
        }
        for (const auto& elem : res)vres.push_back(elem);

        return vres;
    }
};

class Solution2 {
    class Trie {
    public:
        Trie* children[26]; // pointers to its substrings starting with 'a' to 'z'
        bool leaf; // if the node is a leaf, or if there is a word stopping at here
        int idx; // if it is a leaf, the string index of the array words
        Trie()
        {
            this->leaf = false;
            this->idx = 0;
            fill_n(this->children, 26, nullptr);
        }
    };

public:
    void insertWords(Trie* root, vector<string>& words, int idx)
    {
        int pos = 0, len = words[idx].size();
        while (pos < len)
        {
            if (nullptr == root->children[words[idx][pos] - 'a']) root->children[words[idx][pos] - 'a'] = new Trie();
            root = root->children[words[idx][pos++] - 'a'];
        }
        root->leaf = true;
        root->idx = idx;
    }

    Trie* buildTrie(vector<string>& words)
    {
        Trie* root = new Trie();
        int i;
        for (i = 0; i < words.size(); i++) insertWords(root, words, i);
        return root;
    }

    void checkWords(vector<vector<char>>& board, int i, int j, int row, int col, Trie* root, vector<string>& res, vector<string>& words)
    {
        char temp;
        if (board[i][j] == 'X') return; // visited before;
        if (nullptr == root->children[board[i][j] - 'a']) return; // no string with such prefix
        else
        {
            temp = board[i][j];
            if (root->children[temp - 'a']->leaf)  // if it is a leaf
            {
                res.push_back(words[root->children[temp - 'a']->idx]);
                root->children[temp - 'a']->leaf = false; // set to false to indicate that we found it already
            }
            board[i][j] = 'X'; //mark the current position as visited
// check all the possible neighbors
            if (i > 0) checkWords(board, i - 1, j, row, col, root->children[temp - 'a'], res, words);
            if ((i + 1) < row) checkWords(board, i + 1, j, row, col, root->children[temp - 'a'], res, words);
            if (j > 0) checkWords(board, i, j - 1, row, col, root->children[temp - 'a'], res, words);
            if ((j + 1) < col)  checkWords(board, i, j + 1, row, col, root->children[temp - 'a'], res, words);
            board[i][j] = temp; // recover the current position
        }
    }

    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        vector<string> res;
        int row = board.size();
        if (0 == row) return res;
        int col = board[0].size();
        if (0 == col) return res;
        int wordCount = words.size();
        if (0 == wordCount) return res;

        Trie* root = buildTrie(words);

        int i, j;
        for (i = 0; i < row; i++)
        {
            for (j = 0; j<col; j++)
            {
                checkWords(board, i, j, row, col, root, res, words);
            }
        }
        return res;
    }
};
int main()
{
    Timer t;
    t.Start();
    Solution s;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    vector<vector<char>> board;
    board.resize(12);
    for (int i = 0; i < 12; ++i)
    {
        for (int j = 0; j < 12; ++j)
        {
            board[i].push_back('a');
        }
    }
    vector<string> words = vector<string>{ "a", "aa", "aaa", "aaaa", "aaaaa", "aaaaaa", "aaaaaaa", "aaaaaaaa", "aaaaaaaaa", "aaaaaaaaaa" };
    vector<string> res = s.findWords(board, words);
    end = std::chrono::system_clock::now();
    //std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    //std::chrono::duration<double> elapsed_seconds = end - start;
    //  cout  << "elapsed time: " << elapsed_seconds.count() << "s\n";
    t.Stop();
    cout << t.GetElapsedSeconds() << endl;
    return 0;
};


