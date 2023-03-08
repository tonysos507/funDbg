#pragma once
#include <fstream>
#include <sstream>

class StdioOutputCallbacks : public IDebugOutputCallbacks
{
public:
	// IUnknown.
	STDMETHOD(QueryInterface)(
		THIS_
		_In_ REFIID InterfaceId,
		_Out_ PVOID* Interface
		);
	STDMETHOD_(ULONG, AddRef)(
		THIS
		);
	STDMETHOD_(ULONG, Release)(
		THIS
		);

	// IDebugOutputCallbacks.
	STDMETHOD(Output)(
		THIS_
		_In_ ULONG Mask,
		_In_ PCSTR Text
		);

	std::string getResult();
	void openOutpuLogFile();
	void closeOutputLogFile();

private:
	std::string m_OutputBuffer;
	std::ofstream m_OutputFile;
	std::stringstream m_Outputss;
};

class StdioOutputCallback2bufs : public IDebugOutputCallbacks
{
public:
	// IUnknown.
	STDMETHOD(QueryInterface)(
		THIS_
		_In_ REFIID InterfaceId,
		_Out_ PVOID* Interface
		);
	STDMETHOD_(ULONG, AddRef)(
		THIS
		);
	STDMETHOD_(ULONG, Release)(
		THIS
		);

	// IDebugOutputCallbacks.
	STDMETHOD(Output)(
		THIS_
		_In_ ULONG Mask,
		_In_ PCSTR Text
		);

	std::string getResult();

private:
	std::string m_OutputBuffer;
};

