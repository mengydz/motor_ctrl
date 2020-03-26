#include "PID.h"
#include "string.h"
#include "math.h"

PIDStruct LeftSpeedPid=
{
	.Proportion 	= 1.0f,
	.Integral 		= 1.0f,
	.Derivative 	= 0.0f,
	.errMaxIntegral = 0.0f,
	.PoutLimit 		= 999.0f,
	.IoutLimit 		= 999.0f,
	.DoutLimit 		= 500.0f,
	.PIDoutLimit 	= 999.0f, 
};
PIDStruct RightSpeedPid=
{
	.Proportion 	= 1.0f,
	.Integral 		= 1.0f,
	.Derivative 	= 0.0f,
	.errMaxIntegral = 0.0f,
	.PoutLimit 		= 999.0f,
	.IoutLimit 		= 999.0f,
	.DoutLimit 		= 500.0f,
	.PIDoutLimit 	= 999.0f, 
};

float Math_fConstrain(float value, float min, float max)
{
  if(value > max)value = max;
    else if(value < min)value = min;
  return value;
}


float PIDCalc( PIDStruct *pid, float SetValue,float Measure, float dt)   
{   
	float Err;
	float Diff;
	
	pid->Dt = dt;
	pid->SetPoint = SetValue;
	Err  = SetValue -  Measure;     
	Diff = (Err - pid->PrevError) / dt;
	pid->PrevError = Err;

	pid->Pout = pid->Proportion * Err;
	pid->Pout = Math_fConstrain(pid->Pout,-fabs(pid->PoutLimit),fabs(pid->PoutLimit));
	
	pid->SumError += Err * pid->Integral * dt;   
	pid->SumError = Math_fConstrain(pid->SumError,-fabs(pid->IoutLimit),fabs(pid->IoutLimit));
	pid->Iout = pid->SumError;
	
	pid->Dout = pid->Derivative * Diff;
	pid->Dout = Math_fConstrain(pid->Dout,-fabs(pid->DoutLimit),fabs(pid->DoutLimit));
	
	pid->Output = pid->Pout + pid->Iout + pid->Dout;
	pid->Output = Math_fConstrain(pid->Output,-fabs(pid->PIDoutLimit),fabs(pid->PIDoutLimit));

	return pid->Output;
} 

void PIDInit (PIDStruct *pp)   
{   
	memset ( pp,0,sizeof(PIDStruct));   		//memset
}         



