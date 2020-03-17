#ifndef SERIALPLOT_H_
#define SERIALPLOT_H_
#ifdef __cplusplus
	extern "C"{
#endif
		
		#include "stm32f1xx_hal.h"		
		
typedef struct{
	uint8_t frameHeader1;
	uint8_t frameHeader2;
	int16_t	fdata[3];
}__attribute__((packed))SerialPlotFrame;
		
extern SerialPlotFrame frame;		

#ifdef __cplusplus
	}
#endif
#endif
