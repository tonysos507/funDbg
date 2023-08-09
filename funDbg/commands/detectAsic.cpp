#include "..\dbgext.h"
#include <strsafe.h>
#include <string>
#include "..\out.h"
#include <regex>
#include "..\lgparser.h"
#include "..\adapters\adapterParser.h"

extern std::string executecommandout2buf(const char* cmd);

HRESULT CALLBACK
detectAsic(PDEBUG_CLIENT4 Client, PCSTR args)
{
    std::string cmds = ".chain";
    std::string result = executecommandout2buf(cmds.c_str());
    bool found = (result.find("dxgkdx.dll") == -1) ? false : true;
    cmds = "lmmamdkmdag";
    result = executecommandout2buf(cmds.c_str());
    bool found_amdkmdag_private_symbol = (result.find("private pdb symbols") == -1) ? false : true;

    cmds = "lmmdxgkrnl";
    result = executecommandout2buf(cmds.c_str());
    bool found_dxgkrnl_private_symbol = (result.find("private pdb symbols") == -1) ? false : true;

    cmds = "lmmdxgmms2";
    result = executecommandout2buf(cmds.c_str());
    bool found_dxgmms2_private_symbol = (result.find("private pdb symbols") == -1) ? false : true;

    if (found && found_amdkmdag_private_symbol && found_dxgkrnl_private_symbol && found_dxgmms2_private_symbol)
    {
        cmds = "!adapters";
        result = executecommandout2buf(cmds.c_str());
        C_adapterParser adapterObject;

#ifdef DBG
//        dprintf("%s\n", result.c_str());
#endif

        adapterObject.parseAMDGpuInfo(result.c_str());
        PAMDGPUINFO gpuInfo = adapterObject.getAMDGpuInfo();
        int numG = adapterObject.getNumGpu();
        dprintf("\r\n-------------> %d AMD GPU found <--------------\r\n", numG);
        if (gpuInfo)
        {   for(int z = 0; z < numG; z++)
            {
                dprintf("%d : ", z);
                dprintf(" DID: 0x%lx\r\n", gpuInfo[z].m_DeviceID);
            }
//            cmds = "!schstatus ";
//            cmds += gpuInfo->m_Scheduler;
//            result = executecommandout2buf(cmds.c_str());
        }
    }
    else
    {
        dprintf("\r\n\r\n!!!!!!Please load dxgkdx.dll and dxgkrnl.pdb/dxgmms2.pdb/amdkmdag.pdb private symbol first!!!!!\r\n\r\n");
    }

    return S_OK;
}