// PositiveNegativePairs.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_set>
#include "Common.h"

typedef std::vector<INT> Vector;
typedef std::set<INT> Set;
typedef std::unordered_set<INT> HashSet;

void getPositiveNegativePairs(const Vector& arr, Set& out)
{
	HashSet hashSet;
	for (UINT i = 0; i < arr.size(); i++)
	{
		auto it = hashSet.find(-arr[i]);
		if (it != hashSet.end())
		{
			out.insert(abs(arr[i]));
			hashSet.erase(it);
		}
		else
			hashSet.insert(arr[i]);
	}
}

template <typename In_Type, typename Out_Type>
void testgetPositiveNegativePairs(In_Type& in, Out_Type& out)
{
	UINT numTestCases, numElements;
	INT element;
	in >> numTestCases;

	while (numTestCases-- > 0)
	{
		Vector input;
		Set output;
		in >> numElements;
		for (UINT i = 0; i < numElements; i++)
		{
			in >> element;
			input.push_back(element);
		}

		getPositiveNegativePairs(input, output);
		if (0 == output.size())
			out << "0";
		else
		{
			out << "\n";
			for (auto it = output.begin(); it != output.end(); it++)
				out << *it<< " "<<-(*it)<<	" ";
		}
	}
}

int main(int argc, char* argv[])
{
	std::ifstream in("PositiveNegativePairs.txt");
	testgetPositiveNegativePairs<std::ifstream, std::ostream>(in, std::cout);
	return 0;
}

