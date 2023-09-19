//
// Created by cygnus330 on 2023-04-05.
//

#ifndef _BLOTTO_H_
#define _BLOTTO_H_

#include <vector>
#include <cstdint>

namespace blt
{
	void gen(uint64_t, uint64_t, std::vector<uint64_t>&);

	int32_t game(std::vector<uint64_t>&, std::vector<uint64_t>&);
	int32_t game(std::vector<uint64_t>&, std::vector<uint64_t>&, std::vector<uint64_t>&);
} // blt

#endif //_BLOTTO_H_
