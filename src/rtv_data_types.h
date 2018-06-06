#ifndef __RT_VARIADIC_DATA_TYPES__H
#define __RT_VARIADIC_DATA_TYPES__H

#include <string>

namespace rtvdt   // runtime variadic data types
{
	struct Element
	{
		Element(const char * ptr, const int len);

		bool operator < (const Element & other) const;

		operator std::string() const;

		friend bool operator == (const Element&, const Element&);
		friend bool isEquals(const Element *e1, const Element *e2);
		friend struct EqualElement;
		friend struct KeyHash;
		friend std::ostream & operator << (std::ostream &os, const Element & a);
		//private:
		int m_len;
		const char *m_data;
	};

	// Equal operator
	bool operator == (const Element &e1, const Element &e2);

	// Equal function
	bool isEquals(const Element *e1, const Element *e2);

	// Equal Comparator
	struct EqualElement
	{
		bool operator () (const Element &e1, const Element &e2) const
		{
			return (e1.m_len == e2.m_len) ? (memcmp(e1.m_data, e2.m_data, e1.m_len) == 0) : false;
		}
	};

	// out manipulator
	std::ostream & operator << (std::ostream &os, const Element & a);

	struct KeyHash
	{
		std::size_t operator()(const Element& k) const
		{
			return std::hash<int>()(k.m_len) ^
				std::hash<std::string>()(std::string(k.m_data, k.m_len));
		}
	};
}   // end of rtvdt



/////////// template tools //////////
#pragma pack(push, 1)
template<int Len>
struct TElement
{
	bool operator==(const TElement & other) {
		return (memcmp(m_data, other.m_data, Len) == 0);
	}
private:
	const char m_data[Len];
};

template<>
struct TElement<1>
{
	bool operator==(const TElement & other) {
		return (m_data == other.m_data);
	}
private:
	const char m_data;
};
#pragma pack(pop)
///////////////////////////////////////




//////////type for binary tree///////////
struct treeNode
{
	std::unique_ptr<rtvdt::Element> elem;
	std::unique_ptr<treeNode> left;
	std::unique_ptr<treeNode> right;
};
/////////////////////////////////////////


#endif