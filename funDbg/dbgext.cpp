#include "dbgext.h"
#include <strsafe.h>
#include <string>
#include "out.h"
#include <regex>


WINDBG_EXTENSION_APIS   ExtensionApis;

extern "C"
HRESULT
CALLBACK
DebugExtensionInitialize(PULONG Version, PULONG Flags)
{
	IDebugClient *DebugClient;
	PDEBUG_CONTROL DebugControl;
	HRESULT Hr;

	*Version = DEBUG_EXTENSION_VERSION(1, 0);
	*Flags = 0;
	Hr = S_OK;

	if ((Hr = DebugCreate(__uuidof(IDebugClient),
		(void **)&DebugClient)) != S_OK)
	{
		return Hr;
	}

	if ((Hr = DebugClient->QueryInterface(__uuidof(IDebugControl),
		(void **)&DebugControl)) == S_OK)
	{

		//
		// Get the windbg-style extension APIS
		//
		ExtensionApis.nSize = sizeof(ExtensionApis);
		Hr = DebugControl->GetWindbgExtensionApis64(&ExtensionApis);

		DebugControl->Release();

	}
	DebugClient->Release();
	return Hr;
}

/*
  A built-in help for the extension dll
*/
HRESULT CALLBACK
help(PDEBUG_CLIENT4 Client, PCSTR args)
{
	//	INIT_API();

	UNREFERENCED_PARAMETER(args);

	dprintf("xjz's Debug Extension xjzext.dll\n"
		"  help                = Shows this help\n"
	);
	//	EXIT_API();

	return S_OK;
}

HRESULT CALLBACK
dumpstrings(PDEBUG_CLIENT4 Client, PCSTR args)
{
	ULONG64 Address;

	Address = GetExpression(args);
	DWORD Buffer[4], cb;

	// Read and display first 4 dwords at Address
	if (ReadMemory(Address, &Buffer, sizeof(Buffer), &cb) && cb == sizeof(Buffer)) {
		dprintf("%p: %08lx %08lx %08lx %08lx\n\n", Address,
			Buffer[0], Buffer[1], Buffer[2], Buffer[3]);
	}
	return S_OK;
}



void executecommand(const char* cmd)
{
	//std::string returnResult;
	IDebugClient4 *DebugClient;
	IDebugControl4* Control;
	HRESULT Hr;
	ULONG numOfProcess = 0;

	if ((Hr = DebugCreate(__uuidof(IDebugClient4),
		(void **)&DebugClient)) == S_OK)
	{
		if ((Hr = DebugClient->QueryInterface(__uuidof(IDebugControl4), (void**)&Control)) == S_OK)
		{
			StdioOutputCallbacks OutputCb;
			OutputCb.openOutpuLogFile();
			if (DebugClient->SetOutputCallbacks(&OutputCb) == S_OK)
			{

				Control->Execute(DEBUG_OUTCTL_THIS_CLIENT, cmd, DEBUG_EXECUTE_ECHO);
				DebugClient->SetOutputCallbacks(NULL);
				//returnResult = OutputCb.getResult();
				//dprintf("%d\n\n", returnResult.size());
			}
			OutputCb.closeOutputLogFile();
			Control->Release();
		}

		DebugClient->Release();
	}

	return;
}


HRESULT CALLBACK
dumpprocess(PDEBUG_CLIENT4 Client, PCSTR args)
{
	executecommand("!process 0 0");

	std::ifstream resultfile(INTERNALLOG);
	std::string line;
	while (std::getline(resultfile, line))
	{
		dprintf("%s\n", line.c_str());
	}

	return S_OK;
}

std::string executecommandout2buf(const char* cmd)
{
	std::string returnResult;
	IDebugClient4 *DebugClient;
	IDebugControl4* Control;
	HRESULT Hr;
	ULONG numOfProcess = 0;

	if ((Hr = DebugCreate(__uuidof(IDebugClient4),
		(void **)&DebugClient)) == S_OK)
	{
		if ((Hr = DebugClient->QueryInterface(__uuidof(IDebugControl4), (void**)&Control)) == S_OK)
		{
			StdioOutputCallback2bufs OutputCb;
			if (DebugClient->SetOutputCallbacks(&OutputCb) == S_OK)
			{

				Control->Execute(DEBUG_OUTCTL_THIS_CLIENT, cmd, DEBUG_EXECUTE_NOT_LOGGED);
				DebugClient->SetOutputCallbacks(NULL);
				//returnResult = OutputCb.getResult();
				//dprintf("%d\n\n", returnResult.size());
			}
			returnResult = OutputCb.getResult();
			Control->Release();
		}

		DebugClient->Release();
	}

	return returnResult;
}

HRESULT CALLBACK
showtime(PDEBUG_CLIENT4 Client, PCSTR args)
{
	std::string timestr = executecommandout2buf(".time");
	
	std::string timestr1 = "[";
	timestr1 += timestr.substr(24, 24);
	timestr1 += "] ";
	timestr1 += args;

	dprintf("%s\n", timestr1.c_str());

	return S_OK;
}

