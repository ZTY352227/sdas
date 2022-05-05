#include "fan.h"


//根据逆变器和电芯温度，开启风扇不同占空比

//void HeatManage(void)//100ms调用一次
//{
//	static uint8_t fanEnable = 0;
//	static uint8_t fanLevel = 0;//转速等级 0-停止  1-50%  2-60%  3-80%  4-100%
//	static uint8_t Boardflag = 0, invertorFlag = 0, MOSFlag = 0, BattFlag = 0;
//	static uint8_t BordTempCnt = 0,invertorflagTemp=0;
//	uint8_t BordflagTemp = 0;
//	
//	if(TestFlag == TEST_STATE)//测试状态不处理，由上位机控制
//		return;
//	
//	//板上温度判断
//	for(uint8_t i=0; i<3; i++)
//	{
//		if(NTC[i] >= 8500)
//		{
//			BordflagTemp |= 0x08;
//			BordTempCnt = 0;
//		}
//		else if(NTC[i] >= 8000)
//		{
//			BordflagTemp |= 0x04;
//			BordTempCnt = 0;
//		}
//		else if(NTC[i] >= 7500)
//		{
//			BordflagTemp |= 0x02;
//			BordTempCnt = 0;
//		}
//		else if(NTC[i] >= 7000)
//		{
//			BordflagTemp |= 0x01;
//			BordTempCnt = 0;
//		}
//	}
//	
//	if((BordflagTemp != Boardflag) && (BordflagTemp!=0))
//	{
//		Boardflag = BordflagTemp;
//	}
//	
//	if(Boardflag)
//	{
//		uint8_t cnt=0;
//		
//		for(uint8_t i=0; i<3; i++)
//		{
//			if(NTC[i] < 6000)
//			{
//				cnt++;
//			}
//		}
//		
//		if(cnt >= 3)//3路NTC温度都降下来了
//		{
//			BordTempCnt++;
//			if(BordTempCnt >= 10)//连续1S满足低温不开风扇要求
//			{
//				BordTempCnt = 0;
//				Boardflag = 0;
//			}
//		}
//		else
//		{
//			BordTempCnt = 0;
//		}
//			
//	}
//	
//	//逆变器功率判断
////	if(Invertor.PowerOut>400)
////	{
////		if(invertorLoadTime == 0)
////			invertorLoadTime = 1;
////		
////		if(invertorLoadTime > 5)
////		{
////			invertorflagTemp = 4;
////			invertorLoadTime = 0;
////		}
////		
////		if(!BattStatusFlag.ACOutFlag && invertorCoolTime==0)//关掉逆变器后开始计时
////		{
////		  invertorCoolTime = 1;
////			invertorflagTemp = 0;
////		}
////	}
////	else if(Invertor.PowerOut>=300 && Invertor.PowerOut<=400)
////	{
////		if(invertorLoadTime == 0)
////			invertorLoadTime = 1;
////		
////		if(invertorLoadTime > 10)
////		{
////			invertorflagTemp = 3;
////			invertorLoadTime = 0;
////		}
////		
////		if(!BattStatusFlag.ACOutFlag && invertorCoolTime==0)//关掉逆变器后开始计时
////		{
////		  invertorCoolTime = 1;
////			invertorflagTemp = 0;
////		}
////	}
////	else if(Invertor.PowerOut>=200 && Invertor.PowerOut<=300)
////	{
////		if(invertorLoadTime == 0)
////			invertorLoadTime = 1;
////		
////		if(invertorLoadTime > 15)
////		{
////			invertorflagTemp = 2;
////			invertorLoadTime = 0;
////		}
////		
////		if(!BattStatusFlag.ACOutFlag && invertorCoolTime==0)//关掉逆变器后开始计时
////		{
////		  invertorCoolTime = 1;
////			invertorflagTemp = 0;
////		}
////	}
////	else if(Invertor.PowerOut>=30 && Invertor.PowerOut<=200)
////	{
////		if(invertorLoadTime == 0)
////			invertorLoadTime = 1;
////		
////		if(invertorLoadTime > 20)
////		{
////			invertorflagTemp = 1;
////			invertorLoadTime = 0;
////		}
////		
////		if(!BattStatusFlag.ACOutFlag && invertorCoolTime==0)//关掉逆变器后开始计时
////		{
////		  invertorCoolTime = 1;
////			invertorflagTemp = 0;
////		}
////	}
////	else
////	{
////		invertorflagTemp = 0;
////		invertorLoadTime = 0;
////	}
//	
//	if(Invertor.PowerOut>=30)
//	{
//		invertorflagTemp = 1;
//	}
//	else
//		invertorflagTemp = 0;
//	
//	//逆变器温度判断
//	if(Invertor.temp>=85)
//	{
//		invertorFlag = 4;
//		
//		if(!BattStatusFlag.ACOutFlag && invertorCoolTime==0)//关掉逆变器后开始计时
//		{
//		  invertorCoolTime = 1;
//			invertorflagTemp = 0;
//			Invertor.PowerOut = 0;
//		}
//	}
//	else if(Invertor.temp>=80)
//	{
//		invertorFlag = 3;
//		
//		if(!BattStatusFlag.ACOutFlag && invertorCoolTime==0)//关掉逆变器后开始计时
//		{
//		  invertorCoolTime = 1;
//			invertorflagTemp = 0;
//			Invertor.PowerOut = 0;
//		}
//	}
//	else if(Invertor.temp>=75)
//	{
//		invertorFlag = 2;
//		
//		if(!BattStatusFlag.ACOutFlag && invertorCoolTime==0)//关掉逆变器后开始计时
//		{
//		  invertorCoolTime = 1;
//			invertorflagTemp = 0;
//			Invertor.PowerOut = 0;
//		}
//	}
//	else if(Invertor.temp>=70)
//	{
//		invertorFlag = 1;
//		
//		if(!BattStatusFlag.ACOutFlag && invertorCoolTime==0)//关掉逆变器后开始计时
//		{
//		  invertorCoolTime = 1;
//			invertorflagTemp = 0;
//			Invertor.PowerOut = 0;
//		}
//	}
//	
//	if(invertorflagTemp > invertorFlag)
//		invertorFlag = invertorflagTemp;
//	
//	if(invertorFlag)
//	{
//		if(Invertor.temp < 60 && Invertor.PowerOut < 25)
//		{
//			invertorFlag = 0;
//		}
//		
//		if(invertorFlag==1 && invertorCoolTime>15)
//		{
//			invertorFlag = 0;
//	    memset(&Invertor, 0, sizeof(Invertor));
//	    Invertor.temp = 25;
//			invertorCoolTime = 0;
//		}
//		
//		if(invertorFlag==2 && invertorCoolTime>30)
//		{
//			invertorFlag = 0;
//	    memset(&Invertor, 0, sizeof(Invertor));
//	    Invertor.temp = 25;
//			invertorCoolTime = 0;
//		}

