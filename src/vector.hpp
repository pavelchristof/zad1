#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstring>
#include <type_traits>

/**
 * A very basic vector.
 * @tparam T must be a trivial type.
 */
template <typename T>
class Vector
{
public:
	static_assert(std::is_trivial<T>::value, "T must be a trivial type.");

	Vector() = default;
	Vector(const T&) = delete;
	Vector(T &&) = delete;
	Vector& operator = (const T&) = delete;
	Vector& operator = (T &&) = delete;

	~Vector()
	{
		delete[] m_data;
	}

	T& operator [](size_t i)
	{
		return m_data[i];
	}

	const T& operator [](size_t i) const
	{
		return m_data[i];
	}

	T& back()
	{
		return m_data[m_size - 1];
	}

	const T& back() const
	{
		return m_data[m_size - 1];
	}

	size_t size() const
	{
		return m_size;
	}

	void push_back(const T &value)
	{
		if (m_size == m_alloc) {
			size_t newAlloc = 2 * m_size + 1;
			T *newData = new T[newAlloc];
			std::memcpy(newData, m_data, m_size * sizeof(T));
			delete[] m_data;
			m_data = newData;
			m_alloc = newAlloc;
		}

		m_data[m_size++] = value;
	}

private:
	size_t m_size = 0;
	size_t m_alloc = 0;
	T *m_data = nullptr;
};

#endif // VECTOR_HPP
