#pragma once

template<typename T, typename I = int>
class CUtlMemory
{
public:
	T & operator[](I index)
	{
		return memory[index];
	}
protected:
	T* memory;
	int allocationcount;
	int growsize;
};

template<typename T, typename A = CUtlMemory<T>>
class CUtlVector
{
public:
	T& operator[](int index)
	{
		return memory[index];
	}
	int Count() const
	{
		return size;
	}
protected:
	A memory;
	int size;
	T* elements;
};
