//////////////////////////////////////////////////////////////////
//
// TestPowerPjt.cpp - cpp file
//
// This file was generated using the RTX64 Application Template for Visual Studio.
//
// Created: 11/1/2017 5:48:10 PM 
// User: zhcs
//
//////////////////////////////////////////////////////////////////

#include "TestPowerPjt.h"
#include "ShareEPower.h"

   
#define POWER_CHANNEL_NUM	2

int _tmain(int argc, _TCHAR * argv[])
{
	WSADATA		wsd;
	double dValueRead;

	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
		return false;

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */
	//if ( LOBYTE( wsd.wVersion ) != 2 || HIBYTE( wsd.wVersion ) != 2 ) 
	if (wsd.wVersion != 2)
	{
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return -1;
	}

	/* N5749A的默认IP为169.254.57.0 */
	ShareE_Open_SCPI_Instrument(POWER_AGILENT6702_TYPE, "192.168.138.163");
	ShareE_Open_SCPI_Instrument(POWER_AGILENT5749_TYPE, "192.168.138.164");
	ShareE_Open_SCPI_Instrument(POWER_SG_TYPE, "192.168.138.165");

	Power_Init(POWER_AGILENT6702_TYPE);
	Power_Init(POWER_AGILENT5749_TYPE);
	for (int j = 0; j < POWER_CHANNEL_NUM; j++)
	{
		ShareE_PowerSupply_Turnon(POWER_AGILENT6702_TYPE, j + 1);
	}
	ShareE_PowerSupply_Turnon(POWER_AGILENT5749_TYPE, 1);

	for (int i = 0; i < 20; i += 5)
	{
		//printf("Set to %i V\n", i);
		for (int j = 0; j < POWER_CHANNEL_NUM; j++)
		{
			ShareE_PowerSupply_Set_Voltage(POWER_AGILENT6702_TYPE, j + 1, i + j, 1);
			ShareE_PowerSupply_Set_Voltage(POWER_AGILENT5749_TYPE, j + 1, i + j, 1);
		}
		Sleep(3000);
		ShareE_PowerSupply_Get_Voltage(POWER_AGILENT6702_TYPE, 1, &dValueRead);
		ShareE_PowerSupply_Get_Voltage(POWER_AGILENT5749_TYPE, 1, &dValueRead);
		//printf("ReadV %f \n", dValueRead);
		Sleep(2000);
	}

	for (int j = 0; j < POWER_CHANNEL_NUM; j++)
	{
		ShareE_PowerSupply_Turnoff(POWER_AGILENT6702_TYPE, j + 1);
	}
	ShareE_PowerSupply_Turnoff(POWER_AGILENT5749_TYPE, 1);



	/************************SG系列大功率电源***********************/
	if (Power_Init(POWER_SG_TYPE))
	{
		SGPower_Set_Voltage(5.0);
		Sleep(60);
		SGPower_Set_Current(1.5);
		Sleep(60);
		SGPower_Set_Current(1.55);
		Sleep(60);
		SGPower_Set_Current(1);
	}
	ShareE_Close_SCPI_Instrument(POWER_AGILENT6702_TYPE);
	ShareE_Close_SCPI_Instrument(POWER_AGILENT5749_TYPE);
	ShareE_Close_SCPI_Instrument(POWER_SG_TYPE);

	WSACleanup();
}
