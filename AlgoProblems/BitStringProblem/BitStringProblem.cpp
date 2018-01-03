// https://practice.geeksforgeeks.org/problems/find-k-th-character-in-string/0
//Given a decimal number m.Convert it in binary string and apply n iterations, in each iteration 0 becomes 01 and 1 becomes 10. Find kth character in the string after nth iteration.
//
//Input:
//The first line consists of an integer T i.e number of test cases.The first and only line of each test case consists of three integers m, k, n.
//
//Output:
//Print the required output.
//
//Constraints :
//			1 <= T <= 100
//			1 <= m <= 50
//			1 <= n <= 10
//			0 <= k <= | Length of final string |
//
//		Example :
//			Input :
//				  2
//				  5 5 3
//				  11 6 4
//
//			  Output :
//					 1
//					 1

#include "Common.h"

UINT getBitLengthOfNumber(UINT n)
{
	if (0 == n)
		return 1;

	UINT numBits = 0;
	while (0 != n)
	{
		n /= 2;
		numBits++;
	}

	return numBits;
}

UINT getKthStringAfterTransformation(UINT m, UINT k, UINT n)
{
	UINT numBits = getBitLengthOfNumber(m);
	UINT ander = 1;
	UINT factor = 1;
	UINT sum = 0;
	for (UINT i = 0; i < numBits; i++)
	{
		if (m&ander)
			sum += factor;
		else
			sum += factor * 2;

		ander << 1;
		factor *= 4;
	}

	ander = 1;
	for (UINT i = 1; i < k; i++)
		ander << 1;

	if (sum & ander)
		return 1;
	else
		return 0;
}

template <typename In_Type, typename Out_Type>
void testgetKthStringAfterTransformation(In_Type& in, Out_Type& out)
{
	UINT numTestCases, m,k,n;
	INT element;
	in >> numTestCases;

	while (numTestCases-- > 0)
	{
		in >> m >> k >> n;
		out << getKthStringAfterTransformation(m,k,n)<<"\n";
	}
}

int main(int argc, char* argv[])
{
	std::ifstream in("BitStringProblem.txt");
	testgetKthStringAfterTransformation<std::ifstream, std::ostream>(in,std::cout);
	return 0;
}

