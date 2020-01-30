#include "MnistKernels.h"
L1_CL_MEM AT_L1_POINTER L1_Memory;
L2_MEM AT_L2_POINTER Mnist_L2_Memory;
static AT_HYPERFLASH_FS_T HyperFlash;
void  __attribute__ ((noinline)) S1_Conv2d_32x1x5x5_MaxPool_2x2_Relu(
		short int * __restrict__ In,
		short int * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 49312 bytes, L2 buffer: 12448 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_fp_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_fp_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerReLUPool_fp_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Last;
	int T0Ind, T0Ind_Last;
	int D0Ind, D0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: 1][Tile0 Dim: 1][D0 Dim: 1]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 36864 [D1, 1 x 36864][Tile0, 1:[24x24], 2]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 36864][Tile0, 1:[24x24], 2]
		Tile0: [0, 36864, 36864], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 0], T2: [D1: 0][Tile0: 0]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [D1, 1 x 64]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 64]
		Tile0: [0, 64, 64], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0], T1: [D1: 0], T2: [D1: 0]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1600 [D1, 1 x 1600][D0, 1 x 1600]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 1600][D0, 1 x 1600]
		Tile0: [0, 1600, 1600], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][D0: 0], T1: [D1: 0][D0: 0], T2: [D1: 0][D0: 0]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 9216 [D1, 1 x 9216][Tile0, 1:[12x12], 2]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 9216][Tile0, 1:[12x12], 2]
		Tile0: [0, 9216, 9216], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 0], T2: [D1: 0][Tile0: 0]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1568 [D0, 1 x 1568][Tile0, 1:[28x28], 2]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[28x28], 2][D0, 1 x 1568]
		Tile0: [0, 1568, 1568], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (short int * __restrict__) (L1_Memory+12448);
	KerArg0->W = (unsigned short int) (24);
	KerArg0->H = (unsigned short int) (24);
	KerArg0->OutFeatures = (unsigned short int) (32);
	KerArg0->Bias = (short int * __restrict__) (L1_Memory+1568);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg1->In = (short int * __restrict__) (L1_Memory+0);
	KerArg1->W = (unsigned short int) (28);
	KerArg1->UsedW = (unsigned short int) (28);
	KerArg1->InFeatures = (unsigned short int) (1);
	KerArg1->OutFeatures = (unsigned short int) (32);
	KerArg1->Filter = (short int * __restrict__) (L1_Memory+1632);
	KerArg1->Out = (short int * __restrict__) (L1_Memory+12448);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->TotalInFeatures = (short int) (1);
	KerArg1->Pad = (v4s) 0;
	KerArg1->Orientation = (unsigned char) (1);
	KerArg2->In = (short int * __restrict__) (L1_Memory+12448);
	KerArg2->W = (unsigned short int) (24);
	KerArg2->UsedW = (unsigned short int) (24);
	KerArg2->H = (unsigned short int) (24);
	KerArg2->UsedH = (unsigned short int) (24);
	KerArg2->OutFeatures = (unsigned short int) (32);
	KerArg2->Out = (short int * __restrict__) (L1_Memory+3232);
	KerArg2->Pad = (v4s) 0;
	KerArg2->Orientation = (unsigned char) (1);
	KerArg2->Oper = (unsigned char) (1);
	KerArg2->LB = (int) (0);
	KerArg2->UB = (int) (32767);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+1568), 64, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+1632), 1600, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0), 1568, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			AT_FORK(gap_ncore(), (void *) KerParSetBias_fp, (void *) KerArg0);
			__CALL(KerParSetBias_fp, KerArg0);
			{ /* Single iteration on D0 */
				int D0Ind_Last = 1;
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->H = (unsigned short int) (28);
				KerArg1->UsedH = (unsigned short int) (28);
				AT_FORK(gap_ncore(), (void *) KerParConv5x5Stride1_fp, (void *) KerArg1);
				__CALL(KerParConv5x5Stride1_fp, KerArg1);
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			AT_FORK(gap_ncore(), (void *) KerParPool2x2Stride2_fp, (void *) KerArg2);
			__CALL(KerParPool2x2Stride2_fp, KerArg2);
		} /* End iteration on Tile0 */
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+3232), 9216, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu(
		short int * __restrict__ In,
		short int * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 40448 bytes, L2 buffer: 32256 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_fp_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_fp_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerReLUPool_fp_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Total=0, D1Ind_Last, D1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	unsigned int _N_Bias;
	unsigned int _SN_Bias;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _LN_Filter;
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _LN_In;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: 1][Tile0 Dim: 1][D0 Dim: 8]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2048 [D1, 1 x 2048][Tile0, 1:[4x4], 2]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 2048][Tile0, 1:[4x4], 2]
		Tile0: [0, 2048, 32], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 0], T2: [D1: 0][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 128 [D1, 1 x 128]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 128]
		Tile0: [0, 128, 128], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0], T1: [D1: 0], T2: [D1: 0]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 102400 [D1, 1 x 102400][D0, 8 x 12800]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 102400][D0, 8 x 12800]
		Tile0: [0, 12800, 200], Tile1: [12800, 12800, 200], Tile2; [25600, 12800, 200]
		T0: [D1: 0][D0: 0], T1: [D1: 0][D0: 1], T2: [D1: 0][D0: 2]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 9216 [D0, 8 x 1152][Tile0, 1:[12x12], 2]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[12x12], 2][D0, 8 x 1152]
		Tile0: [0, 1152, 288], Tile1: [1152, 1152, 288], Tile2; [2304, 1152, 288]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 8192 [D1, 1 x 8192][Tile0, 1:[8x8], 2]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 8192][Tile0, 1:[8x8], 2]
		Tile0: [0, 8192, 8192], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 0], T2: [D1: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (short int * __restrict__) (L1_Memory+32256);
	KerArg0->W = (unsigned short int) (8);
	KerArg0->H = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (64);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg1->W = (unsigned short int) (12);
	KerArg1->UsedW = (unsigned short int) (12);
	KerArg1->H = (unsigned short int) (12);
	KerArg1->UsedH = (unsigned short int) (12);
	KerArg1->InFeatures = (unsigned short int) (4);
	KerArg1->OutFeatures = (unsigned short int) (64);
	KerArg1->Out = (short int * __restrict__) (L1_Memory+32256);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->TotalInFeatures = (short int) (4);
	KerArg1->Pad = (v4s) 0;
	KerArg1->Orientation = (unsigned char) (1);
	KerArg2->In = (short int * __restrict__) (L1_Memory+32256);
	KerArg2->W = (unsigned short int) (8);
	KerArg2->UsedW = (unsigned short int) (8);
	KerArg2->H = (unsigned short int) (8);
	KerArg2->UsedH = (unsigned short int) (8);
	KerArg2->OutFeatures = (unsigned short int) (64);
	KerArg2->Pad = (v4s) 0;
	KerArg2->Orientation = (unsigned char) (1);
	KerArg2->Oper = (unsigned char) (1);
	KerArg2->LB = (int) (0);
	KerArg2->UB = (int) (32767);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=2048; _LC_Out=32;
	_SP_Out=0;
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+2304+0), 128, 0, &DmaR_Evt1);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+2560+0), 12800, 0, &DmaR_Evt2);
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 1152, 288, 288, 0, &DmaR_Evt3);
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1, D1Ind_NextLast = 1;
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+2304+128*((D1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->Bias = (short int * __restrict__) (L1_Memory+2304+128*((D1Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParSetBias_fp, (void *) KerArg0);
			__CALL(KerParSetBias_fp, KerArg0);
			for (D0Ind=0; D0Ind<8; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==7), D0Ind_NextLast = ((D0Ind+1)==7);
				/*================================= Prepare Tiles ===================================*/
				_SN_Filter = 0;
				if (!(D0Ind_Last)) {
					_N_Filter = _N_Filter + (12800); _LN_Filter = (200); _SN_Filter = (64*_LN_Filter); 
				} else if (!((1))) {
					_N_Filter = _N_Filter + (-89600); _LN_Filter = (200); _SN_Filter = (64*_LN_Filter); 
				}
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (1152); _LN_In = (288); _SN_In = (4*_LN_In); 
				} else if (!(1)) {
					_N_In = _N_In + (-8064); _LN_In = (288); _SN_In = (4*_LN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
				if (_SN_Filter) {
					AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+2560+12800*((D0Ind_Total+1)%2)),
							_SN_Filter, 0, &DmaR_Evt2);
				}
				AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+1152*((D0Ind_Total+1)%2)),
							_SN_In, 288, _LN_In, 0, &DmaR_Evt3);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (short int * __restrict__) (L1_Memory+0+1152*((D0Ind_Total)%2));
				KerArg1->Filter = (short int * __restrict__) (L1_Memory+2560+12800*((D0Ind_Total)%2));
				AT_FORK(gap_ncore(), (void *) KerParConv5x5Stride1_fp, (void *) KerArg1);
				__CALL(KerParConv5x5Stride1_fp, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				
				
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->Out = (short int * __restrict__) (L1_Memory+28160+2048*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParPool2x2Stride2_fp, (void *) KerArg2);
			__CALL(KerParPool2x2Stride2_fp, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+28160+2048*((T0Ind_Total)%2)),
					_SC_Out, 32, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		/*============================= End Update Arg Pipeline =============================*/
		D1Ind_Total++;
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) S3_Linear_10x64x4x4(
		short int * __restrict__ In,
		short int * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 22568 bytes, L2 buffer: 22568 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerLinearLayerReLU_fp_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int D0Ind, D0Ind_Last;
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D0 Dim: 1][Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2048 [Tile0, 1:[1x1], 2048]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 2048]
		Tile0: [0, 2048, 2048], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 20480 [D0, 1 x 20480]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 1 x 20480]
		Tile0: [0, 20480, 20480], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0], T1: [D0: 0], T2: [D0: 0]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 20 [D0, 1 x 20]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 1 x 20]
		Tile0: [0, 20, 20], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0], T1: [D0: 0], T2: [D0: 0]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 20 [D0, 1 x 20]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 1 x 20]
		Tile0: [0, 20, 20], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0], T1: [D0: 0], T2: [D0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (short int * __restrict__) (L1_Memory+0);
	KerArg0->InSize = (unsigned short int) (1024);
	KerArg0->TotalInSize = (unsigned short int) (1024);
	KerArg0->OutSize = (unsigned short int) (10);
	KerArg0->Filter = (short int * __restrict__) (L1_Memory+2048);
	KerArg0->Bias = (short int * __restrict__) (L1_Memory+22528);
	KerArg0->Out = (short int * __restrict__) (L1_Memory+22548);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->LB = (int) (-32768);
	KerArg0->UB = (int) (32767);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0), 2048, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+2048), 20480, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+22528), 20, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			AT_FORK(gap_ncore(), (void *) KerParLinearLayerReLU_fp, (void *) KerArg0);
			__CALL(KerParLinearLayerReLU_fp, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+22548), 20, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) S4_SoftMax(
		short int * __restrict__ In,
		short int * __restrict__ Out,
		unsigned int Norm)

