#include"func_common.h"
#include <stdarg.h>
#include<stdio.h>
#include<string.h>
#include <libopencm3/stm32/usart.h>

int Sum(int Num, ...)
{
    int S = 0, T;
    va_list ap;
    va_start(ap, Num);
    for (int i = 0; i<Num; ++i)
    {
        T = va_arg(ap, int); S += T;
    }
    return S;
}

void float2char(float slope,char*buffer,int n)	//浮点型数，存储的字符数组，字符数组的长度
{
    int temp,i,j;
	if(slope>=0)//判断是否大于0
		buffer[0] = '+';
	else
	{
		buffer[0] = '-';
		slope = -slope;
	}
	temp = (int)slope;//取整数部分
	for(i=0;temp!=0;i++)//计算整数部分的位数
		temp /=10;
	temp  =(int)slope;
	if(slope>0 && slope<1)
		i++;
	for(j=i;j>0;j--)//将整数部分转换成字符串型
	{
		buffer[j] = temp%10+'0';
		temp /=10;
	}
	buffer[i+1]  = '.';
	slope -=(int)slope;
	for(i=i+2;i<n-1;i++)//将小数部分转换成字符串型
	{
		slope*=10;
		buffer[i]=(int)slope+'0';
		slope-=(int)slope;
	}
	buffer[n-1] = '\0';
}

void PrintFloat(float value)
{
    int tmp,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6;
    tmp = (int)value;
    tmp1=(int)((value-tmp)*10)%10;
    tmp2=(int)((value-tmp)*100)%10;
    tmp3=(int)((value-tmp)*1000)%10;
    tmp4=(int)((value-tmp)*10000)%10;
    tmp5=(int)((value-tmp)*100000)%10;
    tmp6=(int)((value-tmp)*1000000)%10;
    printf("f-value=%d.%d%d%d%d%d%d\r\n",tmp,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6);
}

void checkCPU(void)
{
   union w
    {
        int a;
        char b;
    }c;
    c.a = 1;
    // printf("c.a的值:   %x\n",c.a);
    // printf("c.b的值:   %x\n",c.b);
	if(c.b == 1)
        printf("这CPU是小端\n");
    else
        printf("这CPU是大端\n");
}