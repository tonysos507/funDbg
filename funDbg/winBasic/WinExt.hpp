#ifndef __WINEXT_H__
#define __WINEXT_H__

#include "..\dbgext.h"

class WinExt
{
public:
    static WinExt *GetWinExt();
    static void ReleaseWinExt();

    virtual ~WinExt();
    virtual bool Initialize();



    static WinExt* m_pSingletonWinExt;

private:
    WinExt();

    PDEBUG_CLIENT m_pLocalClient;
    PDEBUG_CONTROL4 m_pExtControl;
    PDEBUG_CONTROL m_pDebugControl;
};

#endif