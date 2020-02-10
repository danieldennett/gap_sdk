#ifndef __INTEGRALIMGKERNEL_H__
#define __INTEGRALIMGKERNEL_H__

#include "AutoTilerLibTypes.h"
#include "IntegralImgBasicKernels.h"
#define _II_L1_Memory_SIZE 39284
#define _II_L2_Memory_SIZE 0
extern char *II_L1_Memory; /* Size given for generation: 40000 bytes, used: 39284 bytes */
extern char *II_L2_Memory; /* Size used for generation: 0 bytes */
extern void BodyIntegralImage(
		unsigned char *  __restrict__ ImageIn,
		unsigned int *  __restrict__ IntegralImage);
#endif
