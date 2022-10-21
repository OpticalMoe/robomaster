#include "main.h"

int speed[4] = { 800, 800, 800, 800};
int yaw_speed[2] = {0, 0};
extern yawPid yaw_PID;

int main(void)
{ 
	u8 key;
	u8 Send_Count;
	int i;
	
	//初始化区
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口1波特率为115200,用于电脑通信
	uart2_init(100000);	//初始化串口2波特率为100000，用于接收遥控器数据
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化 
	KEY_Init(); 				//按键初始化  
	BEEP_Init();     		//初始化蜂鸣器端口
	RemoteInit();				//遥控器变量初始化
 	TIM2_PWM_Init(20000-1,84-1);			//20ms周期脉冲
	TIM3_Int_Init(200-1,8400-1);  //定时器3初始化，PID计算中断 20ms
	TIM4_Int_Init(200-1,8400-1);  //定时器3初始化，PID计算中断 20ms
	IWDG_Init(4,100); //与分频数为64,重载值为500,溢出时间为200ms	
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_9tq,3,0);//CAN初始化普通模式,波特率1Mbps  
 	ShooterMotorInit();					//设计拨盘，摩擦轮电调解锁
	Motor_Init();  							//底盘电机初始化
	
//	//屏幕显示，使用示例（详情见lcd.c文件）
//	//LCD_ShowString(30,110,200,16,16,"");	
//	//LCD_ShowxNum(30,110,number,5,16,0X80);	
//	POINT_COLOR=RED;					//设置字体为红色 
//	LCD_ShowString(30,30,200,16,16,"Explorer STM32F4                    ");	
//	POINT_COLOR=BLUE;					//设置字体为蓝色		
	
	key = key; Send_Count = Send_Count; i = i;  	//避免警告
	IWDG_Feed();//喂狗
	
//	while(1)
//	{
//		set_speed_Motor[0] = set_speed_Motor[1] = set_speed_Motor[2] = set_speed_Motor[3] = 0;
//		canbuf[0]=set_speed_Motor[0]/256; canbuf[1]=set_speed_Motor[0]%256; 
//		canbuf[2]=set_speed_Motor[1]/256; canbuf[3]=set_speed_Motor[1]%256; 
//		canbuf[4]=set_speed_Motor[2]/256; canbuf[5]=set_speed_Motor[2]%256; 
//		canbuf[6]=set_speed_Motor[3]/256; canbuf[7]=set_speed_Motor[3]%256; 
//		CAN1_Send_Msg(canbuf,8);//发送8个字节		
//		delay_ms(2);
//	}

	mode = 0;
//	TIM_Cmd(TIM3,DISABLE);
	TIM_Cmd(TIM3,ENABLE);
