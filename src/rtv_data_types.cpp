#include "rtv_data_types.h"

#include <cstring>

namespace rtvdt   // runtime variadic data types
{
	Element::Element(const char * ptr, const int len) : m_len(len),
		m_data(ptr)
	{}

	bool Element::operator < (const Element & other) const
	{
		if (m_len == other.m_len)
			return (memcmp(m_data, other.m_data, m_len) < 0) ? true : false;
		else
			return (m_len < other.m_len);
	}

	Element::operator std::string() const
	{
		return std::string(m_data, m_len);
	}

	// Equal operator
	bool operator == (const Element &e1, const Element &e2)
	{
		return (e1.m_len == e2.m_len) ? (memcmp(e1.m_data, e2.m_data, e1.m_len) == 0) : false;
	}

	// Equal function
	bool isEquals(const Element *e1, const Element *e2)
	{
		return (e1->m_len == e2->m_len) ? (memcmp(e1->m_data, e2->m_data, e1->m_len) == 0) : false;
	}

	// out manipulator
	std::ostream & operator << (std::ostream &os, const Element & a)
	{
		os << std::string(a.m_data, a.m_len);
		return os;
	}
}