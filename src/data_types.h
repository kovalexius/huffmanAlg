#ifndef __DATA_TYPES_H
#define __DATA_TYPES_H

#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <initializer_list>
#include <memory.h>

namespace rtvdt   // runtime variadic data types
{
    struct Element
    {
        Element(const char * ptr, const int len) : m_len(len),
                                                   m_data(ptr)
        {}

        bool operator < (const Element & other) const 
        {
            if (m_len == other.m_len)
                return (memcmp(m_data, other.m_data, m_len) < 0) ? true : false;
            else
                return (m_len < other.m_len);
        }

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
    bool operator == (const Element &e1, const Element &e2)
    {
        return (e1.m_len == e2.m_len) ? (memcmp(e1.m_data, e2.m_data, e1.m_len) == 0) : false;
    }

    // Equal function
    bool isEquals(const Element *e1, const Element *e2)
    {
        return (e1->m_len == e2->m_len) ? (memcmp(e1->m_data, e2->m_data, e1->m_len) == 0) : false;
    }

    // Equal Comparator
    struct EqualElement
    {
        bool operator () (const Element &e1, const Element &e2) const
        {
            return (e1.m_len == e2.m_len) ? (memcmp(e1.m_data, e2.m_data, e1.m_len) == 0) : false;
        }
    };

    // out manipulator
    std::ostream & operator << (std::ostream &os, const Element & a)
    {
        os << std::string(a.m_data, a.m_len);
        return os;
    }

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
struct Element
{
    bool operator==(const Element & other) {
        return (memcmp(m_data, other.m_data, Len) == 0);
    }
private:
    const char m_data[Len];
};

template<>
struct Element<1>
{
    bool operator==(const Element & other) {
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

///////////Pack bits to bytes vector////////////
namespace containers
{
    struct vector_bb
    {
        vector_bb() : m_count(0)
        {}

        vector_bb(std::initializer_list<bool> list) : m_count(0)
        {
            for (auto &item : list)
                push_back(item);
        }

        const char* data()
        {
            return m_data.data();
        }

        size_t size()
        {
            return m_data.size();
        }

        void push_back(const bool bit)
        {
            if (m_count == 0)
                m_data.push_back(0);
            if (bit)
                m_data.back() = m_data.back() | 0x80 >> m_count;
            if ((++m_count) > 7)
                m_count = 0;
        }

        void pop_back()
        {
            if (m_data.size() == 0)
                return;
            if ((--m_count) < 0)
                m_count = 7;
            m_data.back() = m_data.back() & ~(0x80 >> m_count);
            if (m_count == 0)
                m_data.pop_back();
        }

        bool operator < (const vector_bb &other) 
        {
            if (m_count == other.m_count)
                return (m_data < other.m_data);
            return (m_count < other.m_count);
        }

        bool operator != (const vector_bb &other) {
            return (m_count != other.m_count) || (m_data != other.m_data);
        }

        bool operator == (const vector_bb &other) {
            return (m_count == other.m_count) && (m_data == other.m_data);
        }

		char& operator[](size_t pos) {
			return m_data[pos];
        }
    private:
        int m_count;
        std::vector<char> m_data;
    };
}
////////////////////////////////////////////////

#endif