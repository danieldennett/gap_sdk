#ifndef __MNISTKERNEL_H__
#define __MNISTKERNEL_H__

#include "AutoTilerLibTypes.h"
#include "CNN_BasicKernels.h"
#include "mnist.h"
#define _L1_Memory_SIZE 47680
#define _mnist_L2_Memory_SIZE 35240
extern char *L1_Memory; /* Size given for generation: 50784 bytes, used: 47680 bytes */
extern char *mnist_L2_Memory; /* Size used for generation: 35240 bytes */
extern void S1_Conv2d_32x1x5x5_Relu(
		short int * __restrict__ In,
		short int * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias);
extern void S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu(
		short int * __restrict__ In,
		short int * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias);
extern void S3_MaxPool_2x2(
		short int * __restrict__ In,
		short int * __restrict__ Out);
extern void S4_Linear_10x64x5x5(
		short int * __restrict__ In,
		short int * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias);
extern void S5_SoftMax(
		short int * __restrict__ In,
		short int * __restrict__ Out,
		unsigned int Norm);
extern int mnistCNN_Construct();
extern int mnistCNN_Destruct();
extern int mnistCNN(
		short int *__restrict__ Input_1,
		short int *__restrict__ Output_1);
extern unsigned int AT_GraphOperInfosNames[5];
#endif
