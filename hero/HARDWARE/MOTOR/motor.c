#include "motor.h"


u8 motor_Read[5][8];	
u8 canbuf[8];
int set_speed_PID[4];			//PID ���������׼�ٶ�					
int set_speed_Motor[4];		//��������ٶȣ���PID����ٶ�
int real_speed[4];				//������� �ٶ�ֵ����洢��
int motor_angle[4];				//������� ��е�Ƕ�ֵ����洢��

int set_Yaw_speed;				//��̨�����ٶ�����
u8 canbuf_yaw[8];					//can���� ��̨�Ƕ� ���Ʒ��ͻ�����
int Read_angle;						//��̨���� ʵ�ʻ�е�Ƕ�
int Set_angle;					//��̨���� Ŀ���е�Ƕ�

PIDtype MotorSpeedPID[4];
yawPid yaw_PID;

int fast_slow;
int shoot_speed;
u8 ready_shoot;
u8 friction_Running;

void ShooterMotorInit(void)
{
	int i;
	//�����ʼλ�� ( ǧ��ע�� ��
	Set_angle = 1800;      //��̨��ʼ��λ��
	TIM_SetCompare3(TIM8,1000);	//�������
	TIM_SetCompare4(TIM8,2000);	//������
	TIM_SetCompare1(TIM10,Set_angle);	//��̨�Ƕ�
	TIM_SetCompare1(TIM11,1500);	//���ֵ���
	//�������
	for(i = 0; i < 1000; i++)
	{
		TIM_SetCompare1(TIM2,1000);	//С������
		TIM_SetCompare3(TIM2,1000);	//СĦ����
		TIM_SetCompare4(TIM2,1000);	//СĦ����	
		
		TIM_SetCompare1(TIM8,1000);	//��Ħ����
		TIM_SetCompare2(TIM8,1000);	//��Ħ����
		
		IWDG_Feed();//ι��
		delay_ms(1);
	}
	
	fast_slow = 0;
	shoot_speed = 0;
	ready_shoot = 0;
	friction_Running = 0;
}

void setFrictionMotor(int n)
{
	if(n < 1000) n = 1000;
	if(n > 2000) n = 2000;
	TIM_SetCompare3(TIM2,n);	//СĦ����
	TIM_SetCompare4(TIM2,n);	//СĦ����	
}
void setFrictionMotor_Big(int n)
{
	if(n < 1000) n = 1000;
	if(n > 2000) n = 2000;
	TIM_SetCompare1(TIM8,n);	//��Ħ����
	TIM_SetCompare2(TIM8,n);	//��Ħ����
}

void setSelectMotor(int n)
{
	if(n < 1000) n = 1000;
	if(n > 2000) n = 2000;
	TIM_SetCompare1(TIM2,n);	//�޸ıȽ�ֵ���޸�ռ�ձ�
}
void add_bullet(int n) //0:��С��  1���Ӵ���
{
	if(n) TIM_SetCompare1(TIM11,2400);	//���ֵ���
	else TIM_SetCompare1(TIM11,1000);
}



void setShootMotor(void)
{
	if(key_HL) //�ر�����Ħ���֣�Ĭ�ϼ�С��
	{
		setFrictionMotor(1000);
		setFrictionMotor_Big(1000);
		add_bullet(0);
		friction_Running = 0;
	}
	if(key_CL) //����Ħ���֣���С��
	{
		setFrictionMotor(2500);
		setFrictionMotor_Big(1300);
		add_bullet(0);
		friction_Running = 1;
	}	
	if(key_OFF) //����Ħ���֣��Ӵ���
	{
		friction_Running = 1;
		setFrictionMotor(2500);
		setFrictionMotor_Big(1300);
		add_bullet(1);
	}		
	
	//����ѡ��
	if(key_ATTI_middle)
	{
		setSelectMotor(1000);
		fast_slow = 0;
	}
	if(key_ATTI_down && friction_Running == 1)
	{
		fast_slow++; if(fast_slow > 18) fast_slow = 0;
		if(fast_slow < 10)
		{
			setSelectMotor(1150);
		}	
		else 
		{
			setSelectMotor(1000);
		}
	}
	if(key_GPS && friction_Running == 1)
	{
		if(ready_shoot)
		{
			shoot_speed = 0;
			ready_shoot = 0;
		}	
	}
	
	if(ready_shoot == 0)
	{
		shoot_speed++; if(shoot_speed == 1000) shoot_speed = 500;
		if(shoot_speed < 25) //�������
		{
			TIM_SetCompare3(TIM8,700);	//�������
			TIM_SetCompare4(TIM8,1500);	//������,����				
		}
		else if(shoot_speed < 50) //�ȴ�
		{
			TIM_SetCompare3(TIM8,700);	//�������
			TIM_SetCompare4(TIM8,2000);	//������	
		}
		else if(shoot_speed < 80)
		{
			TIM_SetCompare3(TIM8,1000);	//�������������
			TIM_SetCompare4(TIM8,2000);	//������	
		}
		else if(shoot_speed < 105)
		{
			TIM_SetCompare3(TIM8,700);	//�������
			TIM_SetCompare4(TIM8,2000);	//������		
		}
		else
		{
			ready_shoot = 1;
		}
	}
}

