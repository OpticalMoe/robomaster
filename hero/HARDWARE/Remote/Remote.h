#ifndef __REMOTE_H
#define __REMOTE_H	 
#include "main.h"

#define MIDDLE 1024

//ң�����ⲿ������
extern u8 chx[18]; 					//˳��洢��18�ֽ�ң��������ԭ��
extern u8 chx_b[18][8];			//2����ԭ��洢
extern int Yao_R_UD;    //�Ҳ�ҡ�˴�������
extern int Yao_R_LR;
extern int Yao_L_UD;    //���ҡ�˴�������
extern int Yao_L_LR;
extern u8 key_OFF;
extern u8 key_CL;
extern u8 key_HL;
extern u8 key_GPS;
extern u8 key_ATTI_middle;
extern u8 key_ATTI_down;
extern int mouse_up_down_speed;
extern int mouse_lift_right_speed;
extern int mouse_lift_key;
extern int mouse_right_key;
extern int keyboard[16];
extern char keyboard_key[16];
extern u8 mode;


void RemoteInit(void);
void GetRemoteData(void);


#endif



