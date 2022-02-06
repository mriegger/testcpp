#include "SparseHashGrid.h"

SparseHashGrid::Cell::Cell(int x, int y, int z):x(x), y(y), z(z)
{

}

int SparseHashGrid::ComputeHashBucketIndex(Cell cellPos)
{
	const int h1 = 0x8da6b343;
	const int h2 = 0xd8163841;
	const int h3 = 0xcb1ab31f;
	int n = h1 * cellPos.x + h2 * cellPos.y + h3 * cellPos.z;
	n = n % NumBuckets;
	if (n < 0)
	{
		n += NumBuckets;
	}
	return n;
}
