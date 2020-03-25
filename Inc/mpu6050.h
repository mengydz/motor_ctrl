#ifndef __MPU6050_H_
#define __MPU6050_H_

#include "stm32f1xx_hal.h"
#include "vectors.h"
//#define MPU6050_WHO_AM_I_LENGTH     6

typedef struct{
	Vector3i16	gyro;
	Vector3i16	acce;
	float 		temperature;
}IMUData;

typedef struct{
	Vector3f	gyro;
	Vector3f	acce;
	float 		temperature;
}FIMUData;


#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     MPU6050_ADDRESS_AD0_LOW
#define MpuDevAddr  0xD0

#define MPU6050_RA_SMPLRT_DIV       0x19
#define MPU6050_RA_CONFIG           0x1A
#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_ACCEL_CONFIG     0x1C
#define MPU6050_RA_FIFO_EN          0x23
#define MPU6050_RA_INT_PIN_CFG      0x37
#define MPU6050_RA_INT_ENABLE       0x38
#define MPU6050_RA_USER_CTRL        0x6A
#define MPU6050_RA_PWR_MGMT_1       0x6B

#define	WHO_AM_I					0x75	//IIC鍦板潃瀵勫瓨鍣�(榛樿鏁板��0x68锛屽彧璇�)


extern uint8_t mpu6050_buffer[14];					//iic璇诲彇鍚庡瓨鏀炬暟鎹�

int8_t MPU6050_SelfTest(void);
void MPU6050_Init(void);
void Single_ReadI2C(uint8_t REG_Address);
void Single_WriteI2C(uint8_t REG_Address,uint8_t REG_data);
void MPU6050_ReadValue(void);
void HeatCtrPlus(uint8_t _plus);

extern IMUData BaseMpu;

#endif
