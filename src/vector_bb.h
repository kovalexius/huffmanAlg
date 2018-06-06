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
            for (auto &item : list)
                push_back(item);
        }

        const char* data()
        {
            return m_data.data();
        }

		// Return number of bits
        size_t numberBits()
        {
			if (m_data.size() > 0)
			{
				return (m_data.size() - 1) * sizeof(decltype(m_data)::value_type) * 8 + m_count;
			}
			return m_count;
        }

		// Return size in bytes
		size_t size()
		{
			return m_data.size() * sizeof(decltype(m_data)::value_type);
		}

		void push_back(const bool _bit) 
#ifdef _MSC_VER <= 1800
			throw()
#else
			noexcept(true)
#endif
        {
			auto lenBitElem = sizeof(decltype(m_data)::value_type) * 8;
			if (m_count > lenBitElem - 1)
				m_count = 0;
            if (m_count == 0)
                m_data.push_back(0);
            if (_bit)
                m_data.back() = m_data.back() | 0x80 >> m_count;   // Взводим конкретный бит
			else
				m_data.back() = m_data.back() & ~(0x80 >> m_count);  // Обнуляем конкретный бит
			++m_count;
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
			--m_count;
			bool result = m_data.back() & (0x80 >> m_count);
            m_data.back() = m_data.back() & ~(0x80 >> m_count); // Обязательно обнуляем уже извлеченный бит, 
                                                                // т.к. с точки зрения std::vector данная единица информация присутствует в контейнере и 
                                                                // чтобы правильно работали операторы сравнения договариваемся что неиспользуемые биты в байте должны быть обнулены
			if (m_count == 0)
			{
				m_count = sizeof(decltype(m_data)::value_type) * 8;
				m_data.pop_back();
			}
			
			return result;
        }

        auto back()
        {
            return m_data.back();
        }

        bool operator < (const vector_bb &other) 
        {
            if (m_count == other.m_count)
                return (m_data < other.m_data);
            return (m_count < other.m_count);
        }

        bool operator != (const vector_bb &other) 
		{
            return (m_count != other.m_count) || (m_data != other.m_data);
        }

        bool operator == (const vector_bb &other) 
		{
            return (m_count == other.m_count) && (m_data == other.m_data);
        }

		char& operator[](size_t pos) 
		{
			return m_data[pos];
        }

		// Распечатать нолики и единички
		operator std::string() const
		{
			if (m_data.size() < 1)
				return std::string();

			std::string result;
			int numFullBytes = m_data.size() - 1;
			for (int i = 0; i < numFullBytes; i++)
			{
				for (int j = 0; j < sizeof(decltype(m_data)::value_type) * 8; j++)
				{
					if (m_data[i] & (0x80 >> j))
						result += "1";
					else
						result += "0";
				}
			}

			for (int j = 0; j < m_count; j++)
			{
				if (m_data.back() & (0x80 >> j))
					result += "1";
				else
					result += "0";
			}

			return result;
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