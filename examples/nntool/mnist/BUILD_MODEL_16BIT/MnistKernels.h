#ifndef __MNISTKERNEL_H__
#define __MNISTKERNEL_H__

#include "AutoTilerLibTypes.h"
#include "CNN_BasicKernels.h"
#include "Mnist.h"
#define _L1_Memory_SIZE 49312
#define _Mnist_L2_Memory_SIZE 32256
extern char *L1_Memory; /* Size given for generation: 52000 bytes, used: 49312 bytes */
extern char *Mnist_L2_Memory; /* Size used for generation: 32256 bytes */
extern void S1_Conv2d_32x1x5x5_MaxPool_2x2_Relu(
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
extern void S3_Linear_10x64x4x4(
		short int * __restrict__ In,
		short int * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias);
extern void S4_SoftMax(
		short int * __restrict__ In,
		short int * __restrict__ Out,
		unsigned int Norm);
extern int MnistCNN_Construct();
extern int MnistCNN_Destruct();
extern int MnistCNN(
		short int *__restrict__ Input_1,
		short int *__restrict__ Output_1);
extern unsigned int MnistPerf[4];
extern char * MnistLName[4];
extern unsigned int MnistOperCount[4];
#endif
