#include "Tracker.h"


void Tracker_init(){

		// ��ʼ��GPIO
	TRACKER_GPIO_INIT();
}

// ɨ�赱ǰ���е�״̬
// 1: ���ߣ���������û�յ��⣺ָʾ����
// 0: ���棬���������յ����ˣ�ָʾ����
void Tracker_scan(u8 states[]){
	states[0] = TRACK_1;
	states[1] = TRACK_2;
	states[2] = TRACK_3;
}


// ��ȡ��λ��
// 1�ߵ�ƽ: ���ߣ�ָʾ���� (��������û�յ���)
// 0�͵�ƽ: ���棬ָʾ���������������յ����ˣ�
int Tracker_get_position(){
	
	int sum = 0, count = 0, pos = 0;
	// ��¼��һ�ε�λ��, ֻ��ʼ��1��
	static int last_pos = 0;
	
	if(TRACK_1 == 1){
		sum += -32;
		count++;
	}
	if(TRACK_2 == 1){
		sum += 0;
		count++;
	}
	if(TRACK_3 == 1){
		sum += 32;
		count++;
	}
	
	// û�е�ѹ������
	if(count == 0){
		// ״̬��ʧ������һ��״̬Ϊ׼
		return last_pos;
	}
	
	pos = sum / count;
	
	// �ѵ�ǰֵ��¼������������һ��ûѹ����ʱ��������
	last_pos = pos;
	
	return pos;
}



