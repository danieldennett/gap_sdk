#include "MnistKernels.h"
L1_CL_MEM AT_L1_POINTER L1_Memory;
L2_MEM AT_L2_POINTER Mnist_L2_Memory;
static AT_HYPERFLASH_FS_T HyperFlash;
void  __attribute__ ((noinline)) S1_Conv2d_32x1x5x5_Relu(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 20048 bytes, L2 buffer: 20048 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerReLUPool_fps_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Last;
	int T0Ind, T0Ind_Last;
	int D0Ind, D0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: 1][Tile0 Dim: 1][D0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 784 [D0, 1 x 784][Tile0, 1:[28x28], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[28x28], 1][D0, 1 x 784]
		Tile0: [0, 784, 784], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [D1, 1 x 32]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 32]
		Tile0: [0, 32, 32], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0], T1: [D1: 0], T2: [D1: 0]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 800 [D1, 1 x 800][D0, 1 x 800]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 800][D0, 1 x 800]
		Tile0: [0, 800, 800], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][D0: 0], T1: [D1: 0][D0: 0], T2: [D1: 0][D0: 0]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 18432 [D1, 1 x 18432][Tile0, 1:[24x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 18432][Tile0, 1:[24x24], 1]
		Tile0: [0, 18432, 18432], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 0], T2: [D1: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+1616);
	KerArg0->W = (unsigned short int) (24);
	KerArg0->OutFeatures = (unsigned short int) (32);
	KerArg0->Bias = (signed char * __restrict__) (L1_Memory+784);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg1->In = (signed char * __restrict__) (L1_Memory+0);
	KerArg1->W = (unsigned short int) (28);
	KerArg1->UsedW = (unsigned short int) (28);
	KerArg1->InFeatures = (unsigned short int) (1);
	KerArg1->OutFeatures = (unsigned short int) (32);
	KerArg1->Filter = (signed char * __restrict__) (L1_Memory+816);
	KerArg1->Out = (signed char * __restrict__) (L1_Memory+1616);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->TotalInFeatures = (short int) (1);
	KerArg1->Pad = (v4s) 0;
	KerArg1->Orientation = (unsigned char) (1);
	KerArg2->In = (signed char * __restrict__) (L1_Memory+1616);
	KerArg2->W = (unsigned short int) (24);
	KerArg2->UsedW = (unsigned short int) (24);
	KerArg2->OutFeatures = (unsigned short int) (32);
	KerArg2->Out = (signed char * __restrict__) (L1_Memory+1616);
	KerArg2->LB = (int) (0);
	KerArg2->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0), 784, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+784), 32, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+816), 800, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Filter */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->H = (unsigned short int) (24);
			AT_FORK(gap_ncore(), (void *) KerParSetBias_fps, (void *) KerArg0);
			__CALL(KerParSetBias_fps, KerArg0);
			{ /* Single iteration on D0 */
				int D0Ind_Last = 1;
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->H = (unsigned short int) (28);
				KerArg1->UsedH = (unsigned short int) (28);
				AT_FORK(gap_ncore(), (void *) KerParConv5x5Stride1_fps, (void *) KerArg1);
				__CALL(KerParConv5x5Stride1_fps, KerArg1);
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->H = (unsigned short int) (24);
			KerArg2->UsedH = (unsigned short int) (24);
			AT_FORK(gap_ncore(), (void *) KerParReLU_fps, (void *) KerArg2);
			__CALL(KerParReLU_fps, KerArg2);
		} /* End iteration on Tile0 */
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+1616), 18432, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 44448 bytes, L2 buffer: 18848 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_fpd_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_DP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerDP_fps_T S_KerArg2, *KerArg2 = &S_KerArg2;
	KerReLUPool_fps_T S_KerArg3, *KerArg3 = &S_KerArg3;

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
		[D1 Dim: 4][Tile0 Dim: 1][D0 Dim: 4]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 6400 [D1, 4 x 1600][Tile0, 1:[10x10], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 4 x 1600][Tile0, 1:[10x10], 1]
		Tile0: [0, 1600, 100], Tile1: [1600, 1600, 100], Tile2; [3200, 1600, 100]
		T0: [D1: 0][Tile0: 0], T1: [D1: 1][Tile0: 0], T2: [D1: 2][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 64 [D1, 4 x 16]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 4 x 16]
		Tile0: [0, 16, 16], Tile1: [16, 16, 16], Tile2; [32, 16, 16]
		T0: [D1: 0], T1: [D1: 1], T2: [D1: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 16 logical tiles, 16 physical tiles
			Total Size: 51200 [D1, 4 x 12800][D0, 4 x 3200]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 4 x 12800][D0, 4 x 3200]
		Tile0: [0, 3200, 200], Tile1: [3200, 3200, 200], Tile2; [6400, 3200, 200]
		T0: [D1: 0][D0: 0], T1: [D1: 0][D0: 1], T2: [D1: 0][D0: 2]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 18432 [D0, 4 x 4608][Tile0, 1:[24x24], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[24x24], 1][D0, 4 x 4608]
		Tile0: [0, 4608, 576], Tile1: [4608, 4608, 576], Tile2; [9216, 4608, 576]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 102400 [D1, 4 x 25600][Tile0, 1:[20x20], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 4 x 25600][Tile0, 1:[20x20], 4]
		Tile0: [0, 102400, 102400], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 0], T2: [D1: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (L1_Memory+18848);
	KerArg0->W = (unsigned short int) (20);
	KerArg0->H = (unsigned short int) (20);
	KerArg0->OutFeatures = (unsigned short int) (16);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg1->W = (unsigned short int) (24);
	KerArg1->UsedW = (unsigned short int) (24);
	KerArg1->H = (unsigned short int) (24);
	KerArg1->UsedH = (unsigned short int) (24);
	KerArg1->InFeatures = (unsigned short int) (8);
	KerArg1->OutFeatures = (unsigned short int) (16);
	KerArg1->Out = (DP_fps_T * __restrict__) (L1_Memory+18848);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->TotalInFeatures = (short int) (8);
	KerArg1->Pad = (v4s) 0;
	KerArg1->Orientation = (unsigned char) (1);
	KerArg2->In = (DP_fps_T * __restrict__) (L1_Memory+18848);
	KerArg2->W = (unsigned short int) (20);
	KerArg2->H = (unsigned short int) (20);
	KerArg2->Out = (signed char * __restrict__) (L1_Memory+18848);
	KerArg2->Norm = (unsigned char) (Norm);
	KerArg2->InFeatures = (unsigned short int) (16);
	KerArg2->LB = (int) (0);
	KerArg2->UB = (int) (127);
	KerArg3->In = (signed char * __restrict__) (L1_Memory+18848);
	KerArg3->W = (unsigned short int) (20);
	KerArg3->UsedW = (unsigned short int) (20);
	KerArg3->H = (unsigned short int) (20);
	KerArg3->UsedH = (unsigned short int) (20);
	KerArg3->OutFeatures = (unsigned short int) (16);
	KerArg3->Pad = (v4s) 0;
	KerArg3->Orientation = (unsigned char) (1);
	KerArg3->Oper = (unsigned char) (1);
	KerArg3->LB = (int) (0);
	KerArg3->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=1600; _LC_Out=100;
	_SP_Out=0;
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9216+0), 16, 0, &DmaR_Evt1);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9248+0), 3200, 0, &DmaR_Evt2);
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 4608, 576, 576, 0, &DmaR_Evt3);
	/*============================= End Read Tiles Prolog ===============================*/
	for (D1Ind=0; D1Ind<4; D1Ind++, D1Ind_Total++) { /* Iteration on D1 */
		int D1Ind_Last = (D1Ind==3), D1Ind_NextLast = ((D1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D1Ind_Last)) {
			_N_Bias = _N_Bias + (16); _SN_Bias = (16); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9216+16*((D1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+9216+16*((D1Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParSetBias_DP_fps, (void *) KerArg0);
			__CALL(KerParSetBias_DP_fps, KerArg0);
			for (D0Ind=0; D0Ind<4; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==3), D0Ind_NextLast = ((D0Ind+1)==3);
				/*================================= Prepare Tiles ===================================*/
				_SN_Filter = 0;
				if (!(D0Ind_Last)) {
					_N_Filter = _N_Filter + (3200); _LN_Filter = (200); _SN_Filter = (16*_LN_Filter); 
				} else if (!((1))) {
					_N_Filter = _N_Filter + (-9600); _LN_Filter = (200); _SN_Filter = (16*_LN_Filter); 
				} else if (!(D1Ind_Last)) {
					_N_Filter = _N_Filter + (12800)+(-9600); _LN_Filter = (200); _SN_Filter = (16*_LN_Filter); 
				}
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (4608); _LN_In = (576); _SN_In = (8*_LN_In); 
				} else if (!(D1Ind_Last)) {
					_N_In = _N_In + (-13824); _LN_In = (576); _SN_In = (8*_LN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
				if (_SN_Filter) {
					AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9248+3200*((D0Ind_Total+1)%2)),
							_SN_Filter, 0, &DmaR_Evt2);
				}
				AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+4608*((D0Ind_Total+1)%2)),
							_SN_In, 576, _LN_In, 0, &DmaR_Evt3);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (L1_Memory+0+4608*((D0Ind_Total)%2));
				KerArg1->Filter = (signed char * __restrict__) (L1_Memory+9248+3200*((D0Ind_Total)%2));
				AT_FORK(gap_ncore(), (void *) KerParConv5x5Stride1_DP_fps, (void *) KerArg1);
				__CALL(KerParConv5x5Stride1_DP_fps, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				
				
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			AT_FORK(gap_ncore(), (void *) KerDP_IO_fps, (void *) KerArg2);
			__CALL(KerDP_IO_fps, KerArg2);
			KerArg3->Out = (signed char * __restrict__) (L1_Memory+15648+1600*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParPool2x2Stride2_fps, (void *) KerArg3);
			__CALL(KerParPool2x2Stride2_fps, KerArg3);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+15648+1600*((T0Ind_Total)%2)),
					_SC_Out, 100, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D1Ind_Last)) {
				_C_Out = _C_Out + (1600); _LC_Out = (100); _SC_Out = (16*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
			T0Ind_Total++;
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		/*============================= End Update Arg Pipeline =============================*/
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) S3_Conv2d_128x64x3x3_MaxPool_2x2_Relu(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 42592 bytes, L2 buffer: 34400 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerReLUPool_fps_T S_KerArg2, *KerArg2 = &S_KerArg2;

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
		[D1 Dim: 1][Tile0 Dim: 1][D0 Dim: 6]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 2048 [D1, 1 x 2048][Tile0, 1:[4x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 2048][Tile0, 1:[4x4], 1]
		Tile0: [0, 2048, 16], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
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
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 73728 [D1, 1 x 73728][D0, 6 x 13824]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 73728][D0, 6 x 13824]
		Tile0: [0, 13824, 108], Tile1: [13824, 13824, 108], Tile2; [27648, 13824, 108]
		T0: [D1: 0][D0: 0], T1: [D1: 0][D0: 1], T2: [D1: 0][D0: 2]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 6400 [D0, 6 x 1200][Tile0, 1:[10x10], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[10x10], 1][D0, 6 x 1200]
		Tile0: [0, 1200, 100], Tile1: [1200, 1200, 100], Tile2; [2400, 1200, 100]
		T0: [D0: 0][Tile0: 0], T1: [D0: 1][Tile0: 0], T2: [D0: 2][Tile0: 0]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 8192 [D1, 1 x 8192][Tile0, 1:[8x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 8192][Tile0, 1:[8x8], 1]
		Tile0: [0, 8192, 8192], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 0], T2: [D1: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+34400);
	KerArg0->W = (unsigned short int) (8);
	KerArg0->H = (unsigned short int) (8);
	KerArg0->OutFeatures = (unsigned short int) (128);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg1->W = (unsigned short int) (10);
	KerArg1->UsedW = (unsigned short int) (10);
	KerArg1->H = (unsigned short int) (10);
	KerArg1->UsedH = (unsigned short int) (10);
	KerArg1->OutFeatures = (unsigned short int) (128);
	KerArg1->Out = (signed char * __restrict__) (L1_Memory+34400);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->Pad = (v4s) 0;
	KerArg1->Orientation = (unsigned char) (1);
	KerArg2->In = (signed char * __restrict__) (L1_Memory+34400);
	KerArg2->W = (unsigned short int) (8);
	KerArg2->UsedW = (unsigned short int) (8);
	KerArg2->H = (unsigned short int) (8);
	KerArg2->UsedH = (unsigned short int) (8);
	KerArg2->OutFeatures = (unsigned short int) (128);
	KerArg2->Pad = (v4s) 0;
	KerArg2->Orientation = (unsigned char) (1);
	KerArg2->Oper = (unsigned char) (1);
	KerArg2->LB = (int) (0);
	KerArg2->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=2048; _LC_Out=16;
	_SP_Out=0;
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+2400+0), 128, 0, &DmaR_Evt1);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+2656+0), 13824, 0, &DmaR_Evt2);
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 1200, 100, 100, 0, &DmaR_Evt3);
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1, D1Ind_NextLast = 1;
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+2400+128*((D1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->Bias = (signed char * __restrict__) (L1_Memory+2400+128*((D1Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParSetBias_fps, (void *) KerArg0);
			__CALL(KerParSetBias_fps, KerArg0);
			for (D0Ind=0; D0Ind<6; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==5), D0Ind_NextLast = ((D0Ind+1)==5);
				/*================================= Prepare Tiles ===================================*/
				_SN_Filter = 0;
				if (!(D0Ind_Last)) {
					_N_Filter = _N_Filter + (13824); _LN_Filter = ((D0Ind_NextLast)?36:108); _SN_Filter = (128*_LN_Filter); 
				} else if (!((1))) {
					_N_Filter = _N_Filter + (-69120); _LN_Filter = (108); _SN_Filter = (128*_LN_Filter); 
				}
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (1200); _LN_In = (100); _SN_In = (((D0Ind_NextLast)?4:12)*_LN_In); 
				} else if (!(1)) {
					_N_In = _N_In + (-6000); _LN_In = (100); _SN_In = (12*_LN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
				if (_SN_Filter) {
					AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+2656+13824*((D0Ind_Total+1)%2)),
							_SN_Filter, 0, &DmaR_Evt2);
				}
				AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+1200*((D0Ind_Total+1)%2)),
							_SN_In, 100, _LN_In, 0, &DmaR_Evt3);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (L1_Memory+0+1200*((D0Ind_Total)%2));
				KerArg1->InFeatures = (unsigned short int) (D0Ind_Last?4:12);
				KerArg1->Filter = (signed char * __restrict__) (L1_Memory+2656+13824*((D0Ind_Total)%2));
				KerArg1->TotalInFeatures = (short int) (D0Ind_Last?4:12);
				AT_FORK(gap_ncore(), (void *) KerParConv3x3Stride1_fps, (void *) KerArg1);
				__CALL(KerParConv3x3Stride1_fps, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				
				
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->Out = (signed char * __restrict__) (L1_Memory+30304+2048*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParPool2x2Stride2_fps, (void *) KerArg2);
			__CALL(KerParPool2x2Stride2_fps, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+30304+2048*((T0Ind_Total)%2)),
					_SC_Out, 16, _LC_Out, 1, &DmaW_Evt1);
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
void  __attribute__ ((noinline)) S4_Linear_10x128x4x4(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 22552 bytes, L2 buffer: 22552 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerLinearLayerReLU_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

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
			Total Size: 10 [D0, 1 x 10]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 1 x 10]
		Tile0: [0, 10, 10], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0], T1: [D0: 0], T2: [D0: 0]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 10 [D0, 1 x 10]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 1 x 10]
		Tile0: [0, 10, 10], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0], T1: [D0: 0], T2: [D0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char * __restrict__) (L1_Memory+0);
	KerArg0->InSize = (unsigned short int) (2048);
	KerArg0->TotalInSize = (unsigned short int) (2048);
	KerArg0->OutSize = (unsigned short int) (10);
	KerArg0->Filter = (signed char * __restrict__) (L1_Memory+2048);
	KerArg0->Bias = (signed char * __restrict__) (L1_Memory+22528);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+22540);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->LB = (int) (-128);
	KerArg0->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0), 2048, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+2048), 20480, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+22528), 10, 0, &DmaR_Evt3);
	AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Bias */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			AT_FORK(gap_ncore(), (void *) KerParLinearLayerReLU_fps, (void *) KerArg0);
			__CALL(KerParLinearLayerReLU_fps, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+22540), 10, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void  __attribute__ ((noinline)) S5_SoftMax(
		signed char * __restrict__ In,
		short int * __restrict__ Out,
		unsigned int Norm)

