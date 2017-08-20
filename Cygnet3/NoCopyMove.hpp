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
#pragma once


namespace cg {
/**Disable the ability to copy an object.*/
struct NoCopy
{
	NoCopy() {};
	NoCopy(const NoCopy& other) = delete;
	void operator=(const NoCopy& other) = delete;
};
/**Disable the ability to move an object.*/
struct NoMove
{
	NoMove() {};
	NoMove(NoMove&& other) = delete;
	void operator=(NoMove&& other) = delete;
};

}