void Motor_Init(void)
{
	int i;
	
	int yaw = 0;
	int pitch = 0;
	
	//���̵������̨����ٶ�����Ϊ0
	for(i = 0; i < 10; i++)
	{
		set_speed_PID[0] = set_speed_PID[1] = set_speed_PID[2] = set_speed_PID[3] = 0;
		canbuf[0]=set_speed_PID[0]/256; canbuf[1]=set_speed_PID[0]%256; 
		canbuf[2]=set_speed_PID[1]/256; canbuf[3]=set_speed_PID[1]%256; 
		canbuf[4]=set_speed_PID[2]/256; canbuf[5]=set_speed_PID[2]%256; 
		canbuf[6]=set_speed_PID[3]/256; canbuf[7]=set_speed_PID[3]%256;	
		CAN1_Send_Msg(canbuf,8);//����8���ֽ�		
		
		canbuf_yaw[0]=yaw/256; canbuf_yaw[1]=yaw%256; 
		canbuf_yaw[2]=pitch/256; canbuf_yaw[3]=pitch%256; 
		
		CAN1_Send_Msg_yaw(canbuf_yaw, 4);
		delay_ms(1);
	}	
	
	//PID init
	for(i = 0; i < 4; i++)
	{
		MotorSpeedPID[i].SetPoint = 0;	
		
		MotorSpeedPID[i].Proportion = 		1.1; 	
		MotorSpeedPID[i].Integral = 			0;	
		MotorSpeedPID[i].Derivative = 		0.2;	
		
		MotorSpeedPID[i].LastError = 0;		
		MotorSpeedPID[i].PrevError = 0;		
		MotorSpeedPID[i].SumError = 0;		
		MotorSpeedPID[i].outspeed = 0;
	}
	
	//��̨PID
	YawPID_Init(&yaw_PID);
	setYawPID(&yaw_PID, 2, 0, 0);	
	yaw_PID.setAngle = 4000;
	
//	i = 0;
//	MotorSpeedPID[i].Proportion = 		0.8; 	
//	MotorSpeedPID[i].Integral = 			0.01;	
//	MotorSpeedPID[i].Derivative = 		0.4;	
//	i = 1;
//	MotorSpeedPID[i].Proportion = 		0.8; 	
//	MotorSpeedPID[i].Integral = 			0.01;	
//	MotorSpeedPID[i].Derivative = 		0.4;	
//	i = 2;
//	MotorSpeedPID[i].Proportion = 		0.8; 	
//	MotorSpeedPID[i].Integral = 			0.01;	
//	MotorSpeedPID[i].Derivative = 		0.4;	
//	i = 3;
//	MotorSpeedPID[i].Proportion = 		0.8; 	
//	MotorSpeedPID[i].Integral = 			0.01;	
//	MotorSpeedPID[i].Derivative = 		0.4;	
		
}


