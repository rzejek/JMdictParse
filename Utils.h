#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#include "Entry.h"

void SaveDictionaryToFile(std::vector<Entry > dictionary, const char* xmlFile);

Entry FindValueInDictionary(std::vector<Entry >  & Dictionary, std::string value);

