#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "main.h"

extern u8 motor_Read[5][8];	
extern u8 canbuf[8];
extern int set_speed_PID[4];		//PID 计算输入标准速度					
extern int set_speed_Motor[4];	//电机设置速度，即PID输出速度
extern int real_speed[4];				//电机反馈 速度值计算存储区
extern int motor_angle[4];			//电机反馈 机械角度值计算存储区

extern int set_Yaw_speed;				//云台运行速度设置
extern u8 canbuf_yaw[8];				//can总线 云台角度 控制发送缓存区
extern int Read_angle;					//云台反馈 实际机械角度
extern int Set_angle;						//云台设置 目标机械角度
				

void ShooterMotorInit(void);
void setFrictionMotor(int n);
void setSelectMotor(int n);
void setShootMotor(void);
void Motor_Init(void);
void CMControlLoop(void);
void stopAllMotor(void);
void setAll_PWM(void);





#endif





