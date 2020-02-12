#ifndef __MNISTKERNEL_H__
#define __MNISTKERNEL_H__

#include "AutoTilerLibTypes.h"
#include "CNN_BasicKernels.h"
#include "mnist.h"
#define _L1_Memory_SIZE 48496
#define _mnist_L2_Memory_SIZE 18848
extern char *L1_Memory; /* Size given for generation: 50784 bytes, used: 48496 bytes */
extern char *mnist_L2_Memory; /* Size used for generation: 18848 bytes */
extern void S1_Conv2d_32x1x5x5_Relu(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias);
extern void S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias);
extern void S3_MaxPool_2x2(
		signed char * __restrict__ In,
		signed char * __restrict__ Out);
extern void S4_Linear_10x64x5x5(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias);
extern void S5_SoftMax(
		signed char * __restrict__ In,
		short int * __restrict__ Out,
		unsigned int Norm);
extern int mnistCNN_Construct();
extern int mnistCNN_Destruct();
extern int mnistCNN(
		signed char *__restrict__ Input_1,
		short int *__restrict__ Output_1);
extern unsigned int AT_GraphPerf[5];
extern char * AT_GraphNodeNames[5];
extern unsigned int AT_GraphOperInfosNames[5];
#endif