{
	/* Shared L1: 40 bytes, L2 buffer: 40 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerSoftMax_fp_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 20 [Tile0, 1:[1x10], 2]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x10], 2]
		Tile0: [0, 20, 20], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 20 [Tile0, 1:[1x10], 2]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x10], 2]
		Tile0: [0, 20, 20], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (short int *__restrict__) (L1_Memory+0);
	KerArg0->Norm = (unsigned short int) (Norm);
	KerArg0->Out = (short int *__restrict__) (L1_Memory+20);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0), 20, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		KerArg0->N = (unsigned short int) (10);
		AT_FORK(gap_ncore(), (void *) KerParSoftMax_fp, (void *) KerArg0);
		__CALL(KerParSoftMax_fp, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+20), 20, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
int MnistCNN_Construct()

{
	AT_HYPERFLASH_FS_FC_EVENT Uchan1;
	AT_HYPERFLASH_FS_CONF_T HyperFlashConf;
	int Error;
	AT_HYPERFLASH_FS_CONF_INIT(&HyperFlashConf, AT_MEM_L3_HFLASH, 0);
	AT_HYPERFLASH_FS_OPEN(&HyperFlash, &HyperFlashConf, "Mnist_L3_Flash_Const.dat", &Error);
	if (Error) return 1;
	Mnist_L2_Memory = (AT_L2_POINTER) AT_L2_ALLOC(0, 135956);
	L1_Memory = (AT_L1_POINTER) AT_L1_ALLOC(0, 49312);
	/* Moving Step1Weights, size 1600 from HyperFlash at 122880 to (size 1600) L2 at 134144 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 122880), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 134144), 1600, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step1Biases, size 64 from HyperFlash at 124608 to (size 64) L2 at 135872 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 124608), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 135872), 64, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step2Weights, size 102400 from HyperFlash at 0 to (size 102400) L2 at 11264 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 11264), 102400, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step2Biases, size 128 from HyperFlash at 124480 to (size 128) L2 at 135744 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 124480), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 135744), 128, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step3Weights, size 20480 from HyperFlash at 102400 to (size 20480) L2 at 113664 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 102400), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 113664), 20480, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step3Biases, size 20 from HyperFlash at 124672 to (size 20) L2 at 135936 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 124672), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 135936), 20, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	return 0;
}
int MnistCNN_Destruct()

{
	AT_L2_FREE(0, Mnist_L2_Memory, 135956);
	AT_L1_FREE(0, L1_Memory, 49312);
	AT_HYPERFLASH_FS_CLOSE(&HyperFlash);
	return 0;
}
unsigned int MnistPerf[4];
unsigned int MnistOperCount[4] = {
	483840,
	3281920,
	10240,
	10,
};
char *MnistLName[4] = {
	"S1_Conv2d_32x1x5x5_MaxPool_2x2_Relu",
	"S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu",
	"S3_Linear_10x64x4x4",
	"S4_SoftMax",
};
int MnistCNN(
		short int *__restrict__ Input_1,
		short int *__restrict__ Output_1)

{
	MnistPerf[0] = gap_cl_readhwtimer();
	S1_Conv2d_32x1x5x5_MaxPool_2x2_Relu(
		(short int *__restrict__) Input_1, /* In */
		(short int *__restrict__) (Mnist_L2_Memory+134144), /* Filter */
		(short int *__restrict__) (Mnist_L2_Memory+135872), /* Bias */
		(short int *__restrict__) (Mnist_L2_Memory+2048), /* Out */
		16, /* Norm */
		16 /* NormBias */
	);
	MnistPerf[0] = gap_cl_readhwtimer() - MnistPerf[0];
	MnistPerf[1] = gap_cl_readhwtimer();
	S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu(
		(short int *__restrict__) (Mnist_L2_Memory+2048), /* In */
		(short int *__restrict__) (Mnist_L2_Memory+11264), /* Filter */
		(short int *__restrict__) (Mnist_L2_Memory+135744), /* Bias */
		(short int *__restrict__) (Mnist_L2_Memory+0), /* Out */
		16, /* Norm */
		16 /* NormBias */
	);
	MnistPerf[1] = gap_cl_readhwtimer() - MnistPerf[1];
	MnistPerf[2] = gap_cl_readhwtimer();
	S3_Linear_10x64x4x4(
		(short int *__restrict__) (Mnist_L2_Memory+0), /* In */
		(short int *__restrict__) (Mnist_L2_Memory+113664), /* Filter */
		(short int *__restrict__) (Mnist_L2_Memory+135936), /* Bias */
		(short int *__restrict__) (Mnist_L2_Memory+2048), /* Out */
		16, /* Norm */
		16 /* NormBias */
	);
	MnistPerf[2] = gap_cl_readhwtimer() - MnistPerf[2];
	MnistPerf[3] = gap_cl_readhwtimer();
	S4_SoftMax(
		(short int *__restrict__) (Mnist_L2_Memory+2048), /* In */
		(short int *__restrict__) Output_1, /* Out */
		9 /* Norm */
	);
	MnistPerf[3] = gap_cl_readhwtimer() - MnistPerf[3];
	return 0;
}
