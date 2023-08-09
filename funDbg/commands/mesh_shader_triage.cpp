#include "..\dbgext.h"
#include <strsafe.h>
#include <string>
#include "..\out.h"
#include <regex>
#include "..\lgparser.h"
#include "..\adapters\adapterParser.h"

extern std::string executecommandout2buf(const char* cmd);

HRESULT CALLBACK
mesh_shader_triage(PDEBUG_CLIENT4 Client, PCSTR args)
{
    std::string bd = "==============================Reference Ticket SWDEV-376545=============================================\r\n";
    bd += "\r\n";
    bd += "There is function CS_OsRenderingContext::SubmitToUserQueue,  it is called when SUBMITCOMMANDVIRTUAL DDI\r\n";
    bd += "is called. And in this function, there is a check submitCommandInputUq.flags.bitfields.isGangSubmission\r\n";
    bd += "to check if the submission contains MESH shader(or gang submission), this flag is passed in to KMD from\r\n";
    bd += "OS(or to say, from UMD).\r\n";
    bd += "\r\n";
    bd += "If the submission is gang submission, KMD will check if the gang private compute user queue is created and\r\n";
    bd += "if not, create it, then submit the ACE compute IB to gang private compute user queue, other IBs is submitted\r\n";
    bd += "to non-gang private compute user queue.\r\n";
    bd += "\r\n";
    bd += "The two function calls to submit the IBs\r\n";
    bd += "m_pIUserQueueMgr->SubmitCommandToLegacyUqGangedCompute\r\n";
    bd += "m_pIUserQueueMgr->SubmitCommandToLegacyUQ\r\n";
    bd += "=======================================================================================================\r\n";
    dprintf("%s\n", bd.c_str());

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
        dprintf("%s\n", result.c_str());
#endif

        adapterObject.parseAMDGpuInfo(result.c_str());
        PAMDGPUINFO gpuInfo = adapterObject.getAMDGpuInfo();
        if(gpuInfo)
        {
            cmds = "!schstatus ";
            cmds += gpuInfo->m_Scheduler;
            result = executecommandout2buf(cmds.c_str());
        }

    }
    else
    {
        dprintf("\r\n\r\n!!!!!!Please load dxgkdx.dll and dxgkrnl.pdb/dxgmms2.pdb/amdkmdag.pdb private symbol first!!!!!\r\n\r\n");
    }

    return S_OK;
}