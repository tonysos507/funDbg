#include "WinExt.hpp"

WinExt* WinExt::m_pSingletonWinExt = nullptr;

WinExt* WinExt::GetWinExt()
{
    if(m_pSingletonWinExt == nullptr)
    {
        m_pSingletonWinExt = new WinExt();
        if(m_pSingletonWinExt != nullptr)
        {
            m_pSingletonWinExt->Initialize();
        }
    }
    return m_pSingletonWinExt;
}

WinExt::WinExt()
{
    m_pLocalClient = nullptr;
    m_pExtControl = nullptr;
    m_pDebugControl = nullptr;
}

WinExt::~WinExt()
{
    if(m_pDebugControl != nullptr)
    {
        m_pDebugControl->Release();
    }

    if(m_pExtControl != nullptr)
    {
        m_pExtControl->Release();
    }

    if (m_pLocalClient != nullptr)
    {
        m_pLocalClient->EndSession(DEBUG_END_PASSIVE);
        m_pLocalClient->Release();
        m_pLocalClient = nullptr;
    }
}

bool WinExt::Initialize()
{
    bool ret = true;
	if (DebugCreate(__uuidof(IDebugClient), (void**)&m_pLocalClient) != S_OK)
	{
        ret = false;
	}
    if(m_pLocalClient->QueryInterface(__uuidof(IDebugControl4), (void **)&m_pExtControl) != S_OK)
    {
        ret = false;
    }
    if(m_pLocalClient->QueryInterface(__uuidof(IDebugControl), (void **)&m_pDebugControl) != S_OK)
    {
        ret = false;
    }

    ExtensionApis.nSize = sizeof(ExtensionApis);
    m_pDebugControl->GetWindbgExtensionApis64(&ExtensionApis);

    return ret;
}

void WinExt::printRed(const char* buffer)
{
    m_pExtControl->ControlledOutput(DEBUG_OUTCTL_AMBIENT_DML, DEBUG_OUTPUT_NORMAL, "%s%s%s", "<col fg=\"changed\"><b>", buffer, "</b></col>");
}