//	TIM_Cmd(TIM4,DISABLE);
	TIM_Cmd(TIM4,ENABLE);
	
	//主程序循环区
	while(1)
	{	
		setShootMotor();		//摩擦轮、波轮启动	
		GetRemoteData();		//读取遥控器数据
		CMControlLoop();		//底盘电机控制
		

		LED0=!LED0;//提示系统正在运行	
		
		
//		//示波器数据显示
//		yaw_PID.setAngle = -4000;
//		DataScope_Get_Channel_Data(0.001*yaw_PID.setAngle,1);
//		DataScope_Get_Channel_Data(0.001*Read_angle,2);
//		DataScope_Get_Channel_Data(0.001*yaw_PID.err,3);
//		DataScope_Get_Channel_Data(0.001*yaw_PID.out,4);
//		Send_Count = DataScope_Data_Generate(10); //生成10个通道的 格式化帧数据，返回帧数据长度
//		for( i = 0 ; i < Send_Count; i++)  //循环发送,直到发送完毕   
//		{
//			printf("%c",DataScope_OutPut_Buffer[i]); 
//		}	
//		delay_ms(10);	
		
//		printf("Read_angle = %d\r\n\r\n\r\n",Read_angle);
		
//		if(key_OFF)
//		{
//			yaw_speed[0] += 10;
//			printf("speed = %d \r\n\r\n", yaw_speed[0]);
//			canbuf_yaw[0]=yaw_speed[0]/256; canbuf_yaw[1]=yaw_speed[0]%256; 
//			CAN1_Send_Msg_yaw(canbuf_yaw, 4);
//		}
//		if(key_CL)
//		{
//			yaw_speed[0] = 0;
//			printf("speed = %d \r\n\r\n", yaw_speed[0]);
//			canbuf_yaw[0]=yaw_speed[0]/256; canbuf_yaw[1]=yaw_speed[0]%256; 
//			CAN1_Send_Msg_yaw(canbuf_yaw, 4);
//		}
//		if(key_HL)
//		{
//			yaw_speed[0] -= 10;
//			printf("speed = %d \r\n\r\n", yaw_speed[0]);
//			canbuf_yaw[0]=yaw_speed[0]/256; canbuf_yaw[1]=yaw_speed[0]%256; 
//			CAN1_Send_Msg_yaw(canbuf_yaw, 4);		
//		}
//		delay_ms(100);

//		//遥控器测试，显示屏
//		LCD_ShowString(30,50,200,16,16,"Yao_R_UD");	
//		LCD_ShowxNum(120,50,Yao_R_UD,5,16,0X80);	
//		LCD_ShowString(30,70,200,16,16,"Yao_R_LR");	
//		LCD_ShowxNum(120,70,Yao_R_LR,5,16,0X80);	
//		LCD_ShowString(30,90,200,16,16,"Yao_L_UD");	
//		LCD_ShowxNum(120,90,Yao_L_UD,5,16,0X80);
//		LCD_ShowString(30,110,200,16,16,"Yao_L_LR");	
//		LCD_ShowxNum(120,110,Yao_L_LR,5,16,0X80);		
//		if(key_OFF) LCD_ShowString(30,130,200,16,16,"key_OFF  ");
//		if(key_CL) LCD_ShowString(30,130,200,16,16,"key_CL   ");
//	  if(key_HL) LCD_ShowString(30,130,200,16,16,"key_HL  ");
//		if(key_GPS) LCD_ShowString(30,150,200,16,16,"key_GPS           ");
//		if(key_ATTI_middle) LCD_ShowString(30,150,200,16,16,"key_ATTI_middle  ");
//		if(key_ATTI_down) LCD_ShowString(30,150,200,16,16,"key_ATTI_down     ");
//		if(mouse_lift_key) LCD_ShowString(30,170,200,16,16,"mouse_lift_key   "); else LCD_ShowString(30,170,200,16,16,"                  ");
//		if(mouse_right_key) LCD_ShowString(30,190,200,16,16,"mouse_right_key   "); else LCD_ShowString(30,190,200,16,16,"                  ");	
//		LCD_ShowString(30,210,200,16,16,"mouse_up_down_speed  ");
//		LCD_ShowxNum(260,210,mouse_up_down_speed,5,16,0X80);
//		LCD_ShowString(30,230,200,16,16,"mouse_lift_right_speed  ");
//		LCD_ShowxNum(260,230,mouse_lift_right_speed,5,16,0X80);
//		for(i = 0; i < 16; i++)
//		{
//			if(keyboard[i]) LCD_ShowChar(30+i*8,250,keyboard_key[i],16,0); else LCD_ShowChar(30+i*8,250,' ',16,0); 
//		}
//		//云台角度
//		LCD_ShowString(30,250,200,16,16,"YunTai");
//		LCD_ShowxNum(30,270,Read_angle,5,16,0X80);
		
//		//示波器数据显示
//		DataScope_Get_Channel_Data(0.001*Yao_R_LR,1);
//		DataScope_Get_Channel_Data(0.001*Yao_R_UD,2);
////		//1 2 号电机
////		DataScope_Get_Channel_Data(0.001*set_speed_PID[0],3);
////		DataScope_Get_Channel_Data(0.001*set_speed_PID[1],4);
////		DataScope_Get_Channel_Data(0.001*set_speed_Motor[0],5);
//////		DataScope_Get_Channel_Data(0.001*set_speed_Motor[1],6);
////		//3 4 号电机
////		DataScope_Get_Channel_Data(0.001*set_speed_PID[2],3);
////		DataScope_Get_Channel_Data(0.001*set_speed_PID[3],4);
////		DataScope_Get_Channel_Data(0.001*set_speed_Motor[2],5);
////		DataScope_Get_Channel_Data(0.001*set_speed_Motor[3],6);
//		
//		DataScope_Get_Channel_Data(0.001*set_speed_Motor[0],3);
//		DataScope_Get_Channel_Data(0.001*set_speed_Motor[1],4);
//		DataScope_Get_Channel_Data(0.001*set_speed_Motor[2],5);
//		DataScope_Get_Channel_Data(0.001*set_speed_Motor[3],6);
//		DataScope_Get_Channel_Data(0.001*real_speed[0],7);
//		DataScope_Get_Channel_Data(0.001*real_speed[1],8);
//		DataScope_Get_Channel_Data(0.001*real_speed[2],9);
//		DataScope_Get_Channel_Data(0.001*real_speed[3],10);
//		Send_Count = DataScope_Data_Generate(10); //生成10个通道的 格式化帧数据，返回帧数据长度
//		for( i = 0 ; i < Send_Count; i++)  //循环发送,直到发送完毕   
//		{
//			printf("%c",DataScope_OutPut_Buffer[i]); 
//		}		
		while(KEY0==0)
		{
			stopAllMotor();
		}
		while(Yao_R_UD > 2000 || Yao_R_UD < -2000)
		{
			stopAllMotor();
		}			
		
		IWDG_Feed();//喂狗
		delay_ms(20);		
	} 