void CMControlLoop()
{
	int x = Yao_R_LR;
	int y = Yao_R_UD;
	int yaw;
	
	int Send_Count;
	int i;
	int n = 15;
	int mouse_n = 50;
	int yao_n = 9;
	
	Send_Count = Send_Count;
	
	//��������ת��
	if(x==0 && y==0)
	{	
		set_speed_PID[0] = set_speed_PID[1] = set_speed_PID[2] = set_speed_PID[3] = 0;
	}
	else if((x>-1 && y>-1))//��һ����,  ��ǰ
	{	
		if(y >= x)	//ƫǰ
		{
			set_speed_PID[0]=set_speed_PID[2]=y;
			set_speed_PID[1]=set_speed_PID[3]=y-x;
		}
		else  			//ƫ��
		{
			set_speed_PID[0]=set_speed_PID[2]=x;			
			set_speed_PID[1]=set_speed_PID[3]=y-x;
		}
	}
	else if(x<1 && y>-1)//�ڶ����ޣ� ��ǰ
	{
		if(y >= (-x))//ƫǰ
		{
			set_speed_PID[1]=set_speed_PID[3]=y;
			set_speed_PID[0]=set_speed_PID[2]=y+x;
		}
		else   //ƫ��
		{
			set_speed_PID[1]=set_speed_PID[3]=(-x);
			set_speed_PID[0]=set_speed_PID[2]=y+x;
		}
	}
	else if(x<1 && y<1)//�������ޣ����
	{
		if(y >= x)//ƫ��
		{
			set_speed_PID[0]=set_speed_PID[2]=x;
			set_speed_PID[1]=set_speed_PID[3]=y-x;
		}
		else//ƫ��
		{
			set_speed_PID[0]=set_speed_PID[2]=y;
			set_speed_PID[1]=set_speed_PID[3]=y-x;
		}	
	}
	else if(x>-1 && y<-1)//�������ޣ��Һ�
	{
		if(x > (-y))//ƫ��
		{
			set_speed_PID[0]=set_speed_PID[2]=x+y;
			set_speed_PID[1]=set_speed_PID[3]=-x;
		}
		else//ƫ��
		{
			set_speed_PID[0]=set_speed_PID[2]=x+y;
			set_speed_PID[1]=set_speed_PID[3]=y;
		}	
	}
	else
	{
		set_speed_PID[0] = set_speed_PID[1] = set_speed_PID[2] = set_speed_PID[3] = 0;
	}	

	if(mode)//ң����������ģʽ
	{
		yaw = yaw * mouse_n;
	}
	else//ң����ģʽ
	{
		yaw = Yao_L_LR * yao_n;
	}
	
	//���ٱ�
	set_speed_PID[0] *= n; 
	set_speed_PID[1] *= n; 
	set_speed_PID[2] *= n; 
	set_speed_PID[3] *= n;
	
	//������ת��
	set_speed_PID[0] -= yaw; set_speed_PID[1] += yaw; set_speed_PID[2] += yaw; set_speed_PID[3] -= yaw;
	
	//������ת���������
	set_speed_PID[0] = -set_speed_PID[0]; 
	set_speed_PID[3] = -set_speed_PID[3];
	
	for(i = 0; i < 4; i++)
		MotorSpeedPID[i].SetPoint = set_speed_PID[i];	//�ٶ��趨
	
	//���յ������
	CAN1_Receive_Msg(motor_Read);
	CAN1_Receive_Msg(motor_Read);
	CAN1_Receive_Msg(motor_Read);
	CAN1_Receive_Msg(motor_Read);
	CAN1_Receive_Msg(motor_Read);		
	
	//��̨��е�Ƕȷ���ֵ����
	Read_angle = motor_Read[4][0]<<8 | motor_Read[4][1];
	
	yaw_PID.setAngle = 4000;
		
}


//void operation_yaw(int Read_yaw, int Set_yaw,  u8 arr_b[18][8], u8 chx[18], PIDtype *PIDx)
//{
//	int pid_end;
//	u8 canbuf_yaw[2];
//	PIDx->setpoint = Set_yaw;
//	pid_end=incPIDcalc(PIDx,Read_yaw);
//	printf("PID--------------------------------------------------------------------PID %d\r\n",pid_end);
//	printf("PID---------------------------------------------------------------Read_yaw %d\r\n",Read_yaw);
//	if(pid_end > 0) pid_end+=450;
//	else if(pid_end < 0) pid_end-=600;  //̧��
//	else pid_end = 0;
//	if(pid_end > 1000) pid_end = 1000;
//	else if(pid_end < -1000) pid_end = -1000;
//	else pid_end = pid_end;
//	canbuf_yaw[0]=pid_end/256;
//	canbuf_yaw[1]=pid_end%256;
//	CAN1_Send_Msg_yaw(canbuf_yaw,2);//����8���ֽ�	
	
//}

void stopAllMotor(void)
{
	int i;
	for(i = 0; i < 8; i++)
	{
		canbuf[i] = 0;
	}
	CAN1_Send_Msg(canbuf,8);//����8���ֽ�
	
	TIM_SetCompare1(TIM2,1000);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	TIM_SetCompare3(TIM2,1000);	//�޸ıȽ�ֵ���޸�ռ�ձ�
	TIM_SetCompare4(TIM2,1000);	//�޸ıȽ�ֵ���޸�ռ�ձ�	
}


void setAll_PWM(void)
{
	
}
