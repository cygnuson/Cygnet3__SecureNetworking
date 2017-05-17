#pragma once

#include <istream>
#include <sstream>
#include <memory>
#include <cctype>

namespace cg {

/****************************VECTOR PUSH FRONT****************************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushFrontHelper(C<Type1, std::allocator<Type1>>& intake,
		const C<Type1, std::allocator<Type1>>& toInsert,
		const C<T, std::allocator<T>>&...vecs)
{
	intake.insert(intake.begin(), toInsert.begin(), toInsert.end());
	PushFrontHelper(intake, vecs...);
}
template<template<typename, typename> typename C,
	typename T>
	void PushFrontHelper(C<T, std::allocator<T>>& intake,
		const C<T, std::allocator<T>>& toInsert)
{
	intake.insert(intake.begin(), toInsert.begin(), toInsert.end());
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushFront(C<Type1, std::allocator<Type1>>& intake,
		const C<T, std::allocator<T>>&...vecs)
{
	PushFrontHelper(intake, vecs...);
}
/*******************************OBJECT PUSH FRONT*************************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushFrontHelper(C<Type1, std::allocator<Type1>>& intake,
		const Type1& toInsert,
		const T&...nums)
{
	intake.insert(intake.begin(), toInsert);
	PushFrontHelper(intake, nums...);
}
template<template<typename, typename> typename C,
	typename T>
	void PushFrontHelper(C<T, std::allocator<T>>& intake,
		const T& toInsert)
{
	intake.insert(intake.begin(), toInsert);
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushFront(C<Type1, std::allocator<Type1>>& intake,
		const T&...nums)
{
	PushFrontHelper(intake, nums...);
}
/**********************************VECTOR PUSH BACK***********************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushBackHelper(C<Type1, std::allocator<Type1>>& intake,
		const C<Type1, std::allocator<Type1>>& toInsert,
		const C<T, std::allocator<T>>&...vecs)
{
	intake.insert(intake.end(), toInsert.begin(), toInsert.end());
	PushBackHelper(intake, vecs...);
}
template<template<typename, typename> typename C,
	typename T>
	void PushBackHelper(C<T, std::allocator<T>>& intake,
		const C<T, std::allocator<T>>& toInsert)
{
	intake.insert(intake.end(), toInsert.begin(), toInsert.end());
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushBack(C<Type1, std::allocator<Type1>>& intake,
		const C<T, std::allocator<T>>&...vecs)
{
	PushBackHelper(intake, vecs...);
}
/****************************************OBJECT PUSH BACK*****************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushBackHelper(C<Type1, std::allocator<Type1>>& intake,
		const Type1& toInsert,
		const T&...nums)
{
	intake.insert(intake.end(), toInsert);
	PushBackHelper(intake, nums...);
}
template<template<typename, typename> typename C,
	typename T>
	void PushBackHelper(C<T, std::allocator<T>>& intake,
		const T& toInsert)
{
	intake.insert(intake.end(), toInsert);
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void PushBack(C<Type1, std::allocator<Type1>>& intake,
		const T&...nums)
{
	PushBackHelper(intake, nums...);
}
/**********************************************VECTOR INSERT**************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void InsertHelper(C<Type1, std::allocator<Type1>>& intake,
		std::size_t index,
		const C<Type1, std::allocator<Type1>>& toInsert,
		const C<T, std::allocator<T>>&...vecs)
{
	intake.insert(intake.begin() + index, toInsert.begin(), toInsert.end());
	InsertHelper(intake, index, vecs...);
}
template<template<typename, typename> typename C,
	typename T>
	void InsertHelper(C<T, std::allocator<T>>& intake,
		std::size_t index,
		const C<T, std::allocator<T>>& toInsert)
{
	intake.insert(intake.begin() + index, toInsert.begin(), toInsert.end());
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void Insert(C<Type1, std::allocator<Type1>>& intake,
		std::size_t index,
		const C<T, std::allocator<T>>&...vecs)
{
	InsertHelper(intake, index, vecs...);
}
/************************************OBJECT INSERT************************************************/
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void InsertHelper(C<Type1, std::allocator<Type1>>& intake,
		std::size_t index,
		const Type1& toInsert,
		const T&...nums)
{
	intake.insert(intake.begin() + index, toInsert);
	InsertHelper(intake, index, nums...);
}
template<template<typename, typename> typename C,
	typename T>
	void InsertHelper(C<T, std::allocator<T>>& intake,
		std::size_t index,
		const T& toInsert)
{
	intake.insert(intake.begin() + index, toInsert);
}
template<template<typename, typename> typename C,
	typename Type1, typename...T>
	void Insert(C<Type1, std::allocator<Type1>>& intake,
		std::size_t index,
		const T&...nums)
{
	InsertHelper(intake, index, nums...);
}
/******************************************************************************Equality assist****/
/**Check if two lists are equal.
\param lhs The first list.
\param rhs The second list.
\return True if boths list all have elements that operator!= return false.*/
template<
	typename T,
	template<typename, typename> class List,
	template<typename> class Alloc
>
bool Equals(const List<T, Alloc<T>>& lhs,
	const List<T, Alloc<T>>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;
	auto it = lhs.begin();
	auto end = lhs.end();
	auto itR = rhs.begin();
	auto endR = rhs.end();
	for (; it != end; ++it, ++itR)
	{
		if (*it != *itR)
			return false;
	}
	return true;
}
/**Check if two lists are equal.  Each elements will be dereferenced one 
additional time.
\param lhs The first list.
\param rhs The second list.
\return True if boths list all have elements that operator!= return false.*/
template<
	typename T,
	template<typename, typename> class List,
	template<typename> class Alloc
>
bool Equals_Deref(const List<T, Alloc<T>>& lhs,
	const List<T, Alloc<T>>& rhs)
{
	if (lhs.size() != rhs.size())
		return false;
	auto it = lhs.begin();
	auto end = lhs.end();
	auto itR = rhs.begin();
	auto endR = rhs.end();
	for (; it != end; ++it, ++itR)
	{
		if (**it != **itR)
			return false;
	}
	return true;
}
/**Check if two pairs are equal.
\param lhs The first pair.
\param rhs The second pair.
\return True if both pairs are equal.*/
template<
	template<typename, typename> class Pair,
	typename A,
	typename B
>
bool Equals(const Pair<A, B>& lhs,
	const Pair<A, B>& rhs)
{
	return lhs.first == rhs.first && lhs.second == rhs.second;
}

}
