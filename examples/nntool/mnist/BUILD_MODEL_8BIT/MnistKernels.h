#ifndef __MNISTKERNEL_H__
#define __MNISTKERNEL_H__

#include "AutoTilerLibTypes.h"
#include "CNN_BasicKernels.h"
#include "Mnist.h"
#define _L1_Memory_SIZE 44448
#define _Mnist_L2_Memory_SIZE 20048
extern char *L1_Memory; /* Size given for generation: 52000 bytes, used: 44448 bytes */
extern char *Mnist_L2_Memory; /* Size used for generation: 20048 bytes */
extern void S1_Conv2d_32x1x5x5_Relu(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias);
extern void S2_Conv2d_32x32x5x5_MaxPool_2x2_Relu(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias);
extern void S3_MaxPool_2x2(
		signed char * __restrict__ In,
		signed char * __restrict__ Out);
extern void S4_Linear_10x32x5x5(
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
extern int MnistCNN_Construct();
extern int MnistCNN_Destruct();
extern int MnistCNN(
		signed char *__restrict__ Input_1,
		short int *__restrict__ Output_1);
extern unsigned int MnistPerf[5];
extern char * MnistLName[5];
extern unsigned int MnistOperCount[5];
#endif
