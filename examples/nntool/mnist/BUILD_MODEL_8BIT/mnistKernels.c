#include "mnistKernels.h"
L1_CL_MEM AT_L1_POINTER L1_Memory;
L2_MEM AT_L2_POINTER mnist_L2_Memory;
static AT_HYPERFLASH_FS_T HyperFlash;
void S1_Conv2d_32x1x5x5_Relu(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 48496 bytes, L2 buffer: 17776 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_fpd_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_DP_fps_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerDP_fps_T S_KerArg2, *KerArg2 = &S_KerArg2;

	/* Iteration space related variables */
	int D1Ind, D1Ind_Total=0, D1Ind_Last, D1Ind_NextLast;
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	int D0Ind, D0Ind_Total=0, D0Ind_Last, D0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_In;
	unsigned int _SN_In;
	unsigned int _N_Filter;
	unsigned int _SN_Filter;
	unsigned int _C_Out;
	unsigned int _SP_Out, _SC_Out;
	unsigned int _LP_Out, _LC_Out;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[D1 Dim: 1][Tile0 Dim: 3][D0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 784 [D0, 1 x 784][Tile0, 3:[28x14, 1:28x14, 28x8], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 3:[28x14, 1:28x14, 28x8], 1][D0, 1 x 784]
		Tile0: [0, 392, 392], Tile1: [280, 392, 392], Tile2; [560, 224, 224]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 0][Tile0: 2]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32 [D1, 1 x 32]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 32]
		Tile0: [0, 32, 32], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0], T1: [D1: 0], T2: [D1: 0]
	Ker Arg: Filter, Tiled Space: D1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 800 [D1, 1 x 800][D0, 1 x 800]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 800][D0, 1 x 800]
		Tile0: [0, 800, 800], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][D0: 0], T1: [D1: 0][D0: 0], T2: [D1: 0][D0: 0]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 3 physical tiles
			Total Size: 18432 [D1, 1 x 18432][Tile0, 3:[24x10, 1:24x10, 24x4], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 18432][Tile0, 3:[24x10, 1:24x10, 24x4], 1]
		Tile0: [0, 7680, 240], Tile1: [240, 7680, 240], Tile2; [480, 3072, 96]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 1], T2: [D1: 0][Tile0: 2]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 3 logical tiles, 1 physical tiles
			Total Size: 73728 [D1, 1 x 73728][Tile0, 3:[24x10, 1:24x10, 24x4], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 73728][Tile0, 3:[24x10, 1:24x10, 24x4], 4]
		Tile0: [0, 73728, 73728], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 0], T2: [D1: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (L1_Memory+17776);
	KerArg0->W = (unsigned short int) (24);
	KerArg0->OutFeatures = (unsigned short int) (32);
	KerArg0->Bias = (signed char * __restrict__) (L1_Memory+784);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg1->W = (unsigned short int) (28);
	KerArg1->UsedW = (unsigned short int) (28);
	KerArg1->InFeatures = (unsigned short int) (1);
	KerArg1->OutFeatures = (unsigned short int) (32);
	KerArg1->Out = (DP_fps_T * __restrict__) (L1_Memory+17776);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->TotalInFeatures = (short int) (1);
	KerArg1->Pad = (v4s) 0;
	KerArg1->Orientation = (unsigned char) (1);
	KerArg2->In = (DP_fps_T * __restrict__) (L1_Memory+17776);
	KerArg2->W = (unsigned short int) (24);
	KerArg2->Norm = (unsigned char) (Norm);
	KerArg2->InFeatures = (unsigned short int) (32);
	KerArg2->LB = (int) (0);
	KerArg2->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 392, 0, &DmaR_Evt1);
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+784), 32, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+816+0), 800, 0, &DmaR_Evt3);
	_C_Out=0; _SC_Out=7680; _LC_Out=240;
	_SP_Out=0;
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D1 */
		int D1Ind_Last = 1, D1Ind_NextLast = 1;
		/*================================= Prepare Tiles ===================================*/
		_SN_Filter = 0;
		
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read Filter */
		if (_SN_Filter) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+816+800*((D1Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt3);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<3; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==2), T0Ind_NextLast = ((T0Ind+1)==2);
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->H = (unsigned short int) (T0Ind_Last?4:10);
			AT_FORK(gap_ncore(), (void *) KerParSetBias_DP_fps, (void *) KerArg0);
			__CALL(KerParSetBias_DP_fps, KerArg0);
			{ /* Single iteration on D0 */
				int D0Ind_Last = 1, D0Ind_NextLast = 1;
				/*================================= Prepare Tiles ===================================*/
				_SN_In = 0;
				if (!(T0Ind_Last)) {
					_N_In = _N_In + (280); _SN_In = ((T0Ind_NextLast)?224:392); 
				} else if (!(1)) {
					_N_In = _N_In + (-560); _SN_In = (392); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+392*((D0Ind_Total+1)%2)),
							_SN_In, 0, &DmaR_Evt1);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (signed char * __restrict__) (L1_Memory+0+392*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (T0Ind_Last?8:14);
				KerArg1->UsedH = (unsigned short int) (T0Ind_Last?8:14);
				KerArg1->Filter = (signed char * __restrict__) (L1_Memory+816+800*((D1Ind_Total)%2));
				AT_FORK(gap_ncore(), (void *) KerParConv5x5Stride1_DP_fps, (void *) KerArg1);
				__CALL(KerParConv5x5Stride1_DP_fps, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				
				/*============================= End Update Arg Pipeline =============================*/
				D0Ind_Total++;
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->H = (unsigned short int) (T0Ind_Last?4:10);
			KerArg2->Out = (signed char * __restrict__) (L1_Memory+2416+7680*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDP_fps, (void *) KerArg2);
			__CALL(KerDP_fps, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+2416+7680*((T0Ind_Total)%2)),
					_SC_Out, 576, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (240); _LC_Out = ((T0Ind_NextLast)?96:240); _SC_Out = (32*_LC_Out); 
			}
			/*============================= End Prepare Tiles ===================================*/
		} /* End iteration on Tile0 */
		/*================================= Update Arg Pipeline =============================*/
		
		/*============================= End Update Arg Pipeline =============================*/
		D1Ind_Total++;
	} /* End iteration on D1 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu(
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
void S3_MaxPool_2x2(
		signed char * __restrict__ In,
		signed char * __restrict__ Out)

{
	/* Shared L1: 8000 bytes, L2 buffer: 8000 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerReLUPool_fps_T S_KerArg0, *KerArg0 = &S_KerArg0;

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
			Total Size: 6400 [D0, 1 x 6400][Tile0, 1:[10x10], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 1 x 6400][Tile0, 1:[10x10], 1]
		Tile0: [0, 6400, 6400], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1600 [D0, 1 x 1600][Tile0, 1:[5x5], 1]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 1 x 1600][Tile0, 1:[5x5], 1]
		Tile0: [0, 1600, 1600], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (signed char * __restrict__) (L1_Memory+0);
	KerArg0->W = (unsigned short int) (10);
	KerArg0->UsedW = (unsigned short int) (10);
	KerArg0->OutFeatures = (unsigned short int) (64);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+6400);
	KerArg0->Pad = (v4s) 0;
	KerArg0->Orientation = (unsigned char) (1);
	KerArg0->Oper = (unsigned char) (0);
	KerArg0->LB = (int) (-128);
	KerArg0->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0), 6400, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->H = (unsigned short int) (10);
			KerArg0->UsedH = (unsigned short int) (10);
			AT_FORK(gap_ncore(), (void *) KerParPool2x2Stride2_fps, (void *) KerArg0);
			__CALL(KerParPool2x2Stride2_fps, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+6400), 1600, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S4_Linear_10x64x5x5(
		signed char * __restrict__ In,
		signed char * __restrict__ Filter,
		signed char * __restrict__ Bias,
		signed char * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 17624 bytes, L2 buffer: 17624 bytes */
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
			Total Size: 1600 [Tile0, 1:[1x1], 1600]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 1600]
		Tile0: [0, 1600, 1600], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 16000 [D0, 1 x 16000]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 1 x 16000]
		Tile0: [0, 16000, 16000], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
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
	KerArg0->InSize = (unsigned short int) (1600);
	KerArg0->TotalInSize = (unsigned short int) (1600);
	KerArg0->OutSize = (unsigned short int) (10);
	KerArg0->Filter = (signed char * __restrict__) (L1_Memory+1600);
	KerArg0->Bias = (signed char * __restrict__) (L1_Memory+17600);
	KerArg0->Out = (signed char * __restrict__) (L1_Memory+17612);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->LB = (int) (-128);
	KerArg0->UB = (int) (127);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0), 1600, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+1600), 16000, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+17600), 10, 0, &DmaR_Evt3);
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
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+17612), 10, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S5_SoftMax(
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
int mnistCNN_Construct()

{
	AT_HYPERFLASH_FS_FC_EVENT Uchan1;
	AT_HYPERFLASH_FS_CONF_T HyperFlashConf;
	int Error;
	AT_HYPERFLASH_FS_CONF_INIT(&HyperFlashConf, AT_MEM_L3_HFLASH, 0);
	AT_HYPERFLASH_FS_OPEN(&HyperFlash, &HyperFlashConf, "mnist_L3_Flash_Const.dat", &Error);
	if (Error) return 1;
	mnist_L2_Memory = (AT_L2_POINTER) AT_L2_ALLOC(0, 92938);
	L1_Memory = (AT_L1_POINTER) AT_L1_ALLOC(0, 48496);
	/* Moving Step1Weights, size 800 from HyperFlash at 67200 to (size 800) L2 at 92032 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 67200), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 92032), 800, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step1Biases, size 32 from HyperFlash at 68064 to (size 32) L2 at 92896 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 68064), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 92896), 32, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step2Weights, size 51200 from HyperFlash at 0 to (size 51200) L2 at 24832 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 24832), 51200, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step2Biases, size 64 from HyperFlash at 68000 to (size 64) L2 at 92832 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 68000), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 92832), 64, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step4Weights, size 16000 from HyperFlash at 51200 to (size 16000) L2 at 76032 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 51200), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 76032), 16000, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step4Biases, size 10 from HyperFlash at 68096 to (size 10) L2 at 92928 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 68096), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 92928), 10, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	return 0;
}
int mnistCNN_Destruct()

{
	AT_L2_FREE(0, mnist_L2_Memory, 92938);
	AT_L1_FREE(0, L1_Memory, 48496);
	AT_HYPERFLASH_FS_CLOSE(&HyperFlash);
	return 0;
}
unsigned int AT_GraphPerf[5];
unsigned int AT_GraphOperInfosNames[5] = {
	479232,
	20512000,
	6400,
	16000,
	10,
};
char *AT_GraphNodeNames[5] = {
	"S1_Conv2d_32x1x5x5_Relu",
	"S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu",
	"S3_MaxPool_2x2",
	"S4_Linear_10x64x5x5",
	"S5_SoftMax",
};
int mnistCNN(
		signed char *__restrict__ Input_1,
		short int *__restrict__ Output_1)

{
	AT_GraphPerf[0] = gap_cl_readhwtimer();
	S1_Conv2d_32x1x5x5_Relu(
		((signed char *__restrict__) Input_1), /* In */
		((signed char *__restrict__) (mnist_L2_Memory+92032)), /* Filter */
		((signed char *__restrict__) (mnist_L2_Memory+92896)), /* Bias */
		((signed char *__restrict__) (mnist_L2_Memory+6400)), /* Out */
		8, /* Norm */
		9 /* NormBias */
	);
	AT_GraphPerf[0] = gap_cl_readhwtimer() - AT_GraphPerf[0];
	AT_GraphPerf[1] = gap_cl_readhwtimer();
	S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu(
		((signed char *__restrict__) (mnist_L2_Memory+6400)), /* In */
		((signed char *__restrict__) (mnist_L2_Memory+24832)), /* Filter */
		((signed char *__restrict__) (mnist_L2_Memory+92832)), /* Bias */
		((signed char *__restrict__) (mnist_L2_Memory+0)), /* Out */
		9, /* Norm */
		12 /* NormBias */
	);
	AT_GraphPerf[1] = gap_cl_readhwtimer() - AT_GraphPerf[1];
	AT_GraphPerf[2] = gap_cl_readhwtimer();
	S3_MaxPool_2x2(
		((signed char *__restrict__) (mnist_L2_Memory+0)), /* In */
		((signed char *__restrict__) (mnist_L2_Memory+6400)) /* Out */
	);
	AT_GraphPerf[2] = gap_cl_readhwtimer() - AT_GraphPerf[2];
	AT_GraphPerf[3] = gap_cl_readhwtimer();
	S4_Linear_10x64x5x5(
		((signed char *__restrict__) (mnist_L2_Memory+6400)), /* In */
		((signed char *__restrict__) (mnist_L2_Memory+76032)), /* Filter */
		((signed char *__restrict__) (mnist_L2_Memory+92928)), /* Bias */
		((signed char *__restrict__) (mnist_L2_Memory+0)), /* Out */
		9, /* Norm */
		4 /* NormBias */
	);
	AT_GraphPerf[3] = gap_cl_readhwtimer() - AT_GraphPerf[3];
	AT_GraphPerf[4] = gap_cl_readhwtimer();
	S5_SoftMax(
		((signed char *__restrict__) (mnist_L2_Memory+0)), /* In */
		((short int *__restrict__) Output_1), /* Out */
		2 /* Norm */
	);
	AT_GraphPerf[4] = gap_cl_readhwtimer() - AT_GraphPerf[4];
	return 0;
}
