//https://practice.geeksforgeeks.org/problems/maximum-tip-calculator/0/?ref=self
//Rahul and Ankit are the only two waiters in Royal Restaurant.Today, the restaurant received N orders.The amount of tips may differ when handled by different waiters, if Rahul takes the ith order, he would be tipped Ai rupees and if Ankit takes this order, the tip would be Bi rupees.
//In order to maximize the total tip value they decided to distribute the order among themselves.One order will be handled by one person only.Also, due to time constraints Rahul cannot take more than X orders and Ankit cannot take more than Y orders.It is guaranteed that X + Y is greater than or equal to N, which means that all the orders can be handled by either Rahul or Ankit.Find out the maximum possible amount of total tip money after processing all the orders.
//
//
//Input:
//
//•    The first line contains one integer, number of test cases.
//•    The second line contains three integers N, X, Y.
//•    The third line contains N integers.The ith integer represents Ai.
//•    The fourth line contains N integers.The ith integer represents Bi.
//
//
//
//Output :
//	   Print a single integer representing the maximum tip money they would receive.
//
//
//   Constraints :
//			   1 ≤ N ≤ 105
//			   1 ≤ X, Y ≤ N; X + Y ≥ N
//			   1 ≤ Ai, Bi ≤ 104
//
//
//
//		   Example:
//
//Input :
//
//	2
//	5 3 3
//	1 2 3 4 5
//	5 4 3 2 1
//	8 4 4
//	1 4 3 2 7 5 9 6
//	1 2 3 6 5 4 9 8
//
//
//
//Output:
//
//	  21
//		  43
#include <algorithm>
//#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <set>
#include "Common.h"



typedef std::vector<UINT> Vector;
typedef std::vector<size_t> IndexVector;
typedef std::set<UINT> Set;

UINT calculateMaxTip(const Vector& w1, const Vector& w2, UINT capacity_w1, UINT capacity_w2)
{
	UINT res = 0;
	UINT numCustomers = w1.size();
	Set excesses_w1;
	Set excesses_w2;
	for (size_t i = 0; i < numCustomers; i++)
	{
		if (w1[i] > w2[i])
		{
			res += w1[i];
			excesses_w1.insert(w1[i]-w2[i]);
		}
		else
		{
			res += w2[i];
			excesses_w2.insert(w2[i] - w1[i]);
		}
	}

	const Set* excess = NULL;
	UINT numExcesses = 0;
	if (excesses_w1.size() > capacity_w1)
	{
		excess = &excesses_w1;
		numExcesses = excesses_w1.size() - capacity_w1;
	}
	else if (excesses_w2.size() > capacity_w2)
	{
		excess = &excesses_w2;
		numExcesses = excesses_w2.size() - capacity_w2;
	}

	if (NULL != excess)
	{
		auto it = excess->begin();
		for (UINT i = 0; i < numExcesses; i++,it++)
			res -= *it;
	}

	return res;
}


template <typename In_Type, typename Out_Type>
void testcalculateMaxTip(In_Type& in, Out_Type& out)
{
	UINT numTestCases, n, x, y;
	in >> numTestCases;

	while (numTestCases-- > 0)
	{
		Vector w1, w2;
		in >> n >> x >> y;
		UINT wi;
		for (UINT i = 0; i < n; i++)
		{
			in >> wi;
			w1.push_back(wi);
		}

		for (UINT i = 0; i < n; i++)
		{
			in >> wi;
			w2.push_back(wi);
		}
		out << calculateMaxTip(w1, w2, x, y) << std::endl;
	}
}


int main(int argc, char* argv[])
{
	std::ifstream in("Maximum_Tip_Calculator.txt");
	testcalculateMaxTip<std::ifstream, std::ostream>(in, std::cout);
	return 0;
}

