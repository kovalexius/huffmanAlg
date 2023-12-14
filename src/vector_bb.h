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

		vector_bb(std::initializer_list<bool> list);

		const char* data();

		// Return number of bits
		size_t numberBits() const;

		// Return size in bytes
		size_t size();

		
#if _MSC_VER <= 1800
		void push_back(const bool _bit) throw();
#else
		void push_back(const bool _bit) noexcept(true);
#endif

#if _MSC_VER <= 1800
		bool pop_back() throw(std::out_of_range);
#else
		bool pop_back() noexcept(false);
#endif

		auto back();

		bool operator < (const vector_bb& _other);
		bool operator != (const vector_bb& _other);
		bool operator == (const vector_bb& _other);
		
		//char& operator[](size_t pos);
		bool operator[](size_t pos) const;

		// Распечатать нолики и единички
		operator std::string() const;
		void operator += (const vector_bb& _other);
    private:
		// Количество заполненных бит в текущем байте
        int m_count;
		// тут хранится все побайтово
        std::vector<char> m_data;
    };
}
////////////////////////////////////////////////

#endif