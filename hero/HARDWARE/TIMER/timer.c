#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

extern int set_speed_Motor[4];
extern int real_speed[4];	
extern PIDtype MotorSpeedPID[4];
extern yawPid yaw_PID;
extern u8 canbuf[8];
extern u8 motor_Read[5][8];
extern int Set_angle;	
float Set_angle_f;
int i;

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{		
		//���̵����������
		for(i = 0; i < 4; i++)
		{
			real_speed[i] = motor_Read[i][2]<<8 | motor_Read[i][3];
			if(real_speed[i] > 32767)
			{
				real_speed[i]=real_speed[i]-65536;
			}				
		}
		
		set_speed_Motor[0]+=IncPIDCalc(&MotorSpeedPID[0],real_speed[0]);
		set_speed_Motor[1]+=IncPIDCalc(&MotorSpeedPID[1],real_speed[1]);
		set_speed_Motor[2]+=IncPIDCalc(&MotorSpeedPID[2],real_speed[2]);
		set_speed_Motor[3]+=IncPIDCalc(&MotorSpeedPID[3],real_speed[3]);

		canbuf[0]=set_speed_Motor[0]/256; canbuf[1]=set_speed_Motor[0]%256; 
		canbuf[2]=set_speed_Motor[1]/256; canbuf[3]=set_speed_Motor[1]%256; 
		canbuf[4]=set_speed_Motor[2]/256; canbuf[5]=set_speed_Motor[2]%256; 
		canbuf[6]=set_speed_Motor[3]/256; canbuf[7]=set_speed_Motor[3]%256; 
		CAN1_Send_Msg(canbuf,8);//����8���ֽ�		

		LED1 = !LED1;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}



void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	Set_angle_f = 1800;
	Set_angle = 1800;
}

//��ʱ��4�жϷ�����
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{	
		Set_angle_f -= 0.1*Yao_L_UD;
		if(Set_angle_f < 2260 && Set_angle_f > 840)
		{
			Set_angle = (int)(Set_angle_f);
		}
		else Set_angle_f = Set_angle;
	}
	TIM_SetCompare1(TIM10,Set_angle);	//��̨�Ƕ�

	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ
}



