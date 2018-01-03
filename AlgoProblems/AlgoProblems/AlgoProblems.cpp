// AlgoProblems.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <fstream>
#include <string>
#include "AnagramicPairs.h"
#include "DNAHealth.h"



template<typename inputType, typename outputType>
void testgetNumAnagramicPairs(inputType& in, outputType& out)
{
	typedef std::pair<std::string, UINT> pair;
	std::vector<pair> geneHealthList;
	UINT numGenes;
	in >> numGenes;

	for (UINT i = 0; i < numGenes; i++)
	{
		std::string gene;
		in>>gene;
		geneHealthList.push_back(pair(gene, 0));
	}

	for (UINT i = 0; i < numGenes; i++)
	{
		UINT geneHealth;
		in >> geneHealth;
		geneHealthList[i].second = geneHealth;
	}

	UINT numStrands;
	in >> numStrands;
	UINT maxGeneHealth = 0;
	UINT minGeneHealth = UINT_MAX;
	for (UINT i = 0; i < numStrands; i++)
	{
		std::unordered_map<std::string, UINT> geneHealthListCompressed;
		UINT start;
		UINT end;
		std::string dnaStrand;
		in >> start >> end;
		in >> dnaStrand;
		compressGeneHealthMap(geneHealthList, geneHealthListCompressed, start, end);
		UINT geneHealth = getDnaHealth(geneHealthListCompressed, dnaStrand);
		if (geneHealth > maxGeneHealth)
			maxGeneHealth = geneHealth;
		
		if (geneHealth < minGeneHealth)
			minGeneHealth = geneHealth;
	}

	out << minGeneHealth <<" "<< maxGeneHealth;
}

int main(int argc, char* argv[])
{
	//testgetNumAnagramicPairs(std::cin, std::cout);
	//test
	std::ifstream in("test.txt");
	testgetNumAnagramicPairs(in, std::cout);
	return 0;
}

