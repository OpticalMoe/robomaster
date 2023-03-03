#ifndef __PID_H
#define __PID_H
#include "main.h"

typedef struct
{
	int 		SetPoint; 		//�趨Ŀ�� Desired Value
	long 		SumError; 		//����ۼ�
	double 	Proportion; 	//�������� Proportional Const
	double 	Integral; 		//���ֳ��� Integral Const
	double 	Derivative; 	//΢�ֳ��� Derivative Const
	int 		LastError; 		//Error[-1]
	int 		PrevError; 		//Error[-2]
	int 		outspeed;
}PIDtype;

typedef struct yawPid {
	float setAngle;           //�����趨ֵ     
	float realAngle;        	//����ʵ��ֵ     
	float err;                //����ƫ��ֵ      
	float err_next;           //������һ��ƫ��ֵ     
	float err_last;           //��������ǰ��ƫ��ֵ   
	float err_sum;            //��ƫ��
	float Kp, Ki, Kd;         //������������֡�΢��ϵ�� 
	int out;
}yawPid;


int IncPIDCalc(PIDtype *sptr,int NextPoint);
void PID_Init(PIDtype *PIDx);
void setPID(PIDtype *PIDx, double p, double i, double d);
void YawPID_Init(yawPid *sptr);
void setYawPID(yawPid *sptr, float p, float i, float d);
int getYawPIDout(yawPid *sptr, int);

#endif

