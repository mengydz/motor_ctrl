#ifndef __PID_H
#define __PID_H	 
#include "stm32f1xx_hal.h"
#include "main.h"


typedef struct {   

  float SetPoint;           //  设定目标 Desired Value 
  float Dt;

  float Proportion;         //  比例常数 Proportional Const   
  float Integral;           //  积分常数 Integral Const   
  float Derivative;         //  微分常数 Derivative Const   

  float errMaxIntegral;

  float PoutLimit;
  float IoutLimit;
  float DoutLimit;
  float PIDoutLimit;

  float Pout;
  float Iout;
  float Dout;

  float DiffFilt;
  float ErrFilt;

  float PrevError;          //  Error[-1]  
  float SumError;           //  Sums of Errors   

	float Output;
} PIDStruct;       

void PIDInit (PIDStruct *pp);   
float PIDCalc( PIDStruct *pid, float SetValue,float Measure, float dt);  

extern PIDStruct LeftSpeedPid,RightSpeedPid;
#endif

