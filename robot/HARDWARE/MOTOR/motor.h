#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "main.h"

extern u8 motor_Read[5][8];	
extern u8 canbuf[8];
extern int set_speed_PID[4];		//PID ���������׼�ٶ�					
extern int set_speed_Motor[4];	//��������ٶȣ���PID����ٶ�
extern int real_speed[4];				//������� �ٶ�ֵ����洢��
extern int motor_angle[4];			//������� ��е�Ƕ�ֵ����洢��

extern int set_Yaw_speed;				//��̨�����ٶ�����
extern u8 canbuf_yaw[8];				//can���� ��̨�Ƕ� ���Ʒ��ͻ�����
extern int Read_angle;					//��̨���� ʵ�ʻ�е�Ƕ�
extern int Set_angle;						//��̨���� Ŀ���е�Ƕ�
				

void ShooterMotorInit(void);
void setFrictionMotor(int n);
void setSelectMotor(int n);
void setShootMotor(void);
void Motor_Init(void);
void CMControlLoop(void);
void stopAllMotor(void);
void setAll_PWM(void);





#endif





