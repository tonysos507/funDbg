#include "arkdbgxparser.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <regex>


C_arkdbgxParser::C_arkdbgxParser()
{
}

C_arkdbgxParser::~C_arkdbgxParser()
{
}

bool C_arkdbgxParser::parseMesFWVersion(const char* arg, unsigned int& schqFwVersion, unsigned int& kiqFwVersion)
{
    std::string file1 = arg;
    std::istringstream iss(file1);
    bool result = false;

    size_t po = -1;
    for(std::string line; std::getline(iss, line);)
    {
        if((line.find("3.0.0")) != -1)
        {
            if((po=line.find(": CP_MES_GP3_LO")) != -1)
            {
                schqFwVersion = stoi(line.substr(po-3, 3), nullptr, 16);
                result |= true;
            }
        }

        if((line.find("3.1.0")) != -1)
        {
            if((po=line.find(": CP_MES_GP3_LO")) != -1)
            {
                kiqFwVersion = stoi(line.substr(po-3, 3), nullptr, 16);
                result |= true;
            }
        }
    }

    return result;
}