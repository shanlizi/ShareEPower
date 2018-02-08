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

#pragma once
//This define will deprecate all unsupported Microsoft C-runtime functions when compiled under RTSS.
//If using this define, #include <rtapi.h> should remain below all windows headers
//#define UNDER_RTSS_UNSUPPORTED_CRT_APIS

#include <SDKDDKVer.h>

// for RTTCPIP Support
#include <winsock2.h>
#include <ws2tcpip.h>

//#include <stdio.h>
//#include <string.h>
//#include <ctype.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <math.h>
//#include <errno.h>
#include <windows.h>
#include <tchar.h>
#include <rtapi.h>      // RTX64 APIs that can be used in real-time or Windows applications.

#ifdef UNDER_RTSS
#include <rtssapi.h>    // RTX64 APIs that can only be used in real-time applications.
#endif // UNDER_RTSS

#if defined UNDER_RTSS
  #if defined RTX64_EXPORTS
    #define ShareEPowerRTDLL_API __declspec(dllexport)
  #else
    #define ShareEPowerRTDLL_API __declspec(dllimport)
  #endif
#else
  #if defined DLL64_EXPORTS
    #define ShareEPowerRTDLL_API __declspec(dllexport)
  #else
    #define ShareEPowerRTDLL_API __declspec(dllimport)
  #endif
#endif


// Add DEFINES Here
// This class is exported from the ShareEPowerRTDLL.dll
class ShareEPowerRTDLL_API CShareEPowerRTDLL
{
public:
    CShareEPowerRTDLL(void);
    // TODO: add your methods here.
};
extern ShareEPowerRTDLL_API int nShareEPowerRTDLL;


// function prototype for the RTDLL exported function
ShareEPowerRTDLL_API
int 
RTAPI
Toggle(int argc, TCHAR * argv[]);
 
