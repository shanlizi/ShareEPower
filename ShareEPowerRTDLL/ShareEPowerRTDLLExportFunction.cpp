//////////////////////////////////////////////////////////////////
//
// ShareEPowerRTDLLRTDLLExportFunction.cpp - cpp file
//
// This file was generated using the RTX64 RTDLL Template for Visual Studio.
//
// Created: 11/2/2017 4:35:08 PM
// User: zhcs
//
//////////////////////////////////////////////////////////////////
    
#include "ShareEPowerRTDLL.h"
#include "ShareEPower.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDKDDKVer.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <tchar.h>
#include <rtapi.h>      // RTX64 APIs that can be used in real-time or Windows applications.

#define MAX_RECV_BUFFER_LEN 			1024 /*���ջ����������ֵ*/

char g_RecvBuf[MAX_RECV_BUFFER_LEN];/*SCPIȫ�ֽ��ջ�����*/
SOCKET g_PowerSock[4] = { -1,-1,-1. - 1 };/*SCPIȫ��ͨ��socket*/


#ifdef _MSC_VER
void taskDelay(int nDelay)
{
	Sleep(nDelay * 67);
}
#endif

#ifdef UNDER_RTSS
#include <rtssapi.h>    // RTX64 APIs that can only be used in real-time applications.
#endif // UNDER_RTSS

// An exported variable.
ShareEPowerRTDLL_API int nShareEPowerRTDLL = 0;

// Exported RTDLL function
ShareEPowerRTDLL_API
int 
RTAPI
Toggle(int argc, TCHAR * argv[])
{
    return 0;
}


// This is the constructor of a class that has been exported.
// see ShareEPowerRTDLL.h for the class definition
CShareEPowerRTDLL::CShareEPowerRTDLL()
{
        return;
}


//Input:  bRead, bWrite, bExcept --- true: check, false un-check
//Output: bit_0=1---ready for read, bit_1=1---ready for write, bit_2==1---except happened
int SelectCheck(SOCKET hSocket, BOOL bCheckRead, BOOL bCheckWrite, BOOL bCheckExcept, long timeout_usec)
{
	fd_set  fdWrite, fdRead, fdExcept;
	fd_set  *pReadFd = NULL, *pWriteFd = NULL, *pExceptFd = NULL;
	struct timeval timeout;
	int  Result;

	timeout.tv_sec = 0;
	timeout.tv_usec = timeout_usec;
	if (bCheckRead)
	{
		FD_ZERO(&fdRead);
		FD_SET(hSocket, &fdRead);
		pReadFd = &fdRead;
	}
	if (bCheckWrite)
	{
		FD_ZERO(&fdWrite);
		FD_SET(hSocket, &fdWrite);
		pWriteFd = &fdWrite;
	}
	if (bCheckExcept)
	{
		FD_ZERO(&fdExcept);
		FD_SET(hSocket, &fdExcept);
		pExceptFd = &fdExcept;
	}

	Result = 0;
	if (select(FD_SETSIZE, pReadFd, pWriteFd, pExceptFd, &timeout) == -1) return Result;

	if (bCheckRead && FD_ISSET(hSocket, &fdRead)) Result |= 0x01;
	if (bCheckWrite && FD_ISSET(hSocket, &fdWrite)) Result |= 0x02;
	if (bCheckExcept&& FD_ISSET(hSocket, &fdExcept)) Result |= 0x04;
	taskDelay(10);
	return Result;
}

/***********************************************************************
������:
ShareE_Open_SCPI_Instrument
������
��SCPIָ����豸������ָ��ͨ��
���أ�
OK����ͨ������
ERROR����ͨ��ʧ��
��ע��
��ʼ������ͨ���󣬹رյ�Դ����ͨ������������ʼ״̬
***********************************************************************/

