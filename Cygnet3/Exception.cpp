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
#include "Exception.hpp"

namespace cg {
namespace ex {

std::string ExNullPointer::ToString() const
{
	return "The poitner is null.";
}
std::string ExNotImplemented::ToString() const
{
	return "The function or class is not implemented.";
}
std::string ExIndexBounds::ToString() const
{
	return "The index is out of bounds.";
}
std::string ExDoesNotOwn::ToString() const
{
	return "The data is not owned by the object.";
}


std::string ExStillRunning::ToString() const
{
	return "The object is still running.";
}

std::string ExNotRunning::ToString() const
{
	return "The object is not running.";
}

}
}
