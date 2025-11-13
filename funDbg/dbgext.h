#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Define KDEXT_64BIT to make all wdbgexts APIs recognize 64 bit addresses
// It is recommended for extensions to use 64 bit headers from wdbgexts so
// the extensions could support 64 bit targets.
//
#define KDEXT_64BIT
#include <wdbgexts.h>
#include <dbgeng.h>

#pragma warning(disable:4201) // nonstandard extension used : nameless struct
#include <extsfns.h>


#define INTERNALLOG "d:\\xjzext.log"

extern HINSTANCE g_hInstance;