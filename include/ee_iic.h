#include "FreeRTOS.h"
#include "main.h"

#define IIC_WRITE 0
#define IIC_READ 1

typedef enum
{
  GPIO_PIN_RESET = 0u,
  GPIO_PIN_SET
} GPIO_PinState;


void IIC1_SCL(GPIO_PinState pin_status);
void IIC1_SDA(GPIO_PinState pin_status);

void ee_iic_init(void);
void ee_iic_start(void);
void ee_iic_stop(void);
extern void ee_iic_sendbyte(uint8_t byte);
extern uint8_t ee_iic_readbyte(void);
extern uint8_t ee_iic_wait_ack(void);
extern void ee_iic_ack(void);
extern void ee_iic_nack(void);
extern uint8_t ee_iic_check_device_status(uint8_t device_addr);
