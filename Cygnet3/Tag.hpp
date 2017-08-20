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

#include <cstddef>

namespace cg {

/**An easy type of tag for dispaching.*/
template<std::size_t N>
struct TagType {
	enum {
		/**The value of the tag.*/
		value = N,
	};
};

/**A group of tags that are commonly used.*/
struct Tags {
	/**The tag for creating without taking ownership or allocation.*/
	using NoOwnTag = cg::TagType<554687>;
	/**one time object for tagging.*/
	static NoOwnTag NoOwn;
	/**The tag for creating and taking ownership, but not copying.*/
	using OwnTag = cg::TagType<554688>;
	/**one time object for tagging.*/
	static OwnTag Own;
	/**The tag tio signify copy of data.*/
	using CopyTag = cg::TagType<554689>;
	/**one time object for tagging.*/
	static CopyTag Copy;
	/**A tag denoting the desire for a time based thing as NOW.*/
	using CurrentTimeTag = cg::TagType<554690>;
	/**A static tag for the current time.*/
	static CurrentTimeTag CurrentTime;
	/**A tag to denote the desire for a thread.*/
	using ThreadTag = cg::TagType<554691>;
	/**A tag for threads*/
	static ThreadTag Thread;
};

}