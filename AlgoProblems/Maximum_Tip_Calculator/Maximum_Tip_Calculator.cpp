// Maximum_Tip_Calculator.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <hash_map>


struct funcParams
{
	int currentCustomerNo;
	int numTipsLeftForWaiter1;
	int numTipsLeftForWaiter2;

	funcParams(int currentCustomerNo, int numTipsLeftForWaiter1, int numTipsLeftForWaiter2)
	{
		this->currentCustomerNo = currentCustomerNo;
		this->numTipsLeftForWaiter1 = numTipsLeftForWaiter1;
		this->numTipsLeftForWaiter2 = numTipsLeftForWaiter2;
	}
};

bool operator<(const funcParams& params1, const funcParams& params2)
{
	if (params1.currentCustomerNo != params2.currentCustomerNo)
		return (params1.currentCustomerNo < params2.currentCustomerNo);
	else if (params1.numTipsLeftForWaiter1 != params2.numTipsLeftForWaiter1)
		return (params1.numTipsLeftForWaiter1 < params2.numTipsLeftForWaiter1);
	else if (params1.numTipsLeftForWaiter2 != params2.numTipsLeftForWaiter2)
		return (params1.numTipsLeftForWaiter2 < params2.numTipsLeftForWaiter2);
	else
		return false;
}


int calculateMaxTip( int numCustomers, 
					 int currentCustomerNo, 
					 const std::vector<int>& waiter1, 
					 int numTipsLeftForWaiter1, 
					 const std::vector<int>& waiter2, 
					 int numTipsLeftForWaiter2)
{
	if (currentCustomerNo == numCustomers)
		return 0;

	static std::hash_map<funcParams,int> memoized_map;
	funcParams temp(currentCustomerNo, numTipsLeftForWaiter1, numTipsLeftForWaiter2);
	auto it = memoized_map.find(temp);
	if (it != memoized_map.end())
		return it->second;

	int res = 0;
	if (0 == numTipsLeftForWaiter1)
	{ 
		for (int i = currentCustomerNo; i < numCustomers; i++)
			res += waiter2[i];
	}
	else if (0 == numTipsLeftForWaiter2)
	{
		for (int i = currentCustomerNo; i < numCustomers; i++)
			res += waiter1[i];
	}
	else
	{
		int sum1 = waiter1[currentCustomerNo] + calculateMaxTip(numCustomers,
																currentCustomerNo + 1,
																waiter1,
																numTipsLeftForWaiter1 - 1,
																waiter2,
																numTipsLeftForWaiter2
																);

		int sum2 = waiter2[currentCustomerNo] + calculateMaxTip(numCustomers,
																currentCustomerNo + 1,
																waiter1,
																numTipsLeftForWaiter1,
																waiter2,
																numTipsLeftForWaiter2 - 1
																);

		res = (sum1 > sum2) ? sum1 : sum2;
	}

	memoized_map[funcParams(currentCustomerNo, numTipsLeftForWaiter1, numTipsLeftForWaiter2)] = res;
	return res;
}


template <typename In_type, typename Out_type>
void testfindNumAnagrams(In_type& in, Out_type& out)
{
	int numTests;
	int n, x, y;
	in >> numTests;
	std::vector<int> waiter1;
	std::vector<int> waiter2;
	while (numTests-- > 0)
	{
		waiter1.clear();
		waiter2.clear();
		in >> n >> x >> y;
		int w1i;
		int w2i;
		for (int i = 0; i < n; i++)
		{
			in >> w1i;
			waiter1.push_back(w1i);
		}

		for (int i = 0; i < n; i++)
		{
			in >> w2i;
			waiter2.push_back(w2i);
		}

		out << calculateMaxTip(n, 0, waiter1, x, waiter2, y) << std::endl;
	}
}


int main(int argc, char* argv[])
{
	std::ifstream in("Maximum_Tip_Calculator.txt");
	testfindNumAnagrams<std::ifstream, std::ostream>(in, std::cout);

	return 0;
}

