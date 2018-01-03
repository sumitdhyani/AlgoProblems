#include "DNAHealth.h"

void compressGeneHealthMap(const std::vector<std::pair<std::string, UINT>>& in, std::unordered_map<std::string, UINT>& out, UINT start, UINT end)
{
	for (UINT itIn = start; itIn <= end; itIn++)
	{
		auto itOut = out.find(in[itIn].first);
		if (itOut == out.end())
			out[in[itIn].first] = in[itIn].second;
		else
			itOut->second += in[itIn].second;
	}
}

UINT getDnaHealth(const std::unordered_map<std::string, UINT>& geneHealthList, const std::string& dnaStrand)
{
	UINT sum = 0;
	for (auto it = geneHealthList.begin(); it != geneHealthList.end(); it++)
	{
		for (size_t pos = dnaStrand.find(it->first, 0); pos != std::string::npos; pos = dnaStrand.find(it->first, pos + 1))
		{
			sum += it->second;
		}
	}

	return sum;
}
