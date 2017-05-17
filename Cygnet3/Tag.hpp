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