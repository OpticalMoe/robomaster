#include "pid.h"

int IncPIDCalc(PIDtype *sptr,int NextPoint)
{
  int incrementSpeed;
	int iError;

	iError = sptr->SetPoint - NextPoint; //增量计算

	incrementSpeed = sptr->Proportion * (iError - sptr->PrevError)
									+ sptr->Integral * iError
									+ sptr->Derivative * (iError - 2*sptr->PrevError + sptr->LastError);
		//存储误差，用于下次计算
	sptr->LastError = sptr->PrevError;
	sptr->PrevError = iError;
	
//	incrementSpeed = sptr->PrevError * iError
//										- sptr->Integral * sptr->LastError
//										+sptr->Derivative * sptr->PrevError;
//	sptr->PrevError = sptr->LastError; //存储误差，用于下次计算
//	sptr->LastError = iError;

	sptr->outspeed = incrementSpeed;
	
	//返回输出值
	return(sptr->outspeed);
}




void PID_Init(PIDtype *PIDx)
{
	PIDx->Proportion = 0;
	PIDx->Integral = 0;
	PIDx->Derivative = 0;
	
	PIDx->LastError = 0;
	PIDx->PrevError = 0;
	PIDx->SumError = 0;
	
	PIDx->SetPoint = 0;
}

void setPID(PIDtype *PIDx, double p, double i, double d)
{
	PIDx->Proportion = p;
	PIDx->Integral = i;
	PIDx->Derivative =d;
}

void YawPID_Init(yawPid *sptr)
{
	sptr->setAngle = 4000;
	sptr->err_next = 0;
	sptr->err_last = 0;
	sptr->err_sum = 0;
	sptr->Kp = 0;
	sptr->Ki = 0;
	sptr->Kd = 0;
	sptr->out = 0;
}
void setYawPID(yawPid *sptr,float p, float i, float d)
{
	sptr->Kp = p;
	sptr->Ki = i;
	sptr->Kd = d;
}
int getYawPIDout(yawPid *sptr, int realAngle)
{
	float dErr;
	
	sptr->realAngle = realAngle;
	
	sptr->err = sptr->setAngle - sptr->realAngle;
	
	sptr->err_sum+=sptr->err;
	
	dErr = sptr->err-sptr->err_next;
	
	sptr->out = (int)(sptr->Kp*sptr->err+sptr->Ki*sptr->err_sum+sptr->Kd*dErr);
	
	sptr->err_next = sptr->err;
	
	return sptr->out;
}



