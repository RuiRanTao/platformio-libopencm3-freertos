/**
  * @file  iic_dup.c
  * @brief IIC上层程序
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       初次创建
  *
  */

/** 头文件包含区 ------------------------------------------------ */
#include "flash.h"
#include "main.h"
#include "ee_iic.h"
#include "FreeRTOS.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include "task.h"

/** 私有宏(类型定义) -------------------------------------------- */
#define IIC1_SCL(statue)        (statue==0)?gpio_clear(GPIOB, GPIO4):gpio_set(GPIOB, GPIO4);
#define IIC1_SDA(statue)        (statue==0)?gpio_clear(GPIOB, GPIO6):gpio_set(GPIOB, GPIO6);
#define IIC1_SCL_IS_HIGH()          (gpio_get(GPIOB, GPIO4) != GPIO_PIN_RESET)
#define IIC1_SDA_IS_HIGH()          (gpio_get(GPIOB, GPIO6) != GPIO_PIN_RESET)
/** 私有变量 --------------------------------------------------- */


/** 外部变量 --------------------------------------------------- */

/** 私有函数原型 ----------------------------------------------- */
static void ee_iic_delay(void);


void ee_iic_init(void)
{
  /* Set GPIO4,GPIO6 (in GPIO port B) to 'output opendrain'.   nss for i2c*/
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_AFIO);
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON , 0);
    AFIO_MAPR |= AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON;
	
	// Set pinmode for PB4,PB6
	gpio_set_mode(
		GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL,
		GPIO4);
	gpio_set_mode(
		GPIOB,
		GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_OPENDRAIN,
		GPIO6);
}


/** 公有函数 --------------------------------------------------- */
/**
  * @brief  IIC启动
  * @param  None
  * @retval None
  * @note   当SCL处于高电平状态时，SDA出现一个下降沿
            即产生IIC启动信号
  */
void ee_iic_start(void)
{
    IIC1_SCL(GPIO_PIN_SET);
    /** SDA产生一个下降沿 */
    IIC1_SDA(GPIO_PIN_SET);
    ee_iic_delay();

    IIC1_SDA(GPIO_PIN_RESET);
    ee_iic_delay();
    IIC1_SCL(GPIO_PIN_RESET);   /**< 拉低准备发送数据 */
    ee_iic_delay();
}

/**
  * @brief  IIC停止
  * @param  None
  * @retval None
  * @note   当SCL处于高电平状态时，SDA出现一个上升沿
            即产生IIC停止信号
  */
void ee_iic_stop(void)
{
    IIC1_SCL(GPIO_PIN_RESET);
    ee_iic_delay();
    /** SDA产生一个上升沿 */
    IIC1_SDA(GPIO_PIN_RESET);
    ee_iic_delay();

    IIC1_SCL(GPIO_PIN_SET);
    ee_iic_delay();
    IIC1_SDA(GPIO_PIN_SET);
    ee_iic_delay();
}

/**
  * @brief  IIC发送1byte数据
  * @param  None
  * @retval None
  * @note
  */
void ee_iic_sendbyte(uint8_t byte)
{
    uint8_t i;

    /** 发送一个字节的高7位 */
    for (i = 0; i < 8; i++)
    {
        if (byte & 0x80)
        {
            IIC1_SDA(GPIO_PIN_SET);
        }
        else
        {
            IIC1_SDA(GPIO_PIN_RESET);
        }

        ee_iic_delay();
        IIC1_SCL(GPIO_PIN_SET);
        ee_iic_delay();
        IIC1_SCL(GPIO_PIN_RESET);

        if (i == 7)
        {
            IIC1_SDA(GPIO_PIN_SET);
        }

        byte <<= 1;
        ee_iic_delay();
    }
}

/**
  * @brief  IIC读取1byte数据
  * @param  None
  * @retval None
  * @note
  */
uint8_t ee_iic_readbyte(void)
{
    uint8_t i;
    uint8_t recv_value = 0;

    IIC1_SDA(GPIO_PIN_SET);
    ee_iic_delay();

    for (i = 0; i < 8; i++)
    {
        IIC1_SCL(GPIO_PIN_SET);
        ee_iic_delay();
        recv_value <<= 1;
        if (IIC1_SDA_IS_HIGH())
        {
            recv_value |= 0x01;
        }
        else
        {
            recv_value &= ~0x01;
        }

        ee_iic_delay();
        IIC1_SCL(GPIO_PIN_RESET);
    }

    return recv_value;
}

/**
  * @brief  IIC等待应答信号
  * @param  None
  * @retval ack_status： 应答状态，0表示应答，1表示设备无响应
  */
uint8_t ee_iic_wait_ack(void)
{
    uint8_t ack_status = 0;

    /** 在等待应答信号之前，要释放总线，即将SDA置位 */
    IIC1_SDA(GPIO_PIN_SET);
    ee_iic_delay();
    IIC1_SCL(GPIO_PIN_SET);
    ee_iic_delay();

    if (IIC1_SDA_IS_HIGH())
    {
        ack_status = 1;
        ee_iic_stop();
    }
    else
    {
        ack_status = 0;
    }

    IIC1_SCL(GPIO_PIN_RESET);
    ee_iic_delay();

    return ack_status;
}

/**
  * @brief  主机（主控制器）产生应答信号
  * @param  None
  * @retval None
  */
void ee_iic_ack(void)
{
    IIC1_SDA(GPIO_PIN_RESET);
    ee_iic_delay();

    IIC1_SCL(GPIO_PIN_SET);
    ee_iic_delay();
    IIC1_SCL(GPIO_PIN_RESET);
    ee_iic_delay();

    IIC1_SDA(GPIO_PIN_SET);
}

/**
  * @brief  主机（主控制器）产生不应答信号
  * @param  None
  * @retval None
  */
void ee_iic_nack(void)
{
    IIC1_SDA(GPIO_PIN_SET);
    ee_iic_delay();

    IIC1_SCL(GPIO_PIN_SET);
    ee_iic_delay();
    IIC1_SCL(GPIO_PIN_RESET);
    ee_iic_delay();
}

/**
  * @brief  检测IIC总线上的设备状态
  * @param  device_addr： 从机设备地址
  * @retval ack_status： 0 （正常）or 1（异常）
  * @note   主机发送设备地址等待从机应答，若有从机正确的应答信号
            则表明IIC总线上挂接了设备，否则表示IIC总线上未检测到
            设备
  */
uint8_t ee_iic_check_device_status(uint8_t device_addr)
{
    uint8_t ack_status;

    if (IIC1_SCL_IS_HIGH() && IIC1_SDA_IS_HIGH())
    {
        ee_iic_start();

        ee_iic_sendbyte(device_addr | IIC_WRITE);
        ack_status = ee_iic_wait_ack();

        ee_iic_stop();

        return ack_status;
    }

    return 1;
}

/** 私有函数 --------------------------------------------------- */
/**
  * @brief  用于模拟IIC时的简单延时
  * @param  None
  * @retval None
  */
static void ee_iic_delay(void)
{
    uint8_t i = 0;
    uint8_t delay = 5;

    while (delay--)
    {
        i = 10;
        while (i--);
    }
}
