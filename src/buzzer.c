#include "timer.h"
#include "buzzer.h"
#include "task.h"
#include "FreeRTOS.h"
#include <libopencm3/stm32/timer.h> 

uint16_t pwmVal=0;   //PWM占空�??


int music_3[]=
{
	M5,50,M5,25,M5,25,
	M6,100,M5,100,H1,100,
	M7,100,M7,100,M5,50,M5,25,M5,25,
	M6,100,M5,100,H2,100,
	H1,100,H1,100,M5,50,M5,25,M5,25,
	H5,100,H3,100,H1,100,
	M7,100,M6,100,H4,50,H4,25,H4,25,
	H3,100,H1,100,H2,100,H1,100,H1,100
};//生日快乐歌C调简谱C语言表示方法(M5,50)为(音调，音长)100表示为一个音长(节拍)，50位半个音长(节拍)，25位四分之一音长(节拍)

int music_2[]=
{
//    	L1,100,L2,100,L3,100,L4,100,L5,100,L6,100,L7,100,M1,100,M2,100,M3,100,M4,100,M5,100,M6,100,M7,100
	L5,50,  M3,50,  M3,50,  L5,50,  L5,50,  M2,50,  M2,100,		//想去远方的山川，
	M1,50,  M1,50,  M1,50,  M2,50,  M3,50,  L5,50,  L5,100,		//想去海边看海鸥，
	L6,50,  M1,50,  M1,50,  L6,50,  L5,50,  M3,50,  M3,100,	 M2,50,  M1,50,  M1,50,  L6,50,  M2,150,	//不管风雨有多少，有你就足够。
	L5,50,  M3,50,  M3,50,  L5,50,  L5,50,  M2,50,  M2,100,		//喜欢看你的嘴角
	M1,50,  M1,50,  M1,50,  L6,50,  L5,50,  M3,50,  M3,100,		//喜欢看你的眉梢。
	L6,50,  M4,50,  M4,50,  L6,50,  L5,50,  M3,50,  M3,100,		//白云挂在那蓝天，
};

int music_0[]=
{
	0,50,  M6,50,  H1,25,  M6,25,  M6,25,  M6,25,  H1,25,  M6,25,  M6,25,  M6,25,  H1,25,  M6,25,  M6,25,  M6,25,
	H5,3, H3,25,  H3,25,  H3,25,
};//厚颜无耻

int music_1[]=
{
	H2,25,  H3,75,  H3,50,  H3,25,  H3,25,  H3,50,  H6,50,  H5,50,  H2,25,  H2,25,  H2,100,
	 0,25,	H4,50,  H4,25,  H4,25,  H3,25,
	 H2,25,  H2,50,  H3,50,  M6,25,  M6,25,  M6,100,
	 H1,50, H1,25,  H1,25,  H1,50,  H3,25,  H2,150,  H1,25,  H2,75,  H3,100,
};//大地




int length ;//计算数组长度

void buzzer_breathe(void)		//变频
{
	timer_enable_counter(TIM3);
	while (pwmVal< 500)
	  {
		pwmVal++;
		timer_set_oc_value(TIM3, TIM_OC1, pwmVal);   //与上方等效
		vTaskDelay(pdMS_TO_TICKS(1));
	  }
	  while (pwmVal)
	  {
		pwmVal--;
		timer_set_oc_value(TIM3, TIM_OC1, pwmVal);   // 与上方等效
		vTaskDelay(pdMS_TO_TICKS(1));
	  }
	timer_disable_counter(TIM3);
}

void Sound(int cycle)
{
	timer_set_period(TIM3, (uint16_t)((cycle-1)/1));
	timer_set_oc_value(TIM3, TIM_OC1, (uint16_t)((cycle-1)/2)); 
}

void play_music(int Song_Num)
{
   int i=0;
	timer_enable_counter(TIM3);
		switch (Song_Num)
			{
			case 0:
				{
					length =sizeof(music_0)/sizeof(music_0[0]);
					for(i=0; i<(length/2); i++)//取数组数据
					{
						Sound(music_0[2*i]);//PWM波周期
						vTaskDelay(pdMS_TO_TICKS(10*music_0[2*i+1]));//音长的时间都乘以5即一拍为500微秒，此值"5"可调整，只是播放的整个快慢而已，有点类似于视频快进和后退
					}
				}break;
			case 1:
				{
					length = sizeof(music_1)/sizeof(music_1[0]);
					for(i=0; i<(length/2); i++)//取数组数据
					{
						Sound(music_1[2*i]);//PWM波周期
						vTaskDelay(pdMS_TO_TICKS(10*music_1[2*i+1]));//音长的时间都乘以5即一拍为500微秒，此值"5"可调整，只是播放的整个快慢而已，有点类似于视频快进和后退
					}
				}break;
			default :
				;
			}
	timer_disable_counter(TIM3);
}