//		if(invertorFlag==3 && invertorCoolTime>60)
//		{
//			invertorFlag = 0;
//	    memset(&Invertor, 0, sizeof(Invertor));
//	    Invertor.temp = 25;
//			invertorCoolTime = 0;
//		}

//		if(invertorFlag==4 && invertorCoolTime>90)
//		{
//			invertorFlag = 0;
//	    memset(&Invertor, 0, sizeof(Invertor));
//	    Invertor.temp = 25;
//			invertorCoolTime = 0;
//		}		
//	}

//	//MOS温度判断
//	if(BattInformation.MOSTemp >= 8500)
//	{
//		MOSFlag = 4;
//	}
//	else if(BattInformation.MOSTemp >= 8000)
//	{
//		MOSFlag = 3;
//	}
//	else if(BattInformation.MOSTemp >= 7500)
//	{
//		MOSFlag = 2;
//	}
//	else if(BattInformation.MOSTemp >= 7000)
//	{
//		MOSFlag = 1;
//	}
//	
//	if(MOSFlag)
//	{
//		if(BattInformation.MOSTemp < 6500)
//		{
//			MOSFlag = 0;
//		}
//	}
//	
//	//电池温度判断
//	if((BattInformation.state&0x0F) < STATE_DISCHARGE)//充电
//	{
//		if(BattInformation.MAXCellTemp >= 5000)
//		{
//			BattFlag = 4;
//		}
//		else if(BattInformation.MAXCellTemp >= 4500 && BattInformation.MAXCellTemp < 5000)
//		{
//			BattFlag = 3;
//		}
//		else if(BattInformation.MAXCellTemp >= 4200 && BattInformation.MAXCellTemp < 4500)
//		{
//			BattFlag = 2;
//		}
//  }
//	else//放电
//	{
//		if(BattInformation.MAXCellTemp >= 5500)
//		{
//			BattFlag = 4;
//		}
//		else if(BattInformation.MAXCellTemp >= 5000 && BattInformation.MAXCellTemp < 5500)
//		{
//			BattFlag = 3;
//		}
//		else if(BattInformation.MAXCellTemp >= 4500 && BattInformation.MAXCellTemp < 5000)
//		{
//			BattFlag = 2;
//		}
//		else if(BattInformation.MAXCellTemp >= 4200 && BattInformation.MAXCellTemp < 4500)
//		{
//			BattFlag = 1;
//		}
//	}
//	
//	
//	if(BattFlag)
//	{
//		if(((BattInformation.state&0x0F) >= STATE_DISCHARGE && powerOut_Display<15) //放电功率小于15W
//       || ((BattInformation.state&0x0F) < STATE_DISCHARGE && BattInformation.PackCurrent < 50))//充电电流小于0.5A
//		{
//			UserTimerStrat(&TmierGroup_1s[TIMER_BATT_NO_LOAD]);
//		}
//		else
//		{
//			UserTimerStop(&TmierGroup_1s[TIMER_BATT_NO_LOAD]);
//		}
//		
//		if(UserTimerDelay(&TmierGroup_1s[TIMER_BATT_NO_LOAD]) >= 60)
//		{
//			UserTimerLock(&TmierGroup_1s[TIMER_BATT_NO_LOAD]);
//			BattFlag = 0; 
//		}
//		
//		if(BattInformation.MAXCellTemp <= 4000)
//			BattFlag = 0; 
//	}
//	
//	
//	//按温度等级风扇输出控制
//	if((BattFlag==4) || (Boardflag&0x08) || (invertorFlag==4) || (MOSFlag==4))
//	{
//		if(fanEnable == 0)
//		{
//		  fan_pwm_enable();
//			fanEnable = 1;
//		}
//		
//		if(fanLevel != 4)
//		{
//			fan_on();
//		  fan_set_duty(100);
//			fanLevel = 4;
//		}
//	}
//	else if((BattFlag==3) || (Boardflag&0x04) || (invertorFlag==3) || (MOSFlag==3))
//	{
//		if(fanEnable == 0)
//		{
//		  fan_pwm_enable();
//			fanEnable = 1;
//		}
//		
//		if(fanLevel != 3)
//		{
//			fan_on();
//		  fan_set_duty(80);
//			fanLevel = 3;
//		}
//	}
//	else if((BattFlag==2) || (Boardflag&0x02) || (invertorFlag==2) || (MOSFlag==2))
//	{
//		if(fanEnable == 0)
//		{
//		  fan_pwm_enable();
//			fanEnable = 1;
//		}
//		
//		if(fanLevel != 2)
//		{
//			fan_on();
//		  fan_set_duty(60);
//			fanLevel = 2;
//		}
//	}
//	else if((BattFlag==1) || (Boardflag&0x01) || (invertorFlag==1) || (MOSFlag==1))
//	{
//		if(fanEnable == 0)
//		{
//		  fan_pwm_enable();
//			fanEnable = 1;
//		}
//		
//		if(fanLevel != 1)
//		{
//			fan_on();
//		  fan_set_duty(50);
//			fanLevel = 1;
//		}
//	}
//	else
//	{
//		fan_stop();
//		FAN_off;
//		fanEnable = 0;
//		fanLevel = 0;
//	}
//	
//	if(UserTimerDelay(&TmierGroup_1s[TIMER_FAN_ON_COUNT]) >=2 && fanLevel>0)
//	{
//		UserTimerSet(&TmierGroup_1s[TIMER_FAN_ON_COUNT], 3);
//		
//		if(UserTimerDelay(&TmierGroup_1s[TIMER_FAN1]) >= 3)
//		{
//			UserTimerStop(&TmierGroup_1s[TIMER_FAN1]);
//			FAN1fre = 0;
//		}
//		
//		if(FAN1fre < 50)
//			SysAlarm.packAlarmFlag1 |= BIT6;
//		else
//			SysAlarm.packAlarmFlag1 &= (~BIT6);
//		
//		
//		
//		if(UserTimerDelay(&TmierGroup_1s[TIMER_FAN2]) >= 3)
//		{
//			UserTimerStop(&TmierGroup_1s[TIMER_FAN2]);
//			FAN2fre = 0;
//		}		
//		if(FAN2fre < 50)
//			SysAlarm.packAlarmFlag1 |= BIT7;
//		else
//			SysAlarm.packAlarmFlag1 &= (~BIT7);
//	}
//	
//}