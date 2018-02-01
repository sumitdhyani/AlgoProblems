// ForGenericTesting.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <map>
#include <set>
#include <thread>


struct less_equal
{
	bool operator()(int n1, int n2)
	{
		return (n1 <= n2);
	}
};

std::thread

int main(int argc, char* argv[])
{
	std::set<int> set;
	set.insert(3);
	set.insert(4);
	set.insert(5);
	//auto it = set.begin();
	//it++;
	for (auto it : set)
	{
		if (it > 4)
			break;
	}

	std::cout << it;

	return 0;
}