/**************************************************************************************************************************************/		
//
//		//遥控器测试，显示屏
//		LCD_ShowString(30,50,200,16,16,"Yao_R_UD");	
//		LCD_ShowxNum(120,50,Yao_R_UD,5,16,0X80);	
//		LCD_ShowString(30,70,200,16,16,"Yao_R_LR");	
//		LCD_ShowxNum(120,70,Yao_R_LR,5,16,0X80);	
//		LCD_ShowString(30,90,200,16,16,"Yao_L_UD");	
//		LCD_ShowxNum(120,90,Yao_L_UD,5,16,0X80);
//		LCD_ShowString(30,110,200,16,16,"Yao_L_LR");	
//		LCD_ShowxNum(120,110,Yao_L_LR,5,16,0X80);		

//		if(key_OFF) LCD_ShowString(30,130,200,16,16,"key_OFF  ");
//		if(key_CL) LCD_ShowString(30,130,200,16,16,"key_CL   ");
//	  if(key_HL) LCD_ShowString(30,130,200,16,16,"key_HL  ");
//		if(key_GPS) LCD_ShowString(30,150,200,16,16,"key_GPS           ");
//		if(key_ATTI_middle) LCD_ShowString(30,150,200,16,16,"key_ATTI_middle  ");
//		if(key_ATTI_down) LCD_ShowString(30,150,200,16,16,"key_ATTI_down     ");
//		if(mouse_lift_key) LCD_ShowString(30,170,200,16,16,"mouse_lift_key   "); else LCD_ShowString(30,170,200,16,16,"                  ");
//		if(mouse_right_key) LCD_ShowString(30,190,200,16,16,"mouse_right_key   "); else LCD_ShowString(30,190,200,16,16,"                  ");
//		
//		LCD_ShowString(30,210,200,16,16,"mouse_up_down_speed  ");
//		LCD_ShowxNum(260,210,mouse_up_down_speed,5,16,0X80);
//		LCD_ShowString(30,230,200,16,16,"mouse_lift_right_speed  ");
//		LCD_ShowxNum(260,230,mouse_lift_right_speed,5,16,0X80);
//		
//		for(i = 0; i < 16; i++)
//		{
//			if(keyboard[i]) LCD_ShowChar(30+i*8,250,keyboard_key[i],16,0); else LCD_ShowChar(30+i*8,250,' ',16,0); 
//		}	

/**************************************************************************************************************************************/		
	
