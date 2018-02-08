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


/*�����׵�Դ��IP��Port*/
#define AGLIENT_POWER_SERVER		"192.168.138.163"
#define AGLIENT_POWER_SVRPORT		5025

/*������5749��Դ��IP��Port*/
#define AGLIENT5749_POWER_SERVER	"192.168.138.164"
#define AGLIENT5749_POWER_SVRPORT	5025

/*EA��Դ��IP��Port*/
#define EA_POWER_SERVER				"192.168.0.41"
#define EA_POWER_SVRPORT			1001

/*SG���ʵ�Դ��IP��Port*/
#define SG_POWER_SERVER				"192.168.138.165"
#define SG_POWER_SVRPORT			9221


#define POWER_AGILENT6702_TYPE  0   /*�����׵�Դ*/
#define POWER_EA_TYPE		1	/*EA��Դ*/
#define POWER_SG_TYPE		2	/*SG���ʵ�Դ*/
#define POWER_AGILENT5749_TYPE  3   /*�����׵�Դ*/

#define POWER_AGILENT_MAX_VOLT  34.0	/*�����׵�Դ��ѹ���ֵ*/
#define POWER_AGILENT_MAX_CURE  5.0	/*�����׵�Դ�������ֵ*/

#define POWER_EA_MAX_VOLT		64.0 /*EA��Դ��ѹ���ֵ*/
#define POWER_EA_MAX_CURE		150.0	/*EA��Դ�������ֵ*/

typedef int STATUS;
#define OK 1

#ifdef __cplusplus
extern "C"
{
#endif

RTDLL1_API STATUS ShareE_Open_SCPI_Instrument(int type, const char* lpszIP); //��SCPIͨ����������������������֮ǰ���ô˺���
RTDLL1_API char * ShareE_SCPI_Request(int type,char *RequstBuffer);//����SCPIָ�������Ӧ��
RTDLL1_API STATUS ShareE_SCPI_Write(int type,char *buffer);//����SCPIָ��
RTDLL1_API STATUS ShareE_SCPI_Read(int type,double* dValue,unsigned long ulUsSecond);//����SCPIָ��ķ���ֵ
RTDLL1_API STATUS ShareE_SCPI_ReadStr(int type,char * reply,unsigned long ulUsSecond);

RTDLL1_API STATUS ShareE_PowerSupply_Get_Voltage(int type, unsigned long ulChannel,double *dValue);//��ȡ��ԴulChannel�ĵ�ѹֵ
RTDLL1_API STATUS ShareE_PowerSupply_Get_Current(int type, unsigned long ulChannel,double *dValue);//��ȡ��ԴulChannel�ĵ���ֵ
RTDLL1_API STATUS ShareE_PowerSupply_Set_Voltage(int type, unsigned long ulChannel,double dValue,unsigned long SlewRate);//����channelͨ���ĵ�ѹ���ֵ�������ת������
RTDLL1_API STATUS ShareE_PowerSupply_Set_Current(int type, unsigned long ulChannel,double dValue);//����channelͨ���ĵ������ֵ��ע�ⲻ������ת������
RTDLL1_API STATUS ShareE_PowerSupply_Turnon(int type,unsigned long ulChannel);//�򿪵�Դulchannelͨ��
RTDLL1_API STATUS ShareE_PowerSupply_Turnoff(int type,unsigned long ulChannel);//�رյ�Դulchannelͨ��

RTDLL1_API void ShareE_Close_SCPI_Instrument(int type);//�ر�SCPIָ��ͨ��

/************************SGϵ�д��ʵ�Դ***********************/
RTDLL1_API STATUS	Power_Init(int type);
RTDLL1_API STATUS	SGPower_Set_Voltage(double f8Voltage);
RTDLL1_API STATUS	SGPower_Set_Current(double f8Curent);

#ifdef __cplusplus
}
#endif

#endif

 
