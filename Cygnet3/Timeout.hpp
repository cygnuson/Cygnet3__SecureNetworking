#pragma once

#include <cstdint>

namespace cg {

/**Timeout enums.*/
enum class Timeout : int64_t
{
	Inf = -1,
	None = 0,
	Sec_5 =   5000000000,
	Sec_10 = 10000000000,
};

}