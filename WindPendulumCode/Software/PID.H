#ifndef __PID_H_
#define __PID_H_
#include "Sys.h"
#if __PID_ENABLE
#define PID_ERR    200

#define PID_LEVEL  100

extern u8 _fangxiang;
static struct _pid
{
  float Sets;      //����ֵ
	float Actuals;   //ʵ��ֵ
	float err;       //ƫ��ֵ
	float err_last;  //��һ��ƫ��ֵ
	u16 Kp,Ki,Kd;  //���������֡�΢��ϵ��
	float voltage;   //��ѹֵ
	u16 integral;  //����ֵ
}pid[2];

void PID_BP1Init(void);
void PID_BP2Init(void);
void PID_BP3Init(void);
void PID_BP4Init(void);
void PID_IF1Init(void);

u16 PID_del1(float _data,u8 _num);
u16 PID_del2(float _data,u8 _num);
void Wind_drive_YStop(void);
void Wind_drive_XStop(void);
void Wind_drive_YGoal(void);
void Wind_drive_YGoal2(void);
void Wind_drive_YGoalBF3_0(void);
void Wind_drive_YGoalBF3_1(void);
void Wind_drive_XGoal(void);
void Wind_drive_XGoal2(void);
#endif   // __PID_ENABLE
#endif   //__PID_H_
