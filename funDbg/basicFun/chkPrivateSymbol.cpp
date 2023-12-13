#include "basicFun.h"

extern std::string executecommandout2buf(const char* cmd);

bool isPrivateSymbolLoad(std::string modulename)
{
    std::string cmds = "lmm";
    cmds += modulename;
    std::string result = executecommandout2buf(cmds.c_str());
    bool found_private_symbol = (result.find("private pdb symbols") == -1) ? false : true;
    return found_private_symbol;
}