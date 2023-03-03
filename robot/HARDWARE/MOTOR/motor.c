#include "motor.h"


u8 motor_Read[5][8];	
u8 canbuf[8];
int set_speed_PID[4];			//PID 计算输入标准速度					
int set_speed_Motor[4];		//电机设置速度，即PID输出速度
int real_speed[4];				//电机反馈 速度值计算存储区
int motor_angle[4];				//电机反馈 机械角度值计算存储区

int set_Yaw_speed;				//云台运行速度设置
u8 canbuf_yaw[8];					//can总线 云台角度 控制发送缓存区
int Read_angle;						//云台反馈 实际机械角度
int Set_angle;					//云台设置 目标机械角度

PIDtype MotorSpeedPID[4];
yawPid yaw_PID;

int fast_slow;
int shoot_speed;
u8 ready_shoot;
u8 friction_Running;

void ShooterMotorInit(void)
{
	int i;
	//舵机初始位置 ( 千万注意 ）
	Set_angle = 1800;      //云台初始化位置
	TIM_SetCompare3(TIM8,1000);	//补单舵机
	TIM_SetCompare4(TIM8,2000);	//发射舵机
	TIM_SetCompare1(TIM10,Set_angle);	//云台角度
	TIM_SetCompare1(TIM11,1500);	//弹仓挡板
	//电调解锁
	for(i = 0; i < 1000; i++)
	{
		TIM_SetCompare1(TIM2,1000);	//小拨弹轮
		TIM_SetCompare3(TIM2,1000);	//小摩擦轮
		TIM_SetCompare4(TIM2,1000);	//小摩擦轮	
		
		TIM_SetCompare1(TIM8,1000);	//大摩擦轮
		TIM_SetCompare2(TIM8,1000);	//大摩擦轮
		
		IWDG_Feed();//喂狗
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
	TIM_SetCompare3(TIM2,n);	//小摩擦轮
	TIM_SetCompare4(TIM2,n);	//小摩擦轮	
}
void setFrictionMotor_Big(int n)
{
	if(n < 1000) n = 1000;
	if(n > 2000) n = 2000;
	TIM_SetCompare1(TIM8,n);	//大摩擦轮
	TIM_SetCompare2(TIM8,n);	//大摩擦轮
}

void setSelectMotor(int n)
{
	if(n < 1000) n = 1000;
	if(n > 2000) n = 2000;
	TIM_SetCompare1(TIM2,n);	//修改比较值，修改占空比
}
void add_bullet(int n) //0:加小球  1：加大球
{
	if(n) TIM_SetCompare1(TIM11,2400);	//弹仓挡板
	else TIM_SetCompare1(TIM11,1000);
}



void setShootMotor(void)
{
	if(key_HL) //关闭所有摩擦轮，默认加小球
	{
		setFrictionMotor(1000);
		setFrictionMotor_Big(1000);
		add_bullet(0);
		friction_Running = 0;
	}
	if(key_CL) //开启摩擦轮，加小球
	{
		setFrictionMotor(2500);
		setFrictionMotor_Big(1300);
		add_bullet(0);
		friction_Running = 1;
	}	
	if(key_OFF) //开启摩擦轮，加大球
	{
		friction_Running = 1;
		setFrictionMotor(2500);
		setFrictionMotor_Big(1300);
		add_bullet(1);
	}		
	
	//发射选项
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
		if(shoot_speed < 25) //射击动作
		{
			TIM_SetCompare3(TIM8,700);	//补单舵机
			TIM_SetCompare4(TIM8,1500);	//发射舵机,发射				
		}
		else if(shoot_speed < 50) //等待
		{
			TIM_SetCompare3(TIM8,700);	//补单舵机
			TIM_SetCompare4(TIM8,2000);	//发射舵机	
		}
		else if(shoot_speed < 80)
		{
			TIM_SetCompare3(TIM8,1000);	//补单舵机，补弹
			TIM_SetCompare4(TIM8,2000);	//发射舵机	
		}
		else if(shoot_speed < 105)
		{
			TIM_SetCompare3(TIM8,700);	//补单舵机
			TIM_SetCompare4(TIM8,2000);	//发射舵机		
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
	
	//底盘电机、云台电机速度设置为0
	for(i = 0; i < 10; i++)
	{
		set_speed_PID[0] = set_speed_PID[1] = set_speed_PID[2] = set_speed_PID[3] = 0;
		canbuf[0]=set_speed_PID[0]/256; canbuf[1]=set_speed_PID[0]%256; 
		canbuf[2]=set_speed_PID[1]/256; canbuf[3]=set_speed_PID[1]%256; 
		canbuf[4]=set_speed_PID[2]/256; canbuf[5]=set_speed_PID[2]%256; 
		canbuf[6]=set_speed_PID[3]/256; canbuf[7]=set_speed_PID[3]%256;	
		CAN1_Send_Msg(canbuf,8);//发送8个字节		
		
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
	
	//云台PID
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
	
	//四轮理论转速
	if(x==0 && y==0)
	{	
		set_speed_PID[0] = set_speed_PID[1] = set_speed_PID[2] = set_speed_PID[3] = 0;
	}
	else if((x>-1 && y>-1))//第一象限,  右前
	{	
		if(y >= x)	//偏前
		{
			set_speed_PID[0]=set_speed_PID[2]=y;
			set_speed_PID[1]=set_speed_PID[3]=y-x;
		}
		else  			//偏右
		{
			set_speed_PID[0]=set_speed_PID[2]=x;			
			set_speed_PID[1]=set_speed_PID[3]=y-x;
		}
	}
	else if(x<1 && y>-1)//第二象限， 左前
	{
		if(y >= (-x))//偏前
		{
			set_speed_PID[1]=set_speed_PID[3]=y;
			set_speed_PID[0]=set_speed_PID[2]=y+x;
		}
		else   //偏左
		{
			set_speed_PID[1]=set_speed_PID[3]=(-x);
			set_speed_PID[0]=set_speed_PID[2]=y+x;
		}
	}
	else if(x<1 && y<1)//第三象限，左后
	{
		if(y >= x)//偏左
		{
			set_speed_PID[0]=set_speed_PID[2]=x;
			set_speed_PID[1]=set_speed_PID[3]=y-x;
		}
		else//偏后
		{
			set_speed_PID[0]=set_speed_PID[2]=y;
			set_speed_PID[1]=set_speed_PID[3]=y-x;
		}	
	}
	else if(x>-1 && y<-1)//第四象限，右后
	{
		if(x > (-y))//偏右
		{
			set_speed_PID[0]=set_speed_PID[2]=x+y;
			set_speed_PID[1]=set_speed_PID[3]=-x;
		}
		else//偏后
		{
			set_speed_PID[0]=set_speed_PID[2]=x+y;
			set_speed_PID[1]=set_speed_PID[3]=y;
		}	
	}
	else
	{
		set_speed_PID[0] = set_speed_PID[1] = set_speed_PID[2] = set_speed_PID[3] = 0;
	}	

	if(mode)//遥控器鼠标键盘模式
	{
		yaw = yaw * mouse_n;
	}
	else//遥控器模式
	{
		yaw = Yao_L_LR * yao_n;
	}
	
	//加速比
	set_speed_PID[0] *= n; 
	set_speed_PID[1] *= n; 
	set_speed_PID[2] *= n; 
	set_speed_PID[3] *= n;
	
	//计算旋转量
	set_speed_PID[0] -= yaw; set_speed_PID[1] += yaw; set_speed_PID[2] += yaw; set_speed_PID[3] -= yaw;
	
	//轮子旋转正方向矫正
	set_speed_PID[0] = -set_speed_PID[0]; 
	set_speed_PID[3] = -set_speed_PID[3];
	
	for(i = 0; i < 4; i++)
		MotorSpeedPID[i].SetPoint = set_speed_PID[i];	//速度设定
	
	//接收电机反馈
	CAN1_Receive_Msg(motor_Read);
	CAN1_Receive_Msg(motor_Read);
	CAN1_Receive_Msg(motor_Read);
	CAN1_Receive_Msg(motor_Read);
	CAN1_Receive_Msg(motor_Read);		
	
	//云台机械角度反馈值计算
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
//	else if(pid_end < 0) pid_end-=600;  //抬力
//	else pid_end = 0;
//	if(pid_end > 1000) pid_end = 1000;
//	else if(pid_end < -1000) pid_end = -1000;
//	else pid_end = pid_end;
//	canbuf_yaw[0]=pid_end/256;
//	canbuf_yaw[1]=pid_end%256;
//	CAN1_Send_Msg_yaw(canbuf_yaw,2);//发送8个字节	
	
//}

void stopAllMotor(void)
{
	int i;
	for(i = 0; i < 8; i++)
	{
		canbuf[i] = 0;
	}
	CAN1_Send_Msg(canbuf,8);//发送8个字节
	
	TIM_SetCompare1(TIM2,1000);	//修改比较值，修改占空比
	TIM_SetCompare3(TIM2,1000);	//修改比较值，修改占空比
	TIM_SetCompare4(TIM2,1000);	//修改比较值，修改占空比	
}


void setAll_PWM(void)
{
	
}
