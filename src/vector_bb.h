#ifndef __DATA_TYPES_H
#define __DATA_TYPES_H

#include <iostream>
#include <vector>
#include <memory>
#include <utility>
#include <string>
#include <initializer_list>
#include <memory.h>


///////////Pack bits to bytes vector////////////
namespace containers
{
    struct vector_bb
    {
        vector_bb() : m_count(0)
        {}

		vector_bb(std::initializer_list<bool> list) : m_count(0)
        {
			//auto numArgs = list.size();
			//auto lenElem = sizeof(decltype(m_data)::value_type) * 8;
			//m_count = numArgs % lenElem;

            for (auto &item : list)
                push_back(item);
        }

        const char* data()
        {
            return m_data.data();
        }

        size_t numberBits()
        {
			return m_data.size() * sizeof(decltype(m_data)::value_type) + m_count;
        }

		size_t numberElements()
		{

		}

		void push_back(const bool _bit) 
#ifdef _MSC_VER <= 1800
			throw()
#else
			noexcept(true)
#endif
        {
            if (m_count == 0)
                m_data.push_back(0);
            if (_bit)
                m_data.back() = m_data.back() | 0x80 >> m_count;   // Взводим конкретный бит
			else
				m_data.back() = m_data.back() & ~(0x80 >> m_count);  // Обнуляем конкретный бит
			auto bitLen = sizeof(decltype(m_data)::value_type) * 8;
			if ((++m_count) > sizeof(decltype(m_data)::value_type) * 8 - 1)
                m_count = 0;

			std::vector<uint32_t>::value_type someVal;
        }

        bool pop_back()
#ifdef _MSC_VER <= 1800
			throw(std::out_of_range)
#else
			noexcept(false)
#endif
        {
            if (m_data.size() == 0)
                throw(std::out_of_range(std::string("try to pop from empty vector_bb")));
            if ((--m_count) < 0)
				m_count = sizeof(decltype(m_data)::value_type) - 1;
			bool result = m_data.back() & (0x80 >> m_count);
            //m_data.back() = m_data.back() & ~(0x80 >> m_count);
            if (m_count == 0)
                m_data.pop_back();
			return result;
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
		// Количество заполненных бит в текущем байте
        int m_count;
		// тут хранится все побайтово
        std::vector<char> m_data;
    };
}
////////////////////////////////////////////////

#endif