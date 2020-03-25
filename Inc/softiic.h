#ifndef __SOFTIIC_H
#define __SOFTIIC_H	 
#include "stm32f1xx_hal.h"
#include "main.h"

#define IIC_SCL_Pin         GPIO_PIN_8
#define IIC_SCL_GPIO_Port   GPIOB
#define IIC_SDA_Pin         GPIO_PIN_9
#define IIC_SDA_GPIO_Port   GPIOB

void SoftIIC_Port_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Send_Byte(uint8_t txd);
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);


#endif
