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

#include <deque>

#include "Filter.hpp"
#include "Logger.hpp"

namespace cg {

/**A Group of filters.*/
class FilterGroup : public cg::Filter
{
public:
	/**Determine if the size will change when applying all the filters.*/
	virtual bool SizeChanges() const
	{
		for (std::size_t i = 0; i < m_filters.size(); ++i)
			if (m_filters[i]->SizeChanges())
				return true;

		return false;
	}
	/**Virtual DTOR, clean up all the filters.*/
	virtual ~FilterGroup()
	{
		for (std::size_t i = 0; i < m_filters.size(); ++i)
			cg::Delete(__FUNCSTR__,m_filters[i]);
	}
	/**Add a filter.
	\param filter The filter to add.*/
	inline void AddFilter(cg::Filter* filter)
	{
		m_filters.push_back(filter);
	}
	/**Create empty.*/
	FilterGroup() {};
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	inline virtual void Transform(char* data, std::size_t size)
	{
		for (std::size_t i = 0; i < m_filters.size(); ++i)
			m_filters[i]->Transform(data, size);
	}
	/**Transform data using the transform copy method.
	\param data The data place.
	\param size The data size.*/
	inline virtual std::vector<char> TransformCopy(const char* data, 
		std::size_t size)
	{
		auto av = std::vector<char>(data,data+size);
		for (std::size_t i = 0; i < m_filters.size(); ++i)
			av = m_filters[i]->TransformCopy(av.data(), av.size());
		return av;
	}
private:
	/**A list of filters with bool. If second is true, the filter is expected
	to change the size of the data.*/
	std::deque<cg::Filter*> m_filters;
};

}