/**
  ******************************************************************************
  * @file    EV3 Brick of Dr.luck/OptimizePid.c 
  * @author  STU AI & Robot Lab
  * @version V1.0.0
  * @date    14-Nov-2015
  * @brief   Main program body according to ST source
  ******************************************************************************
**/
#include "OptimizePid.h"

//-----------------增量式PID算法优化-------------------------------------
//
float in_kp = 3;
float in_ki = 5;
float in_kd = 1;

float PID_IncrementMode(float ValueWant,float ValueNow)
{
	 static float dErrP, dErrI, dErrD;
	 static float errNow,errOld1=0,errOld2=0;
	 static float dCtrOut,ctrOut,ctrOutOld=0;
	 static float Ud,UdOld=0,a=0.3;
	
	 //==============参数准备=================
	 errNow = ValueWant - ValueNow;
	 
	 //--------------引入死区----------------------
	 if(errNow>-2&&errNow<2)	errNow = 0;

	 dErrP = errNow - errOld1;
	 dErrI = errNow;
	 dErrD = errNow - 2 * errOld1 + errOld2;

	 //============增量式PID计算=================
	 //---引入不完全微分，加入低通滤波器，减少由于微分作用过于灵敏,导致系统控制过程振荡
	 Ud = in_kd*(1-a)*dErrD + a*UdOld;		//a小于1
	 
	 //---引入积分分离，当被控量与目标量偏差较大，取消积分作用---
	 if(errNow<-20||errNow>20){			//积分分离值根据实际情况确定
			dCtrOut = in_kp * dErrP +  Ud;
	 }else{
			dCtrOut = in_kp * dErrP + in_ki * dErrI + Ud;
	 }
	 
	 //================输出======================

	 //----引入抗积分饱和，减少退出饱和区(上限区)时间---
	 if(ctrOutOld>50){
			if(dCtrOut<0)	ctrOut = ctrOutOld + dCtrOut;
			else ctrOut = ctrOutOld;
	 }
	 else if(ctrOutOld<-50){
			if(dCtrOut>0)	ctrOut = ctrOutOld + dCtrOut;
			else ctrOut = ctrOutOld;
	 }
	 
	 //===============变量保存======================
	 errOld2 = errOld1; //二阶误差微分
	 errOld1 = errNow;  //一阶误差微分
	 ctrOutOld = ctrOut;
	 UdOld = Ud;
	 
	 return ctrOut;
}


//-----------------位置式PID算法优化-------------------------------------

float ab_kp = 0.25;
float ab_ki = 0.3;
float ab_kd = 0.1;

float PID_AbsoluteMode(u8 Ochannel,float ValueWant,float ValueNow)
{
	 float errP, errD, errILim=30;
	 static float errI[4], errOld[4]={0,0,0,0}, UdOld[4]={0,0,0,0};
	 float errNow, ctrOut, Ud, a=0.3;
	 
	 //==============参数准备=================
	 errNow = ValueNow - ValueWant;
	 //--------------引入死区----------------------
//	 if(errNow>-3&&errNow<3)	errNow = 0;

	 errP = errNow;  				//读取现在的误差，用于kp控制
	 errI[Ochannel-5] += errNow; 				//误差积分，用于ki控制
	 //--------------引入积分限制----------------------
	 if(errILim != 0){	   	//积分上限和下限
			if(errI[Ochannel-5] > errILim)  errI[Ochannel-5] = errILim;
			else if(errI[Ochannel-5] < -errILim)    errI[Ochannel-5] = -errILim;
	 }
	 errD = errNow - errOld[Ochannel-5];//误差微分，用于kd控制

	 //============PID计算=================
	 //---引入不完全微分，加入低通滤波器，减少由于微分作用过于灵敏,导致系统控制过程振荡
	 Ud = ab_kd*(1-a)*errD + a*UdOld[Ochannel-5];		//a小于1
	 
	 //---引入积分分离，当被控量与目标量偏差较大，取消积分作用---
	 if(errNow<-20||errNow>20){			//积分分离值根据实际情况确定
			ctrOut = ab_kp * errP + Ud;
	 }else{
			ctrOut = ab_kp * errP + ab_ki * errI[Ochannel-5] + Ud;
	 }
	 
	 if(ctrOut>100)	
		 ctrOut = 100;
	 else if(ctrOut<=21)	
		 ctrOut = 21;
	
	 //===============变量保存======================
	 errOld[Ochannel-5] = errNow;	//保存现在的误差
	 UdOld[Ochannel-5] = Ud;
	 
	 return ctrOut;

}

