#pragma once

typedef struct _AMDGPUINFO
{
    unsigned long long m_DeviceID;
    char               m_Scheduler[17];
}AMDGPUINFO, *PAMDGPUINFO;

class __declspec(dllexport) C_adapterParser
{
public:
    C_adapterParser();
    virtual ~C_adapterParser();
    bool parseAMDGpuInfo(const char* arg);
    PAMDGPUINFO getAMDGpuInfo();
    int getNumGpu();

private:


private:


private:
    PAMDGPUINFO m_AmdGpuInfo;
    int         m_numGpu;
};