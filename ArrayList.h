#include "ArrayList_decl.h"

template<typename T> ArrayList<T>::ArrayList()
{

}

template <typename T> ArrayList<T>::ArrayList(const ArrayList<T>& copy) noexcept // copy should not own the pointer of the object it was created from
{
	for (uint32_t _iter = 0; _iter < copy.size; _iter++)
		add(copy.get(_iter));
}

template <typename T> ArrayList<T>::ArrayList(ArrayList<T>&& move) noexcept // old object should not own the pointer anymore
{
	size = move.size;
	capacity = move.capacity;
	internal_data = move->internal_data;
	move->internal_data = nullptr;
}

template <typename T> ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& copy) noexcept
{
	for (uint32_t _iter = 0; _iter < copy.size; _iter++)
		add(copy.get(_iter));
	return *this;
}

template <typename T> ArrayList<T>& ArrayList<T>::operator=(ArrayList<T>&& move) noexcept
{
	size = move.size;
	capacity = move.capacity;
	internal_data = move->internal_data;
	move.internal_data = nullptr;
	return *this;
}

template <typename T> ArrayList<T>::ArrayList(std::initializer_list<T> initializer_list)
{
	for (T elem : initializer_list)
		add(elem);
}

template <typename T> ArrayList<T>::~ArrayList()
{
	free_internal_data();
}

template <typename T> uint32_t ArrayList<T>::getSize() const
{
	return size;
}

template <typename T> uint32_t ArrayList<T>::getCapacity() const
{
	return capacity;
}

template <typename T> void ArrayList<T>::add(T element)
{
	if (size == capacity)
	{
		if (capacity == NULL)
			capacity += 2;
		else
			capacity *= 2;

		T* data_enlarged = new T[capacity];

		for (uint32_t i = 0; i < size; i++)
		{
			data_enlarged[i] = T(internal_data[i]);
		}

		free_internal_data();
		internal_data = data_enlarged;
	}
	if (size < capacity)
	{
		internal_data[size] = T(element);
		size++;
	}
}

template <typename T> void ArrayList<T>::emplace(T element)
{
	if (size > 0)
	{
		add(end());
		for (uint32_t i = size - 2; i > 0; i--)
		{
			set(i, get(i - 1));
		}
		set(0, element);
	}
	else
		add(element);
}

template <typename T> void ArrayList<T>::remove(uint32_t index)
{
	internal_data[index].~T();
	for (uint32_t i = index + 1; i < size; i++)
	{
		internal_data[i - 1] = T(internal_data[i]);
	}
	size--;
}

template <typename T> T& ArrayList<T>::get(uint32_t index) const
{
	return internal_data[index];
}

template <typename T> T& ArrayList<T>::operator[](uint32_t index)
{
	return get(index);
}

template <typename T> void ArrayList<T>::set(uint32_t index, T value)
{
	internal_data[index] = T(value);
}

template <typename T> T& ArrayList<T>::begin() const
{
	return get(0);
}

template <typename T> T& ArrayList<T>::end() const
{
	return get(size - 1);
}


template <typename T> void ArrayList<T>::free_internal_data()
{
	if (internal_data != nullptr)
	{
		delete[] internal_data;
		internal_data = nullptr;
	}
}

template <typename T> std::ostream& operator<<(std::ostream& os, const ArrayList<T>& arraylist)
{
	os << "[ ";
	for (uint32_t i = 0; i < arraylist.getSize(); i++) {
		os << arraylist.get(i);
		if (i < arraylist.getSize() - 1)
			os << ", ";
		else
			os << " ";
	}
	os << "]";
	return os;
}

template <typename T> void print(const T& arg) 
{
	std::cout << arg;
}
template <typename T> void println(const T& arg)
{
	std::cout << arg << "\n";
}