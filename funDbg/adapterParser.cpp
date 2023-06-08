#include "adapterParser.h"

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

#define C_ADAPTERPARSER_INTERNAL_INIT_VAL 0
#define AMDGPUSTRING "   Gpu device: VendorId 0x1002(ATI), DeviceId 0x"
#define AMDGPUSCHEDULER "   Scheduler: 0x"

C_adapterParser::C_adapterParser()
{
    m_AmdGpuInfo = (AMDGPUINFO *)malloc(sizeof(AMDGPUINFO));
    memset(m_AmdGpuInfo, 0, sizeof(AMDGPUINFO));
}

C_adapterParser::~C_adapterParser()
{
    if(m_AmdGpuInfo)
    {
        free(m_AmdGpuInfo);
    }
}

bool C_adapterParser::parseAMDGpuInfo(const char* arg)
{
    bool ret = true;
    std::string file1 = arg;
    std::istringstream iss(file1);

    std::string line;

    size_t po = -1;
    bool isAMDGpuFound = false;

    if(m_AmdGpuInfo)
    {
        while (std::getline(iss, line))
        {
            // check TDR level setting of the target
            if ((po = line.find(AMDGPUSTRING)) != -1)
            {
                isAMDGpuFound = true;
                m_AmdGpuInfo->m_DeviceID = stoll(line.substr(strlen(AMDGPUSTRING)), 0, 16);
            }

            if (isAMDGpuFound)
            {
                if ((po = line.find(AMDGPUSCHEDULER)) != -1)
                {
                    std::string amdGpuSch = line.substr(strlen(AMDGPUSCHEDULER));
                    memcpy(m_AmdGpuInfo->m_Scheduler, amdGpuSch.c_str(), sizeof(m_AmdGpuInfo->m_Scheduler));
                    //m_AmdGpuInfo->m_Scheduler = stoll(line.substr(strlen(AMDGPUSCHEDULER)), 0, 16);
                    break;
                }
            }
        }
    }


    return ret;
}

PAMDGPUINFO C_adapterParser::getAMDGpuInfo()
{
    return m_AmdGpuInfo;
}