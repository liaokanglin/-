
#ifndef		__CONFIG_H
#define		__CONFIG_H

//========================================================================
//                               ��ʱ�Ӷ���
//========================================================================

#define MAIN_Fosc		24000000L	//������ʱ��
//#define MAIN_Fosc		22118400L	//������ʱ��
//#define MAIN_Fosc		12000000L	//������ʱ��
//#define MAIN_Fosc		11059200L	//������ʱ��
//#define MAIN_Fosc		 5529600L	//������ʱ��

#define	NULL	0

//========================================================================
//                                ͷ�ļ�
//========================================================================

#include "STC8H.H"
#include "RTX51TNY.H"
#include <intrins.h>
#include <stdlib.h>
#include <stdio.h>

//========================================================================
//                               ���Ͷ���
//========================================================================

typedef unsigned char   u8;     //  8 bits 
typedef unsigned int    u16;    // 16 bits 
typedef unsigned long   u32;    // 32 bits 

typedef signed char     int8;   //  8 bits 
typedef signed int      int16;  // 16 bits 
typedef signed long     int32;  // 32 bits 

typedef unsigned char   uint8;  //  8 bits 
typedef unsigned int    uint16; // 16 bits 
typedef unsigned long   uint32; // 32 bits 

//===================================================

#define	TRUE	1
#define	FALSE	0

//===================================================

#define	Priority_0			0	//�ж����ȼ�Ϊ 0 ������ͼ���
#define	Priority_1			1	//�ж����ȼ�Ϊ 1 �����ϵͼ���
#define	Priority_2			2	//�ж����ȼ�Ϊ 2 �����ϸ߼���
#define	Priority_3			3	//�ж����ȼ�Ϊ 3 ������߼���

#define ENABLE		1
#define DISABLE		0

#define SUCCESS		0
#define FAIL		-1


//===================================================

#define	I2C_Mode_Master			1
#define	I2C_Mode_Slave			0

#define	PIE			0x20	//1: �ȽϽ����0��1, �����������ж�
#define	NIE			0x10	//1: �ȽϽ����1��0, �����½����ж�

#define	PWMA	128
#define	PWMB	129

#define	FALLING_EDGE		1		//�����½����ж�
#define	RISING_EDGE			2		//�����������ж�
//===================================================


//========================================================================
//                             �ⲿ�����ͱ�������
//========================================================================

#endif