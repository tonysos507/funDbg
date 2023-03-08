#pragma once

#define MAXNUM_ADAPTERS 5
#define MAXNUM_ENGINES 15
#define MAXLEN_NAME 5
#define MAXNUM_FENCETYPE 11

enum TdrLevel
{
    TdrLevelOff = 0,
    TdrLevelBugcheck,
    TdrLevelUndefined
};

enum FenceType
{
    vidschGen = 0,
    subToDriver,
    prosedbyGpu,
    completebyGpu,
    preemtybyGpu,
    faultedbyGpu,
    vidschProces,
    vidschGenPremty,
    subToDriverPremty,
    completebyGpuPremty,
    vidschCompletePremty,
};

struct EngineInfo
{
    unsigned int nodeOrdianl;
    char         name[MAXLEN_NAME];
    unsigned int fenceInfo[MAXNUM_FENCETYPE];
};

struct AdapterInfo
{
    EngineInfo   engineInfo[MAXNUM_ENGINES];
};

class __declspec(dllexport) C_lgParser
{
public:
    C_lgParser();
    virtual ~C_lgParser();
    bool parser(const char* arg);

private:
    bool extractFenceID(const char* arg, const char* searchstr, FenceType ft);

private:
    // put temp value
    unsigned int m_TempAdapterIndex;

private:
    TdrLevel m_TdrLevel;
    AdapterInfo m_Adapters[MAXNUM_ADAPTERS];
};