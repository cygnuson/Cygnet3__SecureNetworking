#pragma once

#include <sstream>
#include <iomanip>


#define __FLN__ cg::StringTogether("\n",__FILE__,":",__LINE__,"\n")
#define __FUNCSTR__ cg::StringTogether(__func__,": ")

namespace cg {

/**An interface for stringable objects.*/
struct Stringable
{
	/**Get the string.
	\return The string.*/
	virtual std::string ToString() const = 0;
};
/**An interface for unstringing objects.*/
struct Unstringable
{
	/**Get the object from a string.
	\param str The string to get from.*/
	virtual void  FromString(const std::string& str) = 0;
};
/**An object to determine if something is stringable or not.*/
template<typename T>
struct IsStringable
{
	/**The type with not type modifiers.*/
	using Type = std::decay_t<T>;
	enum {
		/**The value, true, if the object implements Stringable.*/
		value = std::is_base_of<Stringable, Type>::value,
	};
};
/**An object to determine if something is stringable or not.*/
template<typename T>
struct IsUnstringable
{
	/**The type with not type modifiers.*/
	using Type = std::decay_t<T>;
	enum {
		/**The value, true, if the object implements Stringable.*/
		value = std::is_base_of<Unstringable, Type>::value,
	};
};

/**Convert a bool to a string.
\param b The bool.
\return the string.*/
inline std::string ToString(bool b)
{
	return b ? "true" : "false";
}

/**Convert a string from "true" or "false" to a bool.
\param str The string to convert.
\return True if the str="true" or false otherwise.*/
inline bool FromString(const std::string& str)
{
	return (str == "true" ? true : false);
}

/**Convert char poitners to strings.
\param p The const char ptr.
\return A string object with the letters.*/
inline std::string ToString(const char* p)
{
	return std::string(p);
}

/**Pass a string though the system.
\param str The string.
\return The same string.*/
inline std::string ToString(const std::string& str)
{
	return str;
}

/**Create a string from a fundamental.
\param obj The thing to create a string from.*/
template<typename T>
std::enable_if_t<std::is_fundamental<T>::value, std::string>
ToString(const T& obj)
{
	return std::to_string(obj);
}
/**Create an object from a string.
\param str The string to build from.
\return The object built.*/
template<typename T>
std::enable_if_t<std::is_fundamental<T>::value, T> 
FromString(const std::string& str)
{
	std::stringstream ss(str);
	T t;
	ss >> t;
	return t;
}

/**Create a string from a non fundamental derived from cg::Object.
\param obj The thing to create a string from.*/
template<typename T>
std::enable_if_t<std::is_base_of<cg::Stringable,T>::value, std::string>
ToString(const T& obj)
{
	return obj.ToString();
}

/**Create an object from a string.
\param str The string to build from.
\return The object built.*/
template<typename T>
std::enable_if_t<std::is_base_of<cg::Unstringable, T>::value, T>
FromString(const std::string& str)
{
	T t;
	t.FromString(str);
	return t;
}

/******************************************containers with only two elements.*/
/**Convert a pair to a string.
\param p The object to convert.
\return The string.*/
template<
	template<typename, typename> class Pair,
	typename A,
	typename B
>
std::string ToString(const Pair<A, B>& p)
{
	std::stringstream ss;
	ss << "(" << ToString(p.first) << "," << ToString(p.second) << ")";
	return ss.str();
}
/***************************************************Container with allocator.*/

/**Convert a container with allocator to a string.
\param o The object to convert.
\return The string.*/
template<
	typename T,
	template<typename, typename> class List,
	template<typename> class Alloc
>
std::string ToString(const List<T, Alloc<T>>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e) << ",";
	}
	return ss.str();
}

/*******************************************Containers with alloc and compare*/
/**Convert a container with allocator and predicate to a string.
\param o The object to convert.
\return The string.*/
template<
	template<typename, typename, typename> class List,
	template<typename> class Comp,
	template<typename> class Alloc,
	typename T
>
std::string	ToString(const List<T, Comp<T>, Alloc<T>>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e) << ",";
	}
	return ss.str();
}
/************************************************************************Maps*/
/**Convert maps to a string.
\param o The object to convert.
\return The string.*/
template<
	template<typename, typename, typename, typename> class Map,
	typename Key,
	typename T,
	template<typename> class Comp,
	template<typename> class Alloc,
	template<typename, typename> class Pair
>
std::string ToString(
	const Map<Key, T, Comp<Key>, Alloc<Pair<const Key, T>>>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e);
	}
	return ss.str();
}
/************************************************Unordered sets and multisets*/
/**Convert unordered sets and multisets.
\param o The object to convert.
\return The string.*/
template<
	template<typename, typename, typename, typename> class USet,
	typename Key,
	template<typename> class Hasher,
	template<typename> class Pred,
	template<typename> class Alloc
>
std::string ToString(const USet<Key, Hasher<Key>, Pred<Key>, Alloc<Key>>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e) << ",";
	}
	return ss.str();
}
/***************************************************************Unordered Map*/
/**Convert a std::unorderd_map to a string.
\param o The object to convert.
\return The string.*/
template<
	template<typename, typename, typename, typename, typename> class Map,
	typename Key,
	typename T,
	template<typename> class Hasher,
	template<typename> class Comp,
	template<typename> class Alloc,
	template<typename, typename> class Pair
>
std::string ToString(
	const Map<Key, T, Hasher<Key>, Comp<Key>,
	Alloc<Pair<const Key, T>>>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e);
	}
	return ss.str();

}
/**************************************************************For std::array*/
/**Convert a std::array to a string.
\param o The array.
\return The string.*/
template<
	template<typename, std::size_t> class Array,
	typename T,
	std::size_t Size
>
std::string ToString(const Array<T, Size>& o)
{
	std::stringstream ss;
	for (const auto& e : o)
	{
		ss << ToString(e) << ",";
	}
	return ss.str();
}

/*****************************************************************************/


/**Helper for string together
\param str The string.
\return The combined string.*/
template<typename Str>
std::string StringTogetherHelper(Str&& str)
{
    return std::string(ToString(str));
}
/**Helper for string together
\param str The string.
\param args More strings.
\return The combined string.*/
template<typename...Args, typename Str>
std::string StringTogetherHelper(Str&& str, Args&&...args)
{
    return std::string(ToString(str)) +
        StringTogetherHelper(std::forward<Args>(args)...);
}
/**Helper for string together
\param args More strings.
\return The combined string.*/
template<typename...Args>
std::string StringTogether(Args&&...args)
{
	return StringTogetherHelper(std::forward<Args>(args)...);
}


/**Helper for string together
\param args More strings.
\return The combined string.*/
template<typename...Args>
std::string StringTogetherWithSeperator(Args&&...args)
{
	return StringTogetherWithSeperatorHelper(std::forward<Args>(args)...);
}
/**Helper for string together
\param str The string.
\return The combined string.*/
template<typename Str>
std::string StringTogetherWithSeperatorHelper(Str&& str)
{
	return std::string(ToString(str));
}
/**Helper for string together
\param str The string.
\param args More strings.
\return The combined string.*/
template<typename...Args, typename Str>
std::string StringTogetherWithSeperatorHelper(Str&& str, Args&&...args)
{
	return std::string(ToString(str)) + " " +
		StringTogetherWithSeperatorHelper(std::forward<Args>(args)...);
}


}
