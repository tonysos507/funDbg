#pragma once

class __declspec(dllexport) C_arkdbgxParser
{
public:
    C_arkdbgxParser();
    virtual ~C_arkdbgxParser();
    bool parseMesFWVersion(const char* arg, unsigned int& schqFwVersion, unsigned int& kiqFwVersion);

private:
};