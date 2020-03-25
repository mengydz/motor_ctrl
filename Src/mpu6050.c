#include "mpu6050.h"
#include "softiic.h"

uint8_t	mpu6050_buffer[14];
IMUData BaseMpu;

//**************************************
//IIC连续写
//addr:器件地址
//reg:寄存器地址
//len:写入长度
//buf:数据区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    IIC_Start();
    IIC_Send_Byte(addr|0); //发送器件地址+写命令
    if(IIC_Wait_Ack())          //等待应答
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);         //写寄存器地址
    IIC_Wait_Ack();             //等待应答
    for(i=0;i<len;i++)
    {
        IIC_Send_Byte(buf[i]);  //发送数据
        if(IIC_Wait_Ack())      //等待ACK
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_Stop();
    return 0;
}

//IIC连续读
//addr:器件地址
//reg:要读取的寄存器地址
//len:要读取的长度
//buf:读取到的数据存储区
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    IIC_Start();
    IIC_Send_Byte(addr|0); //发送器件地址+写命令
    if(IIC_Wait_Ack())          //等待应答
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);         //写寄存器地址
    IIC_Wait_Ack();             //等待应答
    IIC_Start();
    IIC_Send_Byte(addr|1); //发送器件地址+读命令
    IIC_Wait_Ack();             //等待应答
    while(len)
    {
        if(len==1)*buf=IIC_Read_Byte(0);//读数据,发送nACK
		else *buf=IIC_Read_Byte(1);		//读数据,发送ACK
		len--;
		buf++;
    }
    IIC_Stop();                 //产生一个停止条件
    return 0;
}

//IIC写一个字节
//devaddr:器件IIC地址
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
uint8_t MPU_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data)
{
    IIC_Start();
    IIC_Send_Byte(addr|0); //发送器件地址+写命令
    if(IIC_Wait_Ack())          //等待应答
    {
        IIC_Stop();
        return 1;
    }
    IIC_Send_Byte(reg);         //写寄存器地址
    IIC_Wait_Ack();             //等待应答
    IIC_Send_Byte(data);        //发送数据
    if(IIC_Wait_Ack())          //等待ACK
    {
        IIC_Stop();
        return 1;
    }
    IIC_Stop();
    return 0;
}

//IIC读一个字节
//reg:寄存器地址
//返回值:读到的数据
uint8_t MPU_Read_Byte(uint8_t addr,uint8_t reg)
{
    uint8_t res;
    IIC_Start();
    IIC_Send_Byte(addr|0); //发送器件地址+写命令
    IIC_Wait_Ack();             //等待应答
    IIC_Send_Byte(reg);         //写寄存器地址
    IIC_Wait_Ack();             //等待应答
	  IIC_Start();
    IIC_Send_Byte(addr|1); //发送器件地址+读命令
    IIC_Wait_Ack();             //等待应答
    res=IIC_Read_Byte(0);		//读数据,发送nACK
    IIC_Stop();                 //产生一个停止条件
    return res;
}

//**************************************
void Single_WriteI2C(uint8_t REG_Address,uint8_t REG_data)
{
    MPU_Write_Byte(MpuDevAddr,REG_Address,REG_data);
//    uint8_t txData[2] = {REG_Address,REG_data};
//    while(HAL_I2C_Master_Transmit(&hi2c1,MpuDevAddr,txData,2,5000) != HAL_OK)
//    {
//        if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//        {}
//    }
}
//**************************************

//**************************************
void Single_ReadI2C(uint8_t REG_Address)
{
	MPU_Read_Len(MpuDevAddr,REG_Address,14,&mpu6050_buffer[0]);
//    while(HAL_I2C_Master_Transmit(&hi2c1,MpuDevAddr,&REG_Address,1,5000) != HAL_OK)
//    {
//        if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//        {}
//    }
//
//    if(HAL_I2C_Master_Receive(&hi2c1,MpuDevAddr+1,&mpu6050_buffer[0],14,10000) != HAL_OK)
//    {
//        if(HAL_I2C_GetError(&hi2c1) != HAL_I2C_ERROR_AF)
//        {}
//    }
}

int8_t MPU6050_SelfTest(void)
{
	uint8_t temp;

	temp = MPU_Read_Byte(MpuDevAddr,WHO_AM_I);
	if(temp == MPU6050_ADDRESS_AD0_LOW)
		return 1;
	else
		return 0;
}

void MPU6050_Init(void)
{
	uint8_t dat;
	dat = 0x80;
	Single_WriteI2C(MPU6050_RA_PWR_MGMT_1,dat);
	HAL_Delay(100);
	dat = 0x03;
	Single_WriteI2C(MPU6050_RA_PWR_MGMT_1,dat);
	HAL_Delay(100);
	dat = 0x03;
	Single_WriteI2C(MPU6050_RA_SMPLRT_DIV,dat);
	HAL_Delay(100);
	dat = 0x00;
	Single_WriteI2C(MPU6050_RA_CONFIG,dat);
	HAL_Delay(100);
	dat = 0x10;//1000°
	Single_WriteI2C(MPU6050_RA_GYRO_CONFIG,dat);
	HAL_Delay(100);
	dat = 0x18;//16G
	Single_WriteI2C(MPU6050_RA_ACCEL_CONFIG,dat);
	HAL_Delay(100);
	dat = 0x80;    //0x80 close heating and 0x00 open heating
	Single_WriteI2C(MPU6050_RA_INT_PIN_CFG,dat);
	HAL_Delay(100);

}

void MPU6050_ReadValue(void)
{
	BaseMpu.acce.x 		= (int16_t)( ( (int16_t)mpu6050_buffer[0]<<8)  + mpu6050_buffer[1]  );
	BaseMpu.acce.y 		= (int16_t)( ( (int16_t)mpu6050_buffer[2]<<8)  + mpu6050_buffer[3]  );
	BaseMpu.acce.z		= (int16_t)( ( (int16_t)mpu6050_buffer[4]<<8)  + mpu6050_buffer[5]  );
	BaseMpu.temperature = 36.53 + (int16_t)(((int16_t)mpu6050_buffer[6]<<8) + mpu6050_buffer[7] )/340.0f;
	BaseMpu.gyro.x 		= (int16_t)( ( (int16_t)mpu6050_buffer[8]<<8)  + mpu6050_buffer[9]  );
	BaseMpu.gyro.y 		= (int16_t)( ( (int16_t)mpu6050_buffer[10]<<8) + mpu6050_buffer[11] );
	BaseMpu.gyro.z 		= (int16_t)( ( (int16_t)mpu6050_buffer[12]<<8) + mpu6050_buffer[13] );

}

uint8_t TempCtrPeriod=100;
void HeatCtrPlus(uint8_t _plus)
{
	static uint8_t TempCtrCnt=0;

	if(_plus <= 0)
	{
		TempCtrCnt = 0;
		Single_WriteI2C(MPU6050_RA_INT_PIN_CFG,0x00);//Set INT Pin Low level
	}
	else if(_plus >= 100)
	{
		TempCtrCnt = 0;
		Single_WriteI2C(MPU6050_RA_INT_PIN_CFG,0x80);//Set INT Pin High level
	}
	else
	{
		TempCtrCnt++;
		if(TempCtrCnt == _plus)
		{
			Single_WriteI2C(MPU6050_RA_INT_PIN_CFG,0x00);//Set INT Pin Low level
		}
		if(TempCtrCnt == TempCtrPeriod)
		{
			TempCtrCnt = 0;
			Single_WriteI2C(MPU6050_RA_INT_PIN_CFG,0x80);//Set INT Pin High level
		}
	}

}
