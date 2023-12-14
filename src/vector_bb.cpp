#include "vector_bb.h"

namespace containers
{
	vector_bb::vector_bb(std::initializer_list<bool> list) : m_count(0)
	{
		for (auto &item : list)
			push_back(item);
	}

	const char* vector_bb::data()
	{
		return m_data.data();
	}

	// Return number of bits
	size_t vector_bb::numberBits() const
	{
		if (m_data.size() > 0)
		{
			return (m_data.size() - 1) * sizeof(decltype(m_data)::value_type) * 8 + m_count;
		}
		return m_count;
	}

	// Return size in bytes
	size_t vector_bb::size()
	{
		return m_data.size() * sizeof(decltype(m_data)::value_type);
	}

#if _MSC_VER <= 1800
	void vector_bb::push_back(const bool _bit) throw()
#else
	void vector_bb::push_back(const bool _bit) noexcept(true)
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

	
#if _MSC_VER <= 1800
	bool vector_bb::pop_back() throw(std::out_of_range)
#else
	bool vector_bb::pop_back() noexcept(false)
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

	auto vector_bb::back()
	{
		return m_data.back();
	}

	bool vector_bb::operator < (const vector_bb &other)
	{
		if (m_count == other.m_count)
			return (m_data < other.m_data);
		return (m_count < other.m_count);
	}

	bool vector_bb::operator != (const vector_bb &other)
	{
		return (m_count != other.m_count) || (m_data != other.m_data);
	}

	bool vector_bb::operator == (const vector_bb &other)
	{
		return (m_count == other.m_count) && (m_data == other.m_data);
	}

	//char& vector_bb::operator[](size_t pos)
	//{
	//	return m_data[pos];
	//}

	bool vector_bb::operator[](size_t pos) const
	{
		auto numByte = pos / (sizeof(decltype(m_data)::value_type) * 8);
		auto numBit = pos % (sizeof(decltype(m_data)::value_type) * 8);
		if (numByte + 1 > m_data.size())
			throw(std::out_of_range(std::string("try to read out of bytes vector_bb")));
		if(numBit > m_count)
			throw(std::out_of_range(std::string("try to read out of bits vector_bb")));
		return m_data[numByte] & (0x80 >> numBit);
	}

	// Распечатать нолики и единички
	vector_bb::operator std::string() const
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

	void vector_bb::operator += (const vector_bb& _other)
	{
		auto numbits = _other.numberBits();
		for (auto i = 0; i < numbits; i++)
		{
			push_back(_other[i]);
		}
	}
}