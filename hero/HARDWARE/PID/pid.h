#ifndef __PID_H
#define __PID_H
#include "main.h"

typedef struct
{
	int 		SetPoint; 		//设定目标 Desired Value
	long 		SumError; 		//误差累计
	double 	Proportion; 	//比例常数 Proportional Const
	double 	Integral; 		//积分常数 Integral Const
	double 	Derivative; 	//微分常数 Derivative Const
	int 		LastError; 		//Error[-1]
	int 		PrevError; 		//Error[-2]
	int 		outspeed;
}PIDtype;

typedef struct yawPid {
	float setAngle;           //定义设定值     
	float realAngle;        	//定义实际值     
	float err;                //定义偏差值      
	float err_next;           //定义上一个偏差值     
	float err_last;           //定义最上前的偏差值   
	float err_sum;            //总偏移
	float Kp, Ki, Kd;         //定义比例、积分、微分系数 
	int out;
}yawPid;


int IncPIDCalc(PIDtype *sptr,int NextPoint);
void PID_Init(PIDtype *PIDx);
void setPID(PIDtype *PIDx, double p, double i, double d);
void YawPID_Init(yawPid *sptr);
void setYawPID(yawPid *sptr, float p, float i, float d);
int getYawPIDout(yawPid *sptr, int);

#endif