HRESULT CALLBACK
dump_ib(PDEBUG_CLIENT4 Client, PCSTR args)
{
    std::string svmid = executecommandout2buf(".printf \"%d\",@@c++(rSubmitCommandInput.vmInfo.vmid)");
    std::string sfenceid = executecommandout2buf(".printf \"%d\",@@c++(rSubmitCommandInput.fenceId)");
    std::string sibcount = executecommandout2buf(".printf \"%d\",@@c++(rSubmitCommandInput.ibCount)");
    std::string sclientid = executecommandout2buf(".printf \"%d\",@@c++(rSubmitCommandInput.clientId)");
    std::string ssizeInD = executecommandout2buf(".printf \"%d\",@@c++(sizeInDword)");
    std::string si = executecommandout2buf(".printf \"%d\",@@c++(i)");
    std::string sibgva = executecommandout2buf(".printf \"0x%I64x\",@@c++(ibgva)");
    int ivmid = std::stoi(svmid);
    int ifenceid = std::stoi(sfenceid);
    int iibcount = std::stoi(sibcount);
    int iclientid = std::stoi(sclientid);
    int isizeInD = std::stoi(ssizeInD) / 4;
    int ii = std::stoi(si);

    dprintf("client id: %d, vmid : %d, fence id : %d,  ibcount : %d, at %d of size in DWORD : %d, ibva : %s\n", iclientid, ivmid, ifenceid, iibcount, ii, isizeInD, sibgva);

    return S_OK;
}

HRESULT CALLBACK
dumpregisters_v_script(PDEBUG_CLIENT4 Client, PCSTR args)
{

 std::ifstream if1("D:\\mydev\\xjzext\\x64\\Release\\cmds.txt");
 std::string line;
 while (std::getline(if1, line))
 {
    if(line.empty())
     continue;

    if(line.find("!glook") == -1)
     continue;


    

    std::string timestr = executecommandout2buf(line.c_str());



    dprintf("%s\n", timestr.c_str());
    
 }

 return S_OK;
}


HRESULT CALLBACK gs(PDEBUG_CLIENT Client, PCSTR args)
{
	HRESULT hr = S_OK;
//	if (!DebugExt::GetDebugExt(EXT_PLATFORM__WINDOWS)->command_gs(Client, (char *)args))
	{
		hr = S_FALSE;
	}
	return hr;
}

HRESULT CALLBACK
check_phydevobj(PDEBUG_CLIENT4 Client, PCSTR args)
{
  std::string cmds = "x amdkmdag!g_pProxyLinkManager";
  std::string result = executecommandout2buf(cmds.c_str());

  std::string cmd1 = result.substr(result.find("= ")+2);
  cmds = "dx -r1 (*((amdkmdag!ProxyAdapterLink * (*)[30])";
  cmds += cmd1;
  cmds += "))";

  result = executecommandout2buf(cmds.c_str());

  dprintf("%s\n", result.c_str());

 return S_OK;
}

HRESULT CALLBACK
dump_psp(PDEBUG_CLIENT4 Client, PCSTR args)
{
 for (int i = 0; i < 16; i++)
 {
  std::string cmds = "??psp->km_cmd_context.km_cmd_buf_pool[";
  cmds += std::to_string(i);
  cmds += "].cmd_mem.virtual_address";
  std::string result = executecommandout2buf(cmds.c_str());
  dprintf("%s = %s\n", cmds.c_str(), result.c_str());
 }


 return S_OK;
}

HRESULT CALLBACK
check_current_process_name(PDEBUG_CLIENT4 Client, PCSTR args)
{
 std::string sargs = args;

 std::string cmds = "!process -1 0";
 std::string result = executecommandout2buf(cmds.c_str());

 std::istringstream ss(result);
 std::string line;
 std::string imagename;
 std::string processaddress;
 while (std::getline(ss, line))
 {
  size_t pos = line.find("Image: ");
  if (pos != -1)
  {
   imagename = line.substr(pos + 7);
   continue;
  }
  pos = line.find("PROCESS ");
  if (pos != -1)
  {
   processaddress = line.substr(pos+8);
   continue;
  }
 }
 sargs = "System";
 if (imagename == sargs)
 {
  executecommandout2buf("g");
 }
 else
 {
  //std::string thisaddr = executecommandout2buf(".printf \"%p\",poi(this)");
  dprintf("#############%s->%s->buffer addr:%s\n", imagename.c_str(), processaddress.c_str()/*, thisaddr.c_str()*/);
  //executecommandout2buf("g");
 }
 
 return S_OK;
}

HRESULT CALLBACK
regfieldlook(PDEBUG_CLIENT4 Client, PCSTR args)
{
 std::string reg_field = args;
 std::string reg = reg_field.substr(0,reg_field.find(" "));
 std::string field = reg_field.substr(reg_field.find(" ")+1);
 //dprintf("%s\n", field.c_str());
 std::string cmd = "!glook /fields ";
 cmd += reg;
 std::string result = executecommandout2buf(cmd.c_str());
 std::stringstream ss(result);
 std::string line;
 //dprintf("%s\n", result.c_str());
 while(std::getline(ss, line))
 {
  //dprintf("%s\n", line.c_str());
  if(line.find(field.c_str()) != std::string::npos)
  {
   dprintf("%s\n", line.c_str());
   break;
  }
 }
 
 
 return S_OK;
}


