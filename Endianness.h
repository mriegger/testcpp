#pragma once

inline uint32_t SwapEndianness(const uint32_t num)
{
	uint32_t b0, b1, b2, b3;
	uint32_t res;

	b0 = (num & 0x000000ff) << 24u;
	b1 = (num & 0x0000ff00) << 8u;
	b2 = (num & 0x00ff0000) >> 8u;
	b3 = (num & 0xff000000) >> 24u;

	res = b0 | b1 | b2 | b3;
	return res;
}