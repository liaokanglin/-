#ifndef __TRACKER_H__
#define __TRACKER_H__

#include "config.h"
#include "GPIO.h"

#define TRACK_1 	P00//��ת
#define TRACK_2 	P01//ֱ��
#define TRACK_3  	P02//��ת

#define TRACKER_GPIO_INIT() P0_MODE_IO_PU(GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);

void Tracker_init();

// ɨ�赱ǰ���е�״̬
// 1: ���ߣ���������û�յ��⣺ָʾ����
// 0: ���棬���������յ����ˣ�ָʾ����
void Tracker_scan(u8 states[]);

// ��ȡ��λ��
int Tracker_get_position();


#endif