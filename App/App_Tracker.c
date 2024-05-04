#include "App.h"
#include "Tracker.h"
#include "Motors.h"



void task_tracker() _task_ TASK_TRACK {
//  u8 states[3];
//  u8 i;
  int pos, last_pos;
  // ���ϼ����� 0 0 0 \r\n
  Tracker_init();
	
  while(1) {
//		Tracker_scan(states);
//		for(i = 0; i < 3; i++){
//			printf("%d ", (int)states[i]);
//		}
//		printf("\r\n");
    // �ϰ����ж�
    // �͵����ж�

    pos = Tracker_get_position();
    if(pos == last_pos) {
      os_wait2(K_TMO, 2);// 10ms = 5ms * 2
      // ����λ��û�����仯��ֱ�ӷ���
      continue;
    }
    // ��¼���һ��pos
    last_pos = pos;

    // -32, -16, 0, 16, 32
    if (pos < 0) {
      Motors_around(40, 0);
//      printf("pos: %d -> %s\r\n", (int)pos, "��ת");
    } else if(pos > 0) {
      Motors_around(40, 1);
//      printf("pos: %d -> %s\r\n", (int)pos, "��ת");
    } else {
      Motors_forward(60);
//      printf("pos: %d -> %s\r\n", (int)pos, "ֱ��");
    }
//    os_wait2(K_TMO, 40);// 200ms = 5ms * 40
//    os_wait2(K_TMO, 4);// 20ms = 5ms * 4
    os_wait2(K_TMO, 2);// 10ms = 5ms * 2
  }
}