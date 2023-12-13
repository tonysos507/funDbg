#include "..\dbgext.h"
#include <strsafe.h>
#include <string>
#include "..\out.h"
#include <regex>
#include "..\arkdbgxParser.h"

extern std::string executecommandout2buf(const char* cmd);

HRESULT CALLBACK
mes_triage(PDEBUG_CLIENT4 Client, PCSTR args)
{
    std::string cmds = ".chain";
    std::string result = executecommandout2buf(cmds.c_str());
    bool found = (result.find("arkdbgx.dll") == -1) ? false : true;
    cmds = "lmmamdkmdag";
    result = executecommandout2buf(cmds.c_str());
    bool found_amdkmdag_private_symbol = (result.find("private pdb symbols") == -1) ? false : true;

    if (found && found_amdkmdag_private_symbol)
    {
        C_arkdbgxParser arkdbgxObject;
        unsigned int schqFwVersion = 0, kiqFwVersion = 0;
        bool status = false;

        cmds = "!glook /p /mes 0 0 /field cp_mes_gp3_lo;!glook /p /mes 1 0 /field cp_mes_gp3_lo";
        result = executecommandout2buf(cmds.c_str());

        status = arkdbgxObject.parseMesFWVersion(result.c_str(), schqFwVersion, kiqFwVersion);
        if(status)
        {
            dprintf("MES SCHQ FW version: 0x%x\n", schqFwVersion);
            dprintf("MES KIQ FW version: 0x%x\n", kiqFwVersion);
        }
    }

    return S_OK;
}