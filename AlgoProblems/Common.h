#pragma once

#include <algorithm>
#include <xfunctional>
#include <vector>
#include <iostream>	
#include <fstream>

#if !defined(UINT)
typedef unsigned int UINT;
#endif

#if !defined(INT)
typedef long long int INT;
#endif


template <class T, class comp = std::less<T> >
class BinaryHeap
{
	std::vector<T> _arr;
	UINT _size;

	UINT getParentIndex(UINT index)
	{
		if (0 == index)
			return 0;
		else
			return ((index - 1) / 2);
	}

	UINT getLeftChildIndex(UINT index)
	{
		return (index * 2 + 1);
	}

	UINT getRightChildIndex(UINT index)
	{
		return (index * 2 + 2);
	}

public:
	BinaryHeap()
	{
		_size = 0;
	}

	~BinaryHeap(){};

	bool empty()
	{
		return (_size == 0);
	}

	void insert(T val)
	{
		if (_arr.size() >= ++_size)
			_arr[_size - 1] = val;
		else
			_arr.push_back(val);

		UINT currIndex = _size - 1;
		UINT  parentIndex = getParentIndex(currIndex);
		while (comp()(_arr[currIndex], _arr[parentIndex]))
		{
			std::swap(_arr[currIndex], _arr[parentIndex]);
			currIndex = parentIndex;
			parentIndex = getParentIndex(currIndex);
		}
	}

	void removeTop()
	{
		if (empty())
			return;

		_arr[0] = _arr[_size - 1];
		--_size;

		UINT currIndex = 0;

		bool carryOn = true;
		while (carryOn)
		{
			UINT left = (getLeftChildIndex(currIndex) < _size) ? getLeftChildIndex(currIndex) : currIndex;
			UINT right = (getRightChildIndex(currIndex) < _size) ? getRightChildIndex(currIndex) : currIndex;

			UINT indexToCompare = comp()(_arr[left], _arr[right]) ? left : right;
			if (comp()(_arr[indexToCompare], _arr[currIndex]))
			{
				std::swap(_arr[indexToCompare], _arr[currIndex]);
				currIndex = indexToCompare;
			}
			else
				carryOn = false;
		}
	}

	const T& getExtreme() const
	{
		if (_size > 0)
			return _arr[0];
		else
			throw std::runtime_error("Heap is empty and top is being asked");
	}

	UINT getSize() const
	{
		return _size;
	}
};


typedef BinaryHeap<UINT, std::less<UINT>> MinHeap;
typedef BinaryHeap<UINT, std::greater<UINT>> MaxHeap;