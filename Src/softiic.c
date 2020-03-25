#include "softiic.h"
#include "gpio.h"
#include "tim.h"

#define IIC_SDA(val) {if(val)	HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_PIN_SET);\
						else	HAL_GPIO_WritePin(IIC_SDA_GPIO_Port,IIC_SDA_Pin,GPIO_PIN_RESET);}
#define IIC_SCL(val) {if(val)	HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin,GPIO_PIN_SET);\
						else	HAL_GPIO_WritePin(IIC_SCL_GPIO_Port,IIC_SCL_Pin,GPIO_PIN_RESET);}

#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(uint32_t)8<<4;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(uint32_t)3<<4;} 

void IIC_Delay(uint16_t times)
{
	for(uint16_t _n=0;_n<times*5;_n++);
}

void SoftIIC_Port_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct;

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOB_CLK_ENABLE();

	  /*Configure GPIO pins : SDA */
	  GPIO_InitStruct.Pin = IIC_SDA_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(IIC_SDA_GPIO_Port, &GPIO_InitStruct);

	  /*Configure GPIO pin : SCLK */
	  GPIO_InitStruct.Pin = IIC_SCL_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(IIC_SCL_GPIO_Port, &GPIO_InitStruct);


}

void IIC_Start(void)
{
	SDA_OUT();
	IIC_SDA(1);
	IIC_SCL(1);
	IIC_Delay(1);
 	IIC_SDA(0);//START:when CLK is high,DATA change form high to low
 	IIC_Delay(1);
	IIC_SCL(0);
}

void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL(0);
	IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
	IIC_Delay(1);
	IIC_SCL(1);
	IIC_SDA(1);
	IIC_Delay(1);
}

uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();
	IIC_SDA(1);IIC_Delay(1);
	IIC_SCL(1);IIC_Delay(1);
	while(HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port,IIC_SDA_Pin))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);
	return 0;
}
//
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	IIC_Delay(1);
	IIC_SCL(1);
	IIC_Delay(1);
	IIC_SCL(0);
}

void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	IIC_Delay(1);
	IIC_SCL(1);
	IIC_Delay(1);
	IIC_SCL(0);
}

void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
	SDA_OUT();
    IIC_SCL(0);
    for(t=0;t<8;t++)
    {
        if((txd&0x80)>>7)
        {
        	IIC_SDA(1);
        }
        else
        {
        	IIC_SDA(0);
        }
        txd<<=1;
        IIC_Delay(1);
		IIC_SCL(1);
		IIC_Delay(1);
		IIC_SCL(0);
		IIC_Delay(1);
    }
}

uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();
    for(i=0;i<8;i++ )
	{
        IIC_SCL(0);
        IIC_Delay(1);
		IIC_SCL(1);
        receive<<=1;
        if(HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port,IIC_SDA_Pin)) receive++;
        IIC_Delay(1);
    }
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}

