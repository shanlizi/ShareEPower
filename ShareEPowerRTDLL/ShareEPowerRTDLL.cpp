//////////////////////////////////////////////////////////////////
//
// ShareEPowerRTDLL.cpp - cpp file
//
// This file was generated using the RTX64 RTDLL Template for Visual Studio.
//
// Created: 11/2/2017 4:35:08 PM 
// User: zhcs
//
//////////////////////////////////////////////////////////////////
    
#include "ShareEPowerRTDLL.h"

// DllMain: Entry into a dynamic-link library (DLL).  It is called by the system
// when processes and threads are created and terminated, or upon calls to
// LoadLibrary and FreeLibrary.  DllMain must explicitly return TRUE, otherwise
// the RTDLL fails to load.
//
BOOL 
RTAPI 
DllMain(HINSTANCE hinstDLL,
		DWORD fdwReason,
		LPVOID lpvReserved)
{
   switch (fdwReason)
   {
		case DLL_PROCESS_ATTACH:

			// Indicates that the DLL is being loaded into the virtual address
			// space of the current process as a result of a call to
			// LoadLibrary (either explicitly or implicitly).
			break;

		case DLL_PROCESS_DETACH:

			// Indicates that the DLL is being unloaded from the virtual address
			// space of the calling process as a result of either a process exit
			// or a call to FreeLibrary (either explicitly or implicitly).
			break;
    }    

	return TRUE;
}