//	//遥控器测试，串口发送
//	GetRemoteData();
//	if(key_OFF) printf("key_OFF\r\n");
//	if(key_CL) printf("key_CL\r\n");
//	if(key_HL) printf("key_HL\r\n");
//	if(key_GPS) printf("key_GPS\r\n");
//	if(key_ATTI_middle) printf("key_ATTI_middle\r\n");
//	if(key_ATTI_down) printf("key_ATTI_down\r\n");
//	if(mouse_lift_key) printf("mouse_lift_key\r\n");
//	if(mouse_right_key) printf("mouse_right_key\r\n");
//	
//	printf("mouse_up_down_speed = %d\r\n",mouse_up_down_speed);
//	printf("mouse_lift_right_speed = %d\r\n",mouse_lift_right_speed);
//	
//	for(i = 0; i < 16; i++)
//	{
//		if(keyboard[i]) printf("%c\r\n",keyboard_key[i]);
//	}
//	printf("\r\n\r\n\r\n\r\n");
//				
//	LCD_ShowString(30,50,200,16,16,"Yao_R_UD");	
//	LCD_ShowxNum(120,50,Yao_R_UD,5,16,0X80);		
//	LCD_ShowString(30,70,200,16,16,"Yao_R_LR");	
//	LCD_ShowxNum(120,70,Yao_R_LR,5,16,0X80);		
//	LCD_ShowString(30,90,200,16,16,"Yao_L_UD");	
//	LCD_ShowxNum(120,90,Yao_L_UD,5,16,0X80);		
//	LCD_ShowString(30,110,200,16,16,"Yao_L_LR");	
//	LCD_ShowxNum(120,110,Yao_L_LR,5,16,0X80);	

/**************************************************************************************************************************************/	

//		//示波器数据显示
//		DataScope_Get_Channel_Data(0.001*Yao_R_UD,1);
//		DataScope_Get_Channel_Data(0.001*Yao_R_LR,2);
//		DataScope_Get_Channel_Data(0.001*Yao_L_UD,3);
//		DataScope_Get_Channel_Data(0.001*Yao_L_LR,4);
//		Send_Count = DataScope_Data_Generate(10); //生成10个通道的 格式化帧数据，返回帧数据长度
//		for( i = 0 ; i < Send_Count; i++)  //循环发送,直到发送完毕   
//		{
//			printf("%c",DataScope_OutPut_Buffer[i]); 
//		}

//	  key = KEY_Scan(0);	//读取学习板按键，用于测试
//		//按键测试区
//		if(key==KEY0_PRES)//KEY0按下
//		{
//		}
//		else if(key==WKUP_PRES) 
//		{
//		}
//		else if(key==KEY1_PRES)
//		{
//		}
//		else if(key==KEY2_PRES)
//		{
//		}
/**************************************************************************************************************************************/					
		
//		//接收电机反馈
//		CAN1_Receive_Msg(motor_Read);
//		CAN1_Receive_Msg(motor_Read);
//		CAN1_Receive_Msg(motor_Read);
//		CAN1_Receive_Msg(motor_Read);
//		CAN1_Receive_Msg(motor_Read);		
//		//云台机械角度反馈值计算
//		Read_angle = motor_Read[4][0]<<8 | motor_Read[4][1];
//		//底盘电机反馈计算
//		for(i = 0; i < 4; i++)
//		{
//			motor_angle[i] = motor_Read[i][0]<<8 | motor_Read[i][1];
//			real_speed[i] = motor_Read[i][2]<<8 | motor_Read[i][3];
//			if(real_speed[i] > 32767)
//			{
//				real_speed[i]=real_speed[i]-65536;
//			}
//		}
/**************************************************************************************************************************************/		

//		//速度波形显示
//		DataScope_Get_Channel_Data(0.001*real_speed[0],5);
//		DataScope_Get_Channel_Data(0.001*real_speed[1],6);
//		DataScope_Get_Channel_Data(0.001*real_speed[2],7);
//		DataScope_Get_Channel_Data(0.001*real_speed[3],8);
//		Send_Count = DataScope_Data_Generate(10); //生成10个通道的 格式化帧数据，返回帧数据长度
//		for( i = 0 ; i < Send_Count; i++)  //循环发送,直到发送完毕   
//		{
//			printf("%c",DataScope_OutPut_Buffer[i]); 
//		}
/**************************************************************************************************************************************/	

}

