STATUS ShareE_Open_SCPI_Instrument(int type, const char* lpszIP)
{
	int ret;
	struct sockaddr_in addrSrv;
	memset(&addrSrv, 0, sizeof(struct sockaddr_in));

	switch (type)
	{
	case POWER_AGILENT6702_TYPE:
		addrSrv.sin_addr.s_addr = inet_addr(lpszIP);
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(AGLIENT_POWER_SVRPORT);
		printf("Connecting Power %s:%u\n", lpszIP, AGLIENT_POWER_SVRPORT);
		break;
	case POWER_EA_TYPE:
		addrSrv.sin_addr.s_addr = inet_addr(lpszIP);
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(EA_POWER_SVRPORT);
		printf("Connecting Power %s:%u\n", lpszIP, EA_POWER_SVRPORT);
		break;
	case POWER_SG_TYPE:
		addrSrv.sin_addr.s_addr = inet_addr(lpszIP);
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(SG_POWER_SVRPORT);
		printf("Connecting Power %s:%u\n", lpszIP, SG_POWER_SVRPORT);
		break;
	case POWER_AGILENT5749_TYPE:
		addrSrv.sin_addr.s_addr = inet_addr(lpszIP);
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(AGLIENT5749_POWER_SVRPORT);
		printf("Connecting Power %s:%u\n", lpszIP, AGLIENT5749_POWER_SVRPORT);
		break;
	default:
		printf("%s(Line:%d):type %d is illegal ,only 0,1,2,3 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}
	g_PowerSock[type] = socket(AF_INET, SOCK_STREAM, 0);

	ret = connect(g_PowerSock[type], (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if (ret < 0)
	{
		printf("error in ShareE_Open_SCPI_Instrument, connect ret=%d,%d\n", ret, WSAGetLastError());
		return ERROR;
	}
	printf("Power connected\n");

	taskDelay(1);/*��֤socket����*/

	switch (type)
	{
	case POWER_AGILENT6702_TYPE:
	case POWER_AGILENT5749_TYPE:
		ret = ShareE_SCPI_Write(type, "DISP:VIEW METER4\n");
		if (ret != OK)
		{
			printf("Anglient Power Supply Init output Error\n");
			return ERROR;
		}
		ret = ShareE_SCPI_Write(type, "OUTP OFF,(@1:2)\n");
		if (ret != OK)
		{
			printf("Anglient Power Supply Init output Error\n");
			return ERROR;
		}
		break;
	case POWER_EA_TYPE:
	{
		/*�л�ΪԶ�̿���ģʽ*/
		ret = ShareE_SCPI_Write(POWER_EA_TYPE, "SYST:LOCK:STAT 1\n");
		if (ret != OK)
		{
			printf("Can't change to Remote Control\n");
			return ERROR;
		}
		taskDelay(10);/*�ȴ��л�ģʽ���*/
		ret = ShareE_SCPI_Write(POWER_EA_TYPE, "OUTP OFF\n");//�ر����
		if (ret != OK)
		{
			printf("EA Power Supply Init output Error\n");
			return ERROR;
		}
	}
	break;
	case POWER_SG_TYPE:
		break;
	default:
		printf("%s(Line:%d):type %d is illegal ,only 0,1,2,3 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}
	return OK;
}

/***********************************************************************
������:
ShareE_SCPI_Request
������
���ô˺�����������Ӧ���SCPIָ������ַ����ķ���Ӧ��
������
RequstBuffer�� �����SCPIָ��
���أ�
��ΪNULL��
NULL��û���ݷ���
��ע��
ʹ��������socketͨ�ţ�����û�з��ػ�һֱ����
***********************************************************************/
char * ShareE_SCPI_Request(int type, char *RequstBuffer)
{
	int len = 0;
	int ret = 0;

	if (type != POWER_AGILENT6702_TYPE&&type != POWER_EA_TYPE)
	{
		printf("%s(Line:%d):type %d is illegal ,only 0,1 available \n", __FUNCTION__, __LINE__, type);
		return NULL;
	}
	ret = ShareE_SCPI_Write(type, RequstBuffer);
	if (ret != OK)
	{
		return NULL;
	}

	len = recv(g_PowerSock[type], g_RecvBuf, MAX_RECV_BUFFER_LEN, 0);
	if (len <= 0)
	{
		return NULL;
	}
	return g_RecvBuf;
}
/***********************************************************************
������:
ShareE_SCPI_ReadStr
������
�����ջ������Ƿ������ݿɶ�����ȡ��ʱ
������
ulUsSecond: �ȴ���ȡ��ʱ����λus
reply: �ӻ������ж�ȡ���ַ���������ֵ��ΪERRORʱ��Ч
���أ�
OK�����յ�����
ERROR��δ���ܵ�����
��ע��
***********************************************************************/
STATUS ShareE_SCPI_ReadStr(int type, char * reply, unsigned long ulUsSecond)
{
	int ret = 0;
	int len = 0;

	if (type != POWER_AGILENT6702_TYPE&&type != POWER_EA_TYPE)
	{
		printf("%s(Line:%d):type %d is illegal ,only 0,1 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}
	/* ���socket �Ƿ������ݣ������ó�ʱ*/
	ret = SelectCheck(g_PowerSock[type], TRUE, FALSE, FALSE, ulUsSecond);
	if ((ret & 1) != 0x01)
	{
		//printf("%s(Line:%d):reply timeout! \n",__FUNCTION__, __LINE__);
		return ERROR;
	}
	/*��������ݶ�ȡ*/
	len = recv(g_PowerSock[type], g_RecvBuf, MAX_RECV_BUFFER_LEN, 0);
	if (len <= 0)
	{
		memset(reply, 0, sizeof(reply));
		printf("%s(Line:%d):recv error\n", __FUNCTION__, __LINE__);
		return ERROR;
	}
	g_RecvBuf[len] = 0;
	memcpy(reply, g_RecvBuf, len + 1);
	return OK;
}
/***********************************************************************
������:
ShareE_SCPI_Read
������
�����ջ������Ƿ������ݿɶ�����ȡ��ʱ
������
ulUsSecond: �ȴ���ȡ��ʱ����λus
dValue: �ӻ������ж�ȡ�����ݣ�����ֵ��ΪERRORʱ��Ч
���أ�
OK�����յ�����
ERROR��δ���ܵ�����
��ע��
***********************************************************************/
STATUS ShareE_SCPI_Read(int type, double* dValue, unsigned long ulUsSecond)
{
	int ret = 0;
	int len = 0;

	if (type != POWER_AGILENT6702_TYPE&&type != POWER_EA_TYPE)
	{
		printf("%s(Line:%d):type %d is illegal ,only 0,1 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}
	/* ���socket �Ƿ������ݣ������ó�ʱ*/
	ret = SelectCheck(g_PowerSock[type], TRUE, FALSE, FALSE, ulUsSecond);
	if ((ret & 1) != 0x01)
	{
		//printf("%s(Line:%d):reply timeout! \n",__FUNCTION__, __LINE__);
		return ERROR;
	}
	/*��������ݶ�ȡ*/
	len = recv(g_PowerSock[type], g_RecvBuf, MAX_RECV_BUFFER_LEN, 0);
	if (len <= 0)
	{
		return ERROR;
	}
	*dValue = atof(g_RecvBuf);
	return OK;
}

/***********************************************************************
������:
ShareE_SCPI_Write
������
������Ӧ���SCPIָ��
������
buffer SCPIָ��
���أ�
OK��ָ���ѷ���
ERROR��ָ���ʧ�ܣ�socket�쳣
��ע��
***********************************************************************/
STATUS ShareE_SCPI_Write(int type, char *buffer)
{
	int ret = 0;
	unsigned long ulStrLen = strlen(buffer);

	if (type != POWER_AGILENT6702_TYPE&&type != POWER_EA_TYPE&&type != POWER_SG_TYPE&&type != POWER_AGILENT5749_TYPE)
	{
		printf("%s(Line:%d):type %d is illegal ,only 0,1,2,3 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}

	if (g_PowerSock[type] == -1)
	{
		printf("%s(Line:%d):type%d socket error  \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}

	ret = send(g_PowerSock[type], buffer, ulStrLen, 0);
	if (ret <= 0)
	{
		printf("%s(Line:%d):type %d  .send command:%s error\n", __FUNCTION__, __LINE__, type, buffer);
		return ERROR;
	}
	//taskDelay(1);
	return OK;
}

/***********************************************************************
������:
ShareE_Close_SCPI_Instrument
������
�ر�SCPIָ��ͨ��
��ע��
***********************************************************************/
void ShareE_Close_SCPI_Instrument(int type)
{
	if (type != POWER_AGILENT5749_TYPE&&type != POWER_AGILENT6702_TYPE&&type != POWER_EA_TYPE&&type != POWER_SG_TYPE)
	{
		printf("%s(Line:%d):type %d is illegal ,only 0,1,2 available \n", __FUNCTION__, __LINE__, type);
	}

#ifdef _MSC_VER
	closesocket(g_PowerSock[type]);
#else
	close(g_PowerSock[type]);
#endif
	g_PowerSock[type] = -1;
}

//��ȡ��ԴulChannel�ĵ�ѹֵ
STATUS ShareE_PowerSupply_Get_Voltage(int type, unsigned long ulChannel, double *dValue)
{
	char SendBuffer[100];
	char *pStrStart = NULL;

	if (type != POWER_AGILENT6702_TYPE&&type != POWER_EA_TYPE&&type != POWER_AGILENT5749_TYPE)
	{
		printf("%s(Line:%d):type %d is illegal ,only 0,1,3 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}

	if (g_PowerSock[type] == 0)
	{
		*dValue = 0.0;
		printf("The SCPI channel is not Init\n");
		return ERROR;
	}

	switch (type)
	{
	case POWER_AGILENT6702_TYPE:
	case POWER_AGILENT5749_TYPE:
		sprintf(SendBuffer, "MEAS:VOLT? (@%u) \n", ulChannel);
		pStrStart = ShareE_SCPI_Request(POWER_AGILENT6702_TYPE, SendBuffer);//ִ��ָ���ȡͨ����ѹֵ
		if (pStrStart == NULL)
		{
			*dValue = 0.0;
			printf("Anglient The SCPI Command : %s  Failed\n", SendBuffer);
			return ERROR;
		}
		break;
	case POWER_EA_TYPE:
		sprintf(SendBuffer, "MEAS:VOLT? \n");
		pStrStart = ShareE_SCPI_Request(POWER_EA_TYPE, SendBuffer);//ִ��ָ���ȡͨ����ѹֵ
		if (pStrStart == NULL)
		{
			*dValue = 0.0;
			printf("EA The SCPI Command : %s  Failed\n", SendBuffer);
			return ERROR;
		}
		break;
	default:
		printf("%s(Line:%d):type %d is illegal ,only 0,1 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}

	*dValue = atof(pStrStart);
	return OK;
}

//��ȡ��ԴulChannel�ĵ���ֵ
STATUS ShareE_PowerSupply_Get_Current(int type, unsigned long ulChannel, double *dValue)
{
	char SendBuffer[100];
	char *pStrStart = NULL;

	if (type != POWER_AGILENT6702_TYPE&&type != POWER_EA_TYPE)
	{
		printf("%s(Line:%d):type %d is illegal ,only 0,1 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}
	if (g_PowerSock[type] == 0)
	{
		*dValue = 0.0;
		printf("The SCPI channel is not Init\n");
		return ERROR;
	}

	switch (type)
	{
	case POWER_AGILENT6702_TYPE:
		sprintf(SendBuffer, "MEAS:CURR? (@%u) \n", ulChannel);
		pStrStart = ShareE_SCPI_Request(POWER_AGILENT6702_TYPE, SendBuffer);//ִ��ָ���ȡͨ����ѹֵ
		if (pStrStart == NULL)
		{
			*dValue = 0.0;
			printf("Anglient The SCPI Command : %s  Failed\n", SendBuffer);
			return ERROR;
		}
		break;
	case POWER_EA_TYPE:
		sprintf(SendBuffer, "MEAS:CURR?\n");
		pStrStart = ShareE_SCPI_Request(POWER_EA_TYPE, SendBuffer);//ִ��ָ���ȡͨ����ѹֵ
		if (pStrStart == NULL)
		{
			*dValue = 0.0;
			printf("EA The SCPI Command : %s  Failed\n", SendBuffer);
			return ERROR;
		}
		break;
	default:
		printf("%s(Line:%d):type %d is illegal ,only 0,1 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}
	*dValue = atof(pStrStart);
	return OK;
}

//����channelͨ���ĵ�ѹ���ֵ�������ת������
STATUS ShareE_PowerSupply_Set_Voltage(int type, unsigned long ulChannel, double dValue, unsigned long SlewRate)
{
	int ret = 0;
	char SendBuffer[100];

	if (type != POWER_AGILENT6702_TYPE&&type != POWER_EA_TYPE&&type != POWER_AGILENT5749_TYPE)
	{
		printf("%s(Line:%d):type %d is illegal ,only 0,1,3 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}

	switch (type)
	{
	case POWER_AGILENT6702_TYPE:
		if (SlewRate == 0)
		{
			sprintf(SendBuffer, "VOLT:SLEW MAX,(@%u)\n VOLT %f,(@%u)\n", ulChannel, dValue, ulChannel);
		}
		else
		{
			sprintf(SendBuffer, "VOLT:SLEW %u,(@%u)\n VOLT %f,(@%u)\n", SlewRate, ulChannel, dValue, ulChannel);
		}
		ret = ShareE_SCPI_Write(POWER_AGILENT6702_TYPE, SendBuffer);//
		if (ret != OK)
		{
			printf("Anglient Power Supply SCPI CMD:%s ERROR\n", SendBuffer);
			return ERROR;
		}
		break;
	case POWER_EA_TYPE:
		sprintf(SendBuffer, "VOLT %f\n", dValue);
		ret = ShareE_SCPI_Write(POWER_EA_TYPE, SendBuffer);//
		if (ret != OK)
		{
			printf("EA Power Supply SCPI CMD:%s ERROR\n", SendBuffer);
			return ERROR;
		}
		break;
	case POWER_AGILENT5749_TYPE:
		if (SlewRate == 0)
		{
			sprintf(SendBuffer, "VOLT:SLEW MAX,(@%u)\n VOLT %f,(@%u)\n", ulChannel, dValue, ulChannel);
		}
		else
		{
			sprintf(SendBuffer, "VOLT:SLEW %u,(@%u)\n VOLT %f,(@%u)\n", SlewRate, ulChannel, dValue, ulChannel);
		}
		ret = ShareE_SCPI_Write(POWER_AGILENT5749_TYPE, SendBuffer);//
		if (ret != OK)
		{
			printf("Anglient5749 Power Supply SCPI CMD:%s ERROR\n", SendBuffer);
			return ERROR;
		}
		break;
	default:
		printf("%s(Line:%d):type %d is illegal ,only 0,1,3 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}

	return OK;
}

//����channelͨ���ĵ������ֵ�������ת������
STATUS ShareE_PowerSupply_Set_Current(int type, unsigned long ulChannel, double dValue)
{
	int ret = 0;
	char SendBuffer[100];

	if (type != POWER_AGILENT6702_TYPE&&type != POWER_EA_TYPE)
	{
		printf("%s(Line:%d):type %d is illegal ,only 0,1 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}

	switch (type)
	{
	case POWER_AGILENT6702_TYPE:
		sprintf(SendBuffer, "CURR %f,(@%u)\n", dValue, ulChannel);
		ret = ShareE_SCPI_Write(POWER_AGILENT6702_TYPE, SendBuffer);//
		if (ret != OK)
		{
			printf("Anglient Power Supply SCPI CMD:%s ERROR\n", SendBuffer);
			return ERROR;
		}
		break;
	case POWER_EA_TYPE:
		sprintf(SendBuffer, "CURR %f\n", dValue);
		ret = ShareE_SCPI_Write(POWER_EA_TYPE, SendBuffer);//
		if (ret != OK)
		{
			printf("EA Power Supply SCPI CMD:%s ERROR\n", SendBuffer);
			return ERROR;
		}
		break;
	default:
		printf("%s(Line:%d):type %d is illegal ,only 0,1 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}
	return OK;
}

//�򿪵�Դulchannelͨ��
STATUS ShareE_PowerSupply_Turnon(int type, unsigned long ulChannel)
{
	int ret = 0;
	char SendBuffer[100];

	if (type != POWER_AGILENT6702_TYPE&&type != POWER_EA_TYPE&&type != POWER_SG_TYPE&&type != POWER_AGILENT5749_TYPE)
	{
		printf("%s(Line:%d):type %d is illegal ,only 0,1,2,3 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}

	switch (type)
	{
	case POWER_AGILENT6702_TYPE:
		sprintf(SendBuffer, "OUTP ON,(@%u)\n", ulChannel);
		ret = ShareE_SCPI_Write(POWER_AGILENT6702_TYPE, SendBuffer);//
		if (ret != OK)
		{
			printf("Anglient Power Supply SCPI CMD:%s ERROR\n", SendBuffer);
			return ERROR;
		}
		break;
	case POWER_EA_TYPE:
		ret = ShareE_SCPI_Write(POWER_EA_TYPE, "OUTP ON\n");//
		if (ret != OK)
		{
			printf("EA Power Supply SCPI CMD:%s ERROR\n", SendBuffer);
			return ERROR;
		}
		break;
	case POWER_AGILENT5749_TYPE:
		sprintf(SendBuffer, "OUTP ON,(@%u)\n", ulChannel);
		ret = ShareE_SCPI_Write(POWER_AGILENT5749_TYPE, SendBuffer);//
		if (ret != OK)
		{
			printf("Anglient5749 Power Supply SCPI CMD:%s ERROR\n", SendBuffer);
			return ERROR;
		}
		break;
	case POWER_SG_TYPE:
		break;
	default:
		printf("%s(Line:%d):type %d is illegal ,only 0,1,2,3 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}
	return OK;
}

//�رյ�Դulchannelͨ��
STATUS ShareE_PowerSupply_Turnoff(int type, unsigned long ulChannel)
{
	int ret = 0;
	char SendBuffer[100];

	if (type != POWER_AGILENT6702_TYPE&&type != POWER_EA_TYPE&&type != POWER_AGILENT5749_TYPE)
	{
		printf("%s(Line:%d):type %d is illegal ,only 0,1,3 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}
	switch (type)
	{
	case POWER_AGILENT6702_TYPE:
		sprintf(SendBuffer, "OUTP OFF,(@%u)\n", ulChannel);
		ret = ShareE_SCPI_Write(POWER_AGILENT6702_TYPE, SendBuffer);//
		if (ret != OK)
		{
			printf("Anglient Power Supply SCPI CMD:%s ERROR\n", SendBuffer);
			return ERROR;
		}
		break;
	case POWER_EA_TYPE:
		sprintf(SendBuffer, "OUTP OFF\n");
		ret = ShareE_SCPI_Write(POWER_EA_TYPE, SendBuffer);//
		if (ret != OK)
		{
			printf("EA Power Supply SCPI CMD:%s ERROR\n", SendBuffer);
			return ERROR;
		}
		break;
	case POWER_AGILENT5749_TYPE:
		sprintf(SendBuffer, "OUTP OFF,(@%u)\n", ulChannel);
		ret = ShareE_SCPI_Write(POWER_AGILENT5749_TYPE, SendBuffer);//
		if (ret != OK)
		{
			printf("Anglient Power Supply SCPI CMD:%s ERROR\n", SendBuffer);
			return ERROR;
		}
		break;
	default:
		printf("%s(Line:%d):type %d is illegal ,only 0,1 available \n", __FUNCTION__, __LINE__, type);
		return ERROR;
	}
	return OK;
}

/************************SGϵ�д��ʵ�Դ***********************/
STATUS	Power_Init(int type)
{
	int ret = 0;
	char SendBuffer1[20];
	char SendBuffer2[20];
	sprintf(SendBuffer1, "*CLS\n");
	sprintf(SendBuffer2, "*RST\n");

	ret = ShareE_SCPI_Write(type, SendBuffer1);
	if (ret != TRUE)
	{
		printf("%s Power Supply SCPI CMD:%s ERROR\n", type, SendBuffer1);
		return ERROR;
	}
	taskDelay(1);
	ret = ShareE_SCPI_Write(type, SendBuffer2);
	if (ret != TRUE)
	{
		printf("%s Power Supply SCPI CMD:%s ERROR\n", type, SendBuffer2);
		return ERROR;
	}
	return TRUE;
}
STATUS	SGPower_Set_Voltage(double f8Voltage)
{
	int ret = 0;
	char SendBuffer[50];
	sprintf(SendBuffer, "SOUR:VOLT %.2f\n", f8Voltage);
	ret = ShareE_SCPI_Write(POWER_SG_TYPE, SendBuffer);
	if (ret != TRUE)
	{
		printf("Failed:Set voltage %.2f\n", f8Voltage);
		return ERROR;
	}
	return TRUE;
}
STATUS	SGPower_Set_Current(double f8Curent)
{
	int ret = 0;
	char SendBuffer[50];
	sprintf(SendBuffer, "SOUR:CURR %.2f\n", f8Curent);
	ret = ShareE_SCPI_Write(POWER_SG_TYPE, SendBuffer);
	if (ret != TRUE)
	{
		printf("Failed:Set current %.2f\n", f8Curent);
		return ERROR;
	}
	return TRUE;
}