//-----------------位置式PID2算法优化-------------------------------------

float ab_kp2 = 0.5;
float ab_ki2 = 0.6;
float ab_kd2 = 0;

float PID_AbsoluteMode2(u8 DeviceID, u8 Ochannel,float ValueWant,float ValueNow)//电机修改12.12
{
	 float errP, errD, errILim=2;
	 static float errI[4], errOld[4]={0,0,0,0}, UdOld[4]={0,0,0,0};
	 float errNow, ctrOut, Ud, a=0.3;
	 float Comp_ctrOut = 0;			//补偿输出
	 
		switch(DeviceID)	//设置参数
		{
			case 0x0a:      //大型电机
				ab_kp2 = 0.5;
				ab_ki2 = 0.6;
				ab_kd2 = 0;
				break;
			case 0x04:			//中型电机
				ab_kp2 = 0.4;
				ab_ki2 = 1;
				ab_kd2 = 0;				
				break;
		}	 
	 
	 ValueNow = ValueNow;//电机修改12.12
	 
	 //==============参数准备=================
	 errNow = ValueWant - ValueNow;
	 //--------------引入死区----------------------
	 if(errNow>-2&&errNow<2)	errNow = 0;

	 errP = errNow;  				//读取现在的误差，用于kp控制
	 errI[Ochannel-5] += errNow; 				//误差积分，用于ki控制
	 //--------------引入积分限制----------------------
	 if(errILim != 0){	   	//积分上限和下限
			if(errI[Ochannel-5] > errILim)  errI[Ochannel-5] = errILim;
			else if(errI[Ochannel-5] < -errILim)    errI[Ochannel-5] = -errILim;
	 }
	 errD = errNow - errOld[Ochannel-5];//误差微分，用于kd控制

	 //============PID计算=================
	 //---引入不完全微分，加入低通滤波器，减少由于微分作用过于灵敏,导致系统控制过程振荡
	 Ud = ab_kd2*(1-a)*errD + a*UdOld[Ochannel-5];		//a小于1
	 
	 //---引入积分分离，当被控量与目标量偏差较大，取消积分作用---
	 if(errNow<-5||errNow>5){			//积分分离值根据实际情况确定
			ctrOut = ab_kp2 * errP + Ud;
	 }else{
			ctrOut = ab_kp2 * errP + ab_ki2 * errI[Ochannel-5] + Ud;
	 }
	 
	 if(ValueWant>80||ValueWant<-80)	
		 Comp_ctrOut = 80;
	 else if(ValueWant>70||ValueWant<-70)	
		 Comp_ctrOut = 70;
	 else if(ValueWant>50||ValueWant<-50)	
		 Comp_ctrOut = 65;
	 else if(ValueWant>30||ValueWant<-30)	
		 Comp_ctrOut = 55;
	 else if(ValueWant>=10||ValueWant<-10)		
		 Comp_ctrOut = 50;
	 else if(ValueWant>0||ValueWant<0)		
		 Comp_ctrOut = 40;
	 
	 if(ValueWant>0)	
		 ctrOut += Comp_ctrOut;
	 else if(ValueWant<0)	
		 ctrOut -= Comp_ctrOut;
	
	 //===============变量保存======================
	 errOld[Ochannel-5] = errNow;	//保存现在的误差
	 UdOld[Ochannel-5] = Ud;
	 
	 return ctrOut;

}




