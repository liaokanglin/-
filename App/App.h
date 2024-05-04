#ifndef __APP_H__
#define __APP_H__

#include "config.h"

#define TASK_KEY 				1
#define TASK_TRACK 			2
#define TASK_UART1 			3
#define TASK_BLUETOOTH 	    4
#define TASK_ULTRA			5
#define TASK_BATTERY		6

// ȫ�־�����Ϣ
extern float global_distance_cm;

// ������๦���Ƿ�����1������0�ر�
extern u8 is_ultrasonic_enable;

// ---------------------------- ��������

// �Ƿ�͵���
extern u8 isLowPower();

// ������ģ���˯���л���
extern void bt_enable();

#endif