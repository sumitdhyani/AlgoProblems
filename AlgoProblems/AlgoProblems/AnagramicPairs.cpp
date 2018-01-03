#include "AnagramicPairs.h"
#include <map>
#include <hash_map>

class StringRepresentation : public std::map < char, UINT >
{
public:
	bool operator <(const StringRepresentation& str)
	{
		auto it1 = this->begin();
		auto it2 = str.end();
		bool retVal;

		while (true)
		{
			if (it1 == this->end() && it2 != str.end())
				retVal = true;
			else if (it1 != this->end() && it2 == str.end())
				retVal = false;
			else if (it1 == this->end() && it2 == str.end())
				retVal = false;
			else if (it1->first < it2->first)
			{
				retVal = true;
				break;
			}
			else if (it1->first > it2->first)
			{
				retVal = false;
				break;
			}
			else if (it1->second < it2->second)
			{
				retVal = true;
				break;
			}
			else if (it1->second > it2->second)
			{
				retVal = false;
				break;
			}
			else
			{
				it1++;
				it2++;
			}
		}

		return retVal;
	}
};

template <typename Map, typename keytype>
void addNewOrDuplicate(Map& map, keytype key)
{
	auto it = map.find(key);
	if (it == map.end())
		map[key] = 1;
	else
		it->second++;
}

UINT getNumAnagramicPairs(const std::string& str)
{
	std::map<StringRepresentation, UINT> stringCounter;
	size_t len = str.length();
	for (UINT i = 0; i < len; i++)
	{
		StringRepresentation stringRepresentation;
		for (UINT j = i; j < len; j++)
		{
			//abba
			addNewOrDuplicate(stringRepresentation, str[j]);
			addNewOrDuplicate(stringCounter, stringRepresentation);
		}
	}

	UINT sum = 0;
	auto it = stringCounter.begin();
	while (it != stringCounter.end())
	{
		UINT n = it->second;
		sum += (n*(n - 1)) / 2;
		it++;
	}

	return sum;

}
