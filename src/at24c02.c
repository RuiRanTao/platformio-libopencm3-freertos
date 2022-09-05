/**
  * @file  at24c02.c
  * @brief at24c02驱动程序
  * @par   date        version    author    remarks
  *        2016-03-21  v1.0       zbt       初次创建
  *
  */

/** 头文件包含区 ------------------------------------------------ */
#include "at24c02.h"
#include "ee_iic.h"
#include "main.h"
#include "stdio.h"
#include "task.h"

/** 私有宏(类型定义) -------------------------------------------- */
#define AT24C02_DEVICE_ADDR         0xA0
#define AT24C02_PAGE_SIZE           8
#define AT24C02_MEM_SIZE            256
#define AT24C02_ADDR_BYTE           1

/** 私有变量 --------------------------------------------------- */
uint8_t test_buffer[AT24C02_MEM_SIZE];

/** 外部变量 --------------------------------------------------- */

/** 私有函数原型 ----------------------------------------------- */
//static void AT24C02_ack(void);
static void AT24C02_error_handle(void);
static void AT24C02_read_test(void);
static void AT24C02_write_test(void);
static void AT24C02_erase_test(void);

/** 公有函数 --------------------------------------------------- */
/**
  * @brief  AT24C02与主控制器的IIC通讯测试代码
  * @param  None
  * @retval None
  */
void AT24C02_iic_test(void)
{
    ee_iic_stop();     /**< 必须先复位IIC总线上的设备到待机模式 */
    vTaskDelay(pdMS_TO_TICKS(10));
    /** 检测总线上是否挂接了IIC设备（此处为AT24C02） */
    if (ee_iic_check_device_status(AT24C02_DEVICE_ADDR) == 0)
    {
        printf("iic device exists\n");
    }
    else
    {
        printf("no iic device exists\n");
    }
    AT24C02_write_test();
    vTaskDelay(pdMS_TO_TICKS(10));
    AT24C02_read_test();
    vTaskDelay(pdMS_TO_TICKS(10));
    AT24C02_erase_test();
}

/**
  * @brief  从AT24C02中读取数据
  * @param  read_data： 读取到的数据
  * @param  start_addr： 读取数据的起始地址
  * @param  data_length： 数据的长度
  * @retval None
  */
void AT24C02_read_data(uint8_t *read_data, uint16_t start_addr, uint16_t data_length)
{
    uint16_t i;

    ee_iic_start();
    ee_iic_sendbyte(AT24C02_DEVICE_ADDR | IIC_WRITE);
//    AT24C02_ack();
    if (ee_iic_wait_ack() != 0)
    {
        AT24C02_error_handle();
        printf("first read error\r\n");
    }

    if (AT24C02_ADDR_BYTE == 1)
    {
        ee_iic_sendbyte((uint8_t)start_addr & 0xff);
//        AT24C02_ack();
        if (ee_iic_wait_ack() != 0)
        {
            AT24C02_error_handle();
            printf("addr byte error\r\n");
        }
    }

    ee_iic_start();
    ee_iic_sendbyte(AT24C02_DEVICE_ADDR | IIC_READ);
//    AT24C02_ack();
    if (ee_iic_wait_ack() != 0)
    {
        AT24C02_error_handle();
        printf("read data error\r\n");
    }

    for (i = 0; i < data_length; i++)
    {
        read_data[i] = ee_iic_readbyte();

        if (i != data_length - 1)
        {
            ee_iic_ack();      /**< 读完非最后一个字节后， 产生应答信号 */
        }
        else
        {
            ee_iic_nack();     /**< 最后一个字节读完后 产生非应答信号 */
        }
    //    printf("read data is %d\n", read_data[i]);  /**< 调试代码 */
    }

    ee_iic_stop();
}

/**
  * @brief  通过IIC向AT24C02写数据
  * @param  write_data：  要写入AT24C02的数据指针
  * @param  start_addr：  要写入AT24C02的起始地址
  * @param  data_length： 要写入AT24C02的数据长度
  * @retval None
  */
void AT24C02_write_data(uint8_t *write_data, uint16_t start_addr, uint16_t data_length)
{
    uint16_t i, j;
    uint16_t start_addr_dup;

    start_addr_dup = start_addr;
    for (i = 0; i < data_length; i++)
    {

        if ((i == 0) || (start_addr_dup & (AT24C02_PAGE_SIZE - 1)) == 0)
        {
            ee_iic_stop();

            for (j = 0; j < 2000; j++)
            {
                ee_iic_start();
                ee_iic_sendbyte(AT24C02_DEVICE_ADDR | IIC_WRITE);

                if (ee_iic_wait_ack() == 0)
                {
                    break;
                }
            }
            if (j >= 2000)
            {
                printf("j = 1000\r\n");
                AT24C02_error_handle();
            }

            if (AT24C02_ADDR_BYTE == 1)
            {
                ee_iic_sendbyte((uint8_t)start_addr_dup & 0xff);
//                AT24C02_ack();
                if (ee_iic_wait_ack() != 0)
                {
                    AT24C02_error_handle();
                    printf("addr_byte wrong\r\n");
                }
            }
        }

        ee_iic_sendbyte(write_data[i]);
//        AT24C02_ack();
        if (ee_iic_wait_ack() != 0)
        {
            AT24C02_error_handle();
            printf("write failed\r\n");
        }

//        printf("write_data is %d \n", write_data[i]);    /**< 调试代码 */
        start_addr_dup++;
    }

    ee_iic_stop();
}

/** 私有函数 --------------------------------------------------- */
/** 以下为测试用程序 ------------------------------------------- */
/**
  * @brief  AT24C02读取数据测试
  * @param  None
  * @retval None
  */
static void AT24C02_read_test(void)
{
    uint16_t i;

    AT24C02_read_data(test_buffer, 0, (AT24C02_MEM_SIZE % 10));

    printf("read data is:\n");
    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)
    {
        printf("%d ", test_buffer[i]);
    }

    printf("\r\nread test ok\r\n");
}

/**
  * @brief  AT24C02写数据测试
  * @param  None
  * @retval None
  */
static void AT24C02_write_test(void)
{
    uint16_t i;

    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)
    {
        test_buffer[i] = i;
    }

    AT24C02_write_data(test_buffer, 0, (AT24C02_MEM_SIZE % 10));

    printf("write data is:\n");
    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)
    {
        printf("%d ", test_buffer[i]);
    }

    printf("\r\nwrite test ok\r\n");
}

/**
  * @brief  AT24C02擦除数据测试
  * @param  None
  * @retval None
  */
static void AT24C02_erase_test(void)
{
    uint16_t i;

    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)
    {
        test_buffer[i] = 0xff;
    }

    AT24C02_write_data(test_buffer, 0, (AT24C02_MEM_SIZE % 10));

    printf("erase value is: \n");
    for (i = 0; i < (AT24C02_MEM_SIZE % 10); i++)
    {
        printf("%d ", test_buffer[i]);
    }

    printf("\r\nerase test ok\r\n");
}
/** 以上为测试用程序 ------------------------------------------- */

/**
  * @brief  AT24C02应答错误处理程序
  * @param  None
  * @retval None
  */
static void AT24C02_error_handle(void)
{
    ee_iic_stop();
//    printf("At24C02 read failed\r\n");   /**< 调试用 */
}

//static void AT24C02_ack(void)
//{
//    if (ee_iic_wait_ack() != 0)
//    {
//        AT24C02_error_handle();
//    }
//}

