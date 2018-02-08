#ifndef __SCPI_UTIL_H__
#define __SCPI_UTIL_H__

#if defined UNDER_RTSS
  #if defined SE_POWER_EXPORTS
    #define RTDLL1_API __declspec(dllexport)
  #else
    #define RTDLL1_API __declspec(dllimport)
  #endif
#else
  #if defined DLL64_EXPORTS
    #define RTDLL1_API __declspec(dllexport)
  #else
    #define RTDLL1_API __declspec(dllimport)
  #endif
#endif


/*安捷伦电源的IP及Port*/
#define AGLIENT_POWER_SERVER		"192.168.138.163"
#define AGLIENT_POWER_SVRPORT		5025

/*安捷伦5749电源的IP及Port*/
#define AGLIENT5749_POWER_SERVER	"192.168.138.164"
#define AGLIENT5749_POWER_SVRPORT	5025

/*EA电源的IP及Port*/
#define EA_POWER_SERVER				"192.168.0.41"
#define EA_POWER_SVRPORT			1001

/*SG大功率电源的IP及Port*/
#define SG_POWER_SERVER				"192.168.138.165"
#define SG_POWER_SVRPORT			9221


#define POWER_AGILENT6702_TYPE  0   /*安捷伦电源*/
#define POWER_EA_TYPE		1	/*EA电源*/
#define POWER_SG_TYPE		2	/*SG大功率电源*/
#define POWER_AGILENT5749_TYPE  3   /*安捷伦电源*/

#define POWER_AGILENT_MAX_VOLT  34.0	/*安捷伦电源电压最大值*/
#define POWER_AGILENT_MAX_CURE  5.0	/*安捷伦电源电流最大值*/

#define POWER_EA_MAX_VOLT		64.0 /*EA电源电压最大值*/
#define POWER_EA_MAX_CURE		150.0	/*EA电源电流最大值*/

typedef int STATUS;
#define OK 1

#ifdef __cplusplus
extern "C"
{
#endif

RTDLL1_API STATUS ShareE_Open_SCPI_Instrument(int type, const char* lpszIP); //打开SCPI通道，在以下其他函数调用之前调用此函数
RTDLL1_API char * ShareE_SCPI_Request(int type,char *RequstBuffer);//发送SCPI指令，并接受应答
RTDLL1_API STATUS ShareE_SCPI_Write(int type,char *buffer);//发送SCPI指令
RTDLL1_API STATUS ShareE_SCPI_Read(int type,double* dValue,unsigned long ulUsSecond);//接收SCPI指令的返回值
RTDLL1_API STATUS ShareE_SCPI_ReadStr(int type,char * reply,unsigned long ulUsSecond);

RTDLL1_API STATUS ShareE_PowerSupply_Get_Voltage(int type, unsigned long ulChannel,double *dValue);//获取电源ulChannel的电压值
RTDLL1_API STATUS ShareE_PowerSupply_Get_Current(int type, unsigned long ulChannel,double *dValue);//获取电源ulChannel的电流值
RTDLL1_API STATUS ShareE_PowerSupply_Set_Voltage(int type, unsigned long ulChannel,double dValue,unsigned long SlewRate);//设置channel通道的电压输出值，及输出转换速率
RTDLL1_API STATUS ShareE_PowerSupply_Set_Current(int type, unsigned long ulChannel,double dValue);//设置channel通道的电流输出值，注意不能设置转换速率
RTDLL1_API STATUS ShareE_PowerSupply_Turnon(int type,unsigned long ulChannel);//打开电源ulchannel通道
RTDLL1_API STATUS ShareE_PowerSupply_Turnoff(int type,unsigned long ulChannel);//关闭电源ulchannel通道

RTDLL1_API void ShareE_Close_SCPI_Instrument(int type);//关闭SCPI指令通道

/************************SG系列大功率电源***********************/
RTDLL1_API STATUS	Power_Init(int type);
RTDLL1_API STATUS	SGPower_Set_Voltage(double f8Voltage);
RTDLL1_API STATUS	SGPower_Set_Current(double f8Curent);

#ifdef __cplusplus
}
#endif

#endif

 
