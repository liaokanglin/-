#include "App.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Buzzer.h"
#include "Light.h"
#include "Motors.h"

#define BT_EN	P40
void bt_enable(){
	// ����ʹ�� �������½��ؼ��ɻ�������ģ�飩
	BT_EN = 1;
	os_wait2(K_TMO, 1);
	BT_EN = 0;
}

static void GPIO_config(){
  GPIO_InitTypeDef	GPIO_InitStructure;		//�ṹ����

  // UART2
  GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//ָ��Ҫ��ʼ����IO,
  GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
  GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//��ʼ��

  // BT_EN
  GPIO_InitStructure.Pin  = GPIO_Pin_0;		//ָ��Ҫ��ʼ����IO,
  GPIO_InitStructure.Mode = GPIO_PullUp;	//ָ��IO������������ʽ,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
  GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//��ʼ��
}

void UART2_config(void) {
  COMx_InitDefine		COMx_InitStructure;					//�ṹ����
  COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//ģʽ, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
  COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//ѡ�����ʷ�����, BRT_Timer1, BRT_Timer2 (ע��: ����2�̶�ʹ��BRT_Timer2)
  COMx_InitStructure.UART_BaudRate  = 9600ul;			//������, һ�� 110 ~ 115200
  COMx_InitStructure.UART_RxEnable  = ENABLE;				//��������,   ENABLE��DISABLE
  COMx_InitStructure.BaudRateDouble = DISABLE;			//�����ʼӱ�, ENABLE��DISABLE
  UART_Configuration(UART2, &COMx_InitStructure);		//��ʼ������1 UART1,UART2,UART3,UART4

  NVIC_UART2_Init(ENABLE,Priority_1);		//�ж�ʹ��, ENABLE/DISABLE; ���ȼ�(�͵���) Priority_0,Priority_1,Priority_2,Priority_3
  UART2_SW(UART2_SW_P10_P11);		// ����ѡ��, UART2_SW_P10_P11,UART2_SW_P46_P47
}
#define BUTTON_A 	4
#define BUTTON_B 	5
#define BUTTON_C 	6
#define BUTTON_D 	7

#define IS_PRESSED(btn) (RX2_Buffer[btn] == 0x01)
/************************
 0	1	 2  3  4  5  6  7  ��������
 ֡ͷ  x  y  A  B  C  D
DD 77 EF 50 01 00 01 00  ����ʾ��

֡ͷ 01��DD 77
���� 23��EF 50
��ť 4567: ABCD  ����01 ̧��00

A: ������/����
B: ����ת: ���¿�ʼת,̧��ֹͣת
C: ����ת: ���¿�ʼת,̧��ֹͣת
D: ����/�ر�Ѳ��
**************************/
u8 is_turning 	= FALSE; 	// �Ƿ����ڵ�ͷ��ת   1
u8 is_tracking 	= FALSE; 	// �Ƿ�����Ѳ��  0
void do_control_action(u8* array, u8 len){
	static char x, y;
	u8 i;
	if(len < 8){
		return;
	}
	// �����յ����������ݣ��ֽ����飩����С�����п���
//	printf("data: %02x\n", (int)array[4]);
	printf("-> ");
	for(i = 0; i < 8; i++){
		printf("%02X ", (int)array[i]);
	}
	printf("\n");
	
	if(array[0] != 0xDD){
		return;
	}
	if(array[1] != 0x77){
		return;
	}
	
	// ----------------------------------------- ��ťA�ж�
	if(array[BUTTON_A] == 0x01){
		Light_on(ALL);
		Buzzer_play(2, 200);
		Light_off(ALL);
		
		// �����ر��ϰ����ж��߼���������ģ�飩
		is_ultrasonic_enable = !is_ultrasonic_enable;
		return;
	}
	
	// �͵�����ֹ���������˶�
	if(isLowPower()){
		Motors_stop();
		return;
	}
	
	// ----------------------------------------- ��ťBC��ת�ж�
	if(array[BUTTON_B] == 0x01){// ����ת
		Motors_around(25, 0);
		is_turning = TRUE;
		return;
	}else if(array[BUTTON_C] == 0x01){ // ����ת
		Motors_around(25, 1);
		is_turning = TRUE;
		return;
	}else if (is_turning){	// ֹͣ��ת
		is_turning = FALSE;
		Motors_stop();
		return;
	}
	
	// -------------------------------------------��ťD �����ر�Ѳ��
	if(array[BUTTON_D] == 0x01){
		if(is_tracking) {
			// �ڶ��ΰ��£�ֹͣѲ��
			Motors_stop();
			os_delete_task(TASK_TRACK);
			printf("����Ѳ��\n");
		} else {
			// ��һ�ΰ��£�Ѳ��
			os_create_task(TASK_TRACK);
			printf("��ʼѲ��\n");
		}
		is_tracking = !is_tracking;
		return;
	}
	// -------------------------------------------- ҡ�˿����ƶ�
	x = array[2];
	y = array[3];
	
	printf("x->%d,\ty->%d\r\n", (int)x, (int)y);
	
	Motors_move((int)x, (int)y);
}

void do_control(){
	u8 i;
	u8 total = COM2.RX_Cnt;
	// ���COM2.RX_Cnt == 8
	// ���COM2.RX_Cnt == 16
	
	// DD 77 EF 50 01 00 01 00 DD 77 00 00 01 00 01 00
	// DD 77 EF 50 01 00 01 00 DD 77 00 00  ==> 8 + 4 = 12
	// DD 77 EF 50 01 00 01 00 DD 77 00 00 01 00 01 00 DD 77 3A 4B
	
	for(i = 0; i < COM2.RX_Cnt - 1; i++){
		// ���������ֵ� DD 77�� ��ȡ��ߵ�6���ֽڣ�һ��8���ֽ�
		if(RX2_Buffer[i] == 0xDD && RX2_Buffer[i+1] == 0x77){
			
			// i==8, i+1==9 ����ĩβ���ݲ�����������Բ����ǣ�
			if(COM2.RX_Cnt - i < 8){ // 12 - 8 == 4
				// ʣ������ < 8ֱ�ӽ���ѭ��
				break;
			}
			
			// ��DD��ʼ��ȡ��8���ֽ�
			do_control_action(&RX2_Buffer[i], 8);
			i += 7;
		}
	}
}

// 2. UART2�������񣺰�UART2�������յ�������ת����UART1����PC
void task_bluetooth() _task_ TASK_BLUETOOTH {
	u8 i;
	GPIO_config();
	UART2_config();
	
  while(1) {
    if(COM2.RX_TimeOut > 0) {
      //��ʱ����
      if(--COM2.RX_TimeOut == 0) {
        if(COM2.RX_Cnt > 0) {
					
					// �ڴ˺����ڲ������յ������ݽ��зְ�����
					do_control();
					
          for(i=0; i<COM2.RX_Cnt; i++)	{
            // RX2_Buffer[i]����ǽ��յ����ݣ�д���� TX2_write2buff
            // TODO: ��������߼� on_uart2_recv
						TX1_write2buff(RX2_Buffer[i]); // ת����PC
          }
        }
        COM2.RX_Cnt = 0;
      }
    }
		
    // ��Ҫ�����̫��
    os_wait2(K_TMO, 2); // 10ms
  }
}