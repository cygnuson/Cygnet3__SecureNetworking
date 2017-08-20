/*

(C) Matthew Swanson

This file is part of Cygnet3.

Cygnet3 is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

Cygnet3 is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Cygnet3.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "Endian.hpp"

namespace cg {

const uint32_t Endian::__ENDIANNESS_CHECKER = 1;

const bool Endian::big
= *((uint8_t*)(&__ENDIANNESS_CHECKER)) == 0;

const bool Endian::little
= *((uint8_t*)(&__ENDIANNESS_CHECKER)) != 0;

}