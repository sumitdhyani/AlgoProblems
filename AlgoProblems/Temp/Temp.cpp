// Temp.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

struct StringAnagram : public std::map<char, int>
{
	explicit StringAnagram(const std::string& str)
	{
		for (int i = 0; i < str.length(); i++)
			add(str[i]);
	}

	StringAnagram()
	{
	}

	void add(char ch)
	{
		this->operator[](ch)++;
	}

	void remove(char ch)
	{
		auto it = find(ch);
		if (it != end())
		{
			it->second--;
			if (0 == it->second)
				erase(it);
		}
	}
};

int findNumAnagrams(std::string input, std::string pattern)
{
	StringAnagram anagaram_pattern(pattern);
	StringAnagram anagaram_input;
	int numAnagrams = 0;

	for (int i = 0, j = 0; j < input.length(); )
	{
		anagaram_input.add(input[j]);
		if (j - i + 1 > pattern.length())
		{
			anagaram_input.remove(input[i]);
			i++;
		}

		if (j - i + 1 == pattern.length() &&
			(anagaram_input == anagaram_pattern)
			)
			numAnagrams++;

		j++;
	}

	return numAnagrams;
}

template <typename In_type, typename Out_type>
void testfindNumAnagrams(In_type& in, Out_type& out)
{
	int n;
	std::string num;
	std::getline(in, num);
	n = atoi(num.c_str());
	std::string input, pattern;
	while (n-- > 0)
	{
		std::getline(in,input);
		std::getline(in,pattern);
		out << findNumAnagrams(input, pattern)<<std::endl;
	}
}

int main(int argc, char* argv[])
{
	std::ifstream in("test.txt");
	testfindNumAnagrams<std::ifstream, std::ostream>(in, std::cout);
	//findNumAnagrams("BACDGABCDA", "ABCD", out);
	return 0;
}

