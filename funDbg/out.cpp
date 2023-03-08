#include <stdio.h>
#include <windows.h>
#include <dbgeng.h>
#include <string>
#include "out.h"
#include "dbgext.h"


STDMETHODIMP
StdioOutputCallbacks::QueryInterface(
	THIS_
	_In_ REFIID InterfaceId,
	_Out_ PVOID* Interface
)
{
	*Interface = NULL;

	if (IsEqualIID(InterfaceId, __uuidof(IUnknown)) ||
		IsEqualIID(InterfaceId, __uuidof(IDebugOutputCallbacks)))
	{
		*Interface = (IDebugOutputCallbacks *)this;
		AddRef();
		return S_OK;
	}
	else
	{
		return E_NOINTERFACE;
	}
}

STDMETHODIMP_(ULONG)
StdioOutputCallbacks::AddRef(
	THIS
)
{
	// This class is designed to be static so
	// there's no true refcount.
	return 1;
}

STDMETHODIMP_(ULONG)
StdioOutputCallbacks::Release(
	THIS
)
{
	// This class is designed to be static so
	// there's no true refcount.
	return 0;
}

STDMETHODIMP
StdioOutputCallbacks::Output(
	THIS_
	_In_ ULONG Mask,
	_In_ PCSTR Text
)
{
	UNREFERENCED_PARAMETER(Mask);
	//fputs(Text, stdout);
	//m_OutputBuffer.append(Text);
	//m_OutputBuffer += Text;
	m_OutputFile << Text;
	return S_OK;
}

std::string StdioOutputCallbacks::getResult()
{
	return m_Outputss.str();
}

void StdioOutputCallbacks::openOutpuLogFile()
{
	m_OutputFile.open(INTERNALLOG);
}

void StdioOutputCallbacks::closeOutputLogFile()
{
	m_OutputFile.flush();
	m_OutputFile.close();
}


////////////////////////////////
STDMETHODIMP
StdioOutputCallback2bufs::QueryInterface(
	THIS_
	_In_ REFIID InterfaceId,
	_Out_ PVOID* Interface
)
{
	*Interface = NULL;

	if (IsEqualIID(InterfaceId, __uuidof(IUnknown)) ||
		IsEqualIID(InterfaceId, __uuidof(IDebugOutputCallbacks)))
	{
		*Interface = (IDebugOutputCallbacks *)this;
		AddRef();
		return S_OK;
	}
	else
	{
		return E_NOINTERFACE;
	}
}

STDMETHODIMP_(ULONG)
StdioOutputCallback2bufs::AddRef(
	THIS
)
{
	// This class is designed to be static so
	// there's no true refcount.
	return 1;
}

STDMETHODIMP_(ULONG)
StdioOutputCallback2bufs::Release(
	THIS
)
{
	// This class is designed to be static so
	// there's no true refcount.
	return 0;
}

STDMETHODIMP
StdioOutputCallback2bufs::Output(
	THIS_
	_In_ ULONG Mask,
	_In_ PCSTR Text
)
{
	UNREFERENCED_PARAMETER(Mask);
	//fputs(Text, stdout);
	//m_OutputBuffer.append(Text);
	m_OutputBuffer += Text;
	return S_OK;
}

std::string
StdioOutputCallback2bufs::getResult()
{
	return m_OutputBuffer;
}