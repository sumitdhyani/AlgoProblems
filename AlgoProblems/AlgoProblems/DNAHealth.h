#pragma once
#include "CommonUtils.h"
#include <unordered_map>
#include <string>

void compressGeneHealthMap(const std::vector<std::pair<std::string, UINT>>& in, std::unordered_map<std::string, UINT>& out, UINT start, UINT end);

UINT getDnaHealth(const std::unordered_map<std::string, UINT>& geneHealthList, const std::string& dnaStrand);