{
	/* Shared L1: 32 bytes, L2 buffer: 32 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerSoftMax_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Last;
	/* User kernel arguments related variables */
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 1]
	Ker Arg: In, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 10 [Tile0, 1:[1x10], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x10], 1]
		Tile0: [0, 10, 10], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
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
	KerArg0->In = (signed char *__restrict__) (L1_Memory+0);
	KerArg0->Norm = (unsigned short int) (Norm);
	KerArg0->Out = (short int *__restrict__) (L1_Memory+12);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0), 10, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on Tile0 */
		int T0Ind_Last = 1;
		/*====================== Call Kernel LOC_LOOP =========================*/
		KerArg0->N = (unsigned short int) (10);
		AT_FORK(gap_ncore(), (void *) KerParSoftMax_fps, (void *) KerArg0);
		__CALL(KerParSoftMax_fps, KerArg0);
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+12), 20, 1, &DmaW_Evt1);
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
	Mnist_L2_Memory = (AT_L2_POINTER) AT_L2_ALLOC(0, 171274);
	L1_Memory = (AT_L1_POINTER) AT_L1_ALLOC(0, 44448);
	/* Moving Step1Weights, size 800 from HyperFlash at 145408 to (size 800) L2 at 170240 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 145408), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 170240), 800, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step1Biases, size 32 from HyperFlash at 146400 to (size 32) L2 at 171232 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 146400), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 171232), 32, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step2Weights, size 51200 from HyperFlash at 73728 to (size 51200) L2 at 98560 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 73728), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 98560), 51200, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step2Biases, size 64 from HyperFlash at 146336 to (size 64) L2 at 171168 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 146336), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 171168), 64, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step3Weights, size 73728 from HyperFlash at 0 to (size 73728) L2 at 24832 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 24832), 73728, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step3Biases, size 128 from HyperFlash at 146208 to (size 128) L2 at 171040 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 146208), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 171040), 128, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step4Weights, size 20480 from HyperFlash at 124928 to (size 20480) L2 at 149760 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 124928), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 149760), 20480, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step4Biases, size 10 from HyperFlash at 146432 to (size 10) L2 at 171264 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) Mnist_L3_Flash + 146432), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) Mnist_L2_Memory + 171264), 10, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	return 0;
}
int MnistCNN_Destruct()

{
	AT_L2_FREE(0, Mnist_L2_Memory, 171274);
	AT_L1_FREE(0, L1_Memory, 44448);
	AT_HYPERFLASH_FS_CLOSE(&HyperFlash);
	return 0;
}
unsigned int MnistPerf[5];
unsigned int MnistOperCount[5] = {
	479232,
	20512000,
	4728832,
	20480,
	10,
};
char *MnistLName[5] = {
	"S1_Conv2d_32x1x5x5_Relu",
	"S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu",
	"S3_Conv2d_128x64x3x3_MaxPool_2x2_Relu",
	"S4_Linear_10x128x4x4",
	"S5_SoftMax",
};
int MnistCNN(
		signed char *__restrict__ Input_1,
		short int *__restrict__ Output_1)

{
	MnistPerf[0] = gap_cl_readhwtimer();
	S1_Conv2d_32x1x5x5_Relu(
		(signed char *__restrict__) Input_1, /* In */
		(signed char *__restrict__) (Mnist_L2_Memory+170240), /* Filter */
		(signed char *__restrict__) (Mnist_L2_Memory+171232), /* Bias */
		(signed char *__restrict__) (Mnist_L2_Memory+6400), /* Out */
		8, /* Norm */
		8 /* NormBias */
	);
	MnistPerf[0] = gap_cl_readhwtimer() - MnistPerf[0];
	MnistPerf[1] = gap_cl_readhwtimer();
	S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu(
		(signed char *__restrict__) (Mnist_L2_Memory+6400), /* In */
		(signed char *__restrict__) (Mnist_L2_Memory+98560), /* Filter */
		(signed char *__restrict__) (Mnist_L2_Memory+171168), /* Bias */
		(signed char *__restrict__) (Mnist_L2_Memory+0), /* Out */
		8, /* Norm */
		8 /* NormBias */
	);
	MnistPerf[1] = gap_cl_readhwtimer() - MnistPerf[1];
	MnistPerf[2] = gap_cl_readhwtimer();
	S3_Conv2d_128x64x3x3_MaxPool_2x2_Relu(
		(signed char *__restrict__) (Mnist_L2_Memory+0), /* In */
		(signed char *__restrict__) (Mnist_L2_Memory+24832), /* Filter */
		(signed char *__restrict__) (Mnist_L2_Memory+171040), /* Bias */
		(signed char *__restrict__) (Mnist_L2_Memory+6400), /* Out */
		8, /* Norm */
		8 /* NormBias */
	);
	MnistPerf[2] = gap_cl_readhwtimer() - MnistPerf[2];
	MnistPerf[3] = gap_cl_readhwtimer();
	S4_Linear_10x128x4x4(
		(signed char *__restrict__) (Mnist_L2_Memory+6400), /* In */
		(signed char *__restrict__) (Mnist_L2_Memory+149760), /* Filter */
		(signed char *__restrict__) (Mnist_L2_Memory+171264), /* Bias */
		(signed char *__restrict__) (Mnist_L2_Memory+0), /* Out */
		7, /* Norm */
		7 /* NormBias */
	);
	MnistPerf[3] = gap_cl_readhwtimer() - MnistPerf[3];
	MnistPerf[4] = gap_cl_readhwtimer();
	S5_SoftMax(
		(signed char *__restrict__) (Mnist_L2_Memory+0), /* In */
		(short int *__restrict__) Output_1, /* Out */
		1 /* Norm */
	);
	MnistPerf[4] = gap_cl_readhwtimer() - MnistPerf[4];
	return 0;
}
