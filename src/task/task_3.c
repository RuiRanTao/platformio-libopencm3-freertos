/***********official***********/
#include "FreeRTOS.h"
#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include <math.h>       /* sin函数必须包含头文件math.h */
#include "task.h"
/*****************************/
#include"func_common.h"
#include "buzzer.h"
#include "task_.h"

#define PI 3.14159265

int num1 = 90;
int num2 = 90;
int deg1 = 1;
int deg2 = 1;
float f_1 = 0;
float f_2 = 0;
char buf_1[10]={0};
char buf_2[10]={0};
uint8_t rand_num = 0;


void fun_task_3(void)
{
    // play_music(0);
	// enQueue( &rear, a); printf("enQueue\n");  print_Link(H,front,rear);
	// vTaskDelay(pdMS_TO_TICKS(1000));
	// play_music(1);
	// srand((unsigned)time(NULL));
	rand_num = rand()%200;
	if(abs(num1)>=180)
	{
		// deg1 = -deg1;
		num1 = -180;
	}
	if(abs(num2)>=180)
	{
		// deg2 = -deg2;
		num2 = -180;
	}
	num1+=deg1;num2+=deg2;
	f_1 = 180*cos((float)num1*PI/180);
	f_2 = 180*sin((float)num2*PI/180);
	// sprintf(buf, "%f",9.0);
	float2char(f_1,buf_1,10);
	float2char(f_2,buf_2,10);
	printf("channels: %d, %d, %d, %s, %s\n",\
							rand_num,\
							num1 ,\
							num2 ,\
							buf_1 ,\
							buf_2
								);
	// printf("%d\n", Sum(10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
	// checkCPU();
	vTaskDelay(pdMS_TO_TICKS(100));
	// vTaskDelay(pdMS_TO_TICKS(900));
}