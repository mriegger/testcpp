#pragma once

// See Chapter 7 - Real-Time Collision Detection
class SparseHashGrid
{
public:



private:

	struct Cell
	{
		Cell() = default;
		Cell(int x, int y, int z);
		int x, y, z;
	};

	int ComputeHashBucketIndex(Cell cellPos);

	static constexpr int NumBuckets = 1024;
};