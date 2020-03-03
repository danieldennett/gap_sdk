#include "mnistKernels.h"
L1_CL_MEM AT_L1_POINTER L1_Memory;
L2_MEM AT_L2_POINTER mnist_L2_Memory;
static AT_HYPERFLASH_FS_T HyperFlash;
void S1_Conv2d_32x1x5x5_Relu(
		short int * __restrict__ In,
		short int * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 47504 bytes, L2 buffer: 26000 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_fpd_fp_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_DP_fp_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerDP_fp_T S_KerArg2, *KerArg2 = &S_KerArg2;

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
		[D1 Dim: 1][Tile0 Dim: 4][D0 Dim: 1]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 1568 [D0, 1 x 1568][Tile0, 4:[28x11, 2:28x11, 28x7], 2]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 4:[28x11, 2:28x11, 28x7], 2][D0, 1 x 1568]
		Tile0: [0, 616, 616], Tile1: [392, 616, 616], Tile2; [784, 616, 616]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 1], T2: [D0: 0][Tile0: 2]
	Ker Arg: Bias, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 64 [D1, 1 x 64]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 64]
		Tile0: [0, 64, 64], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0], T1: [D1: 0], T2: [D1: 0]
	Ker Arg: Filter, Tiled Space: D1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 1600 [D1, 1 x 1600][D0, 1 x 1600]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 1600][D0, 1 x 1600]
		Tile0: [0, 1600, 1600], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][D0: 0], T1: [D1: 0][D0: 0], T2: [D1: 0][D0: 0]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 36864 [D1, 1 x 36864][Tile0, 4:[24x7, 2:24x7, 24x3], 2]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 36864][Tile0, 4:[24x7, 2:24x7, 24x3], 2]
		Tile0: [0, 10752, 336], Tile1: [336, 10752, 336], Tile2; [672, 10752, 336]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 1], T2: [D1: 0][Tile0: 2]
	Ker Arg: ConvOut, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 1 physical tiles
			Total Size: 73728 [D1, 1 x 73728][Tile0, 4:[24x7, 2:24x7, 24x3], 4]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 1 x 73728][Tile0, 4:[24x7, 2:24x7, 24x3], 4]
		Tile0: [0, 73728, 73728], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D1: 0][Tile0: 0], T1: [D1: 0][Tile0: 0], T2: [D1: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->Out = (int * __restrict__) (L1_Memory+26000);
	KerArg0->W = (unsigned short int) (24);
	KerArg0->OutFeatures = (unsigned short int) (32);
	KerArg0->Bias = (short int * __restrict__) (L1_Memory+1232);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg1->W = (unsigned short int) (28);
	KerArg1->UsedW = (unsigned short int) (28);
	KerArg1->InFeatures = (unsigned short int) (1);
	KerArg1->OutFeatures = (unsigned short int) (32);
	KerArg1->Out = (int * __restrict__) (L1_Memory+26000);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->TotalInFeatures = (short int) (1);
	KerArg1->Pad = (v4s) 0;
	KerArg1->Orientation = (unsigned char) (1);
	KerArg2->In = (int * __restrict__) (L1_Memory+26000);
	KerArg2->W = (unsigned short int) (24);
	KerArg2->Norm = (unsigned char) (Norm);
	KerArg2->InFeatures = (unsigned short int) (32);
	KerArg2->LB = (int) (0);
	KerArg2->UB = (int) (32767);
	/*================================= Read Tiles Prolog ===============================*/
	_N_In=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 616, 0, &DmaR_Evt1);
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+1232), 64, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Bias */
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+1296+0), 1600, 0, &DmaR_Evt3);
	_C_Out=0; _SC_Out=10752; _LC_Out=336;
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
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+1296+1600*((D1Ind_Total+1)%2)),
					_SN_Filter, 0, &DmaR_Evt3);
		}
		/*============================= End Read Tiles ======================================*/
		for (T0Ind=0; T0Ind<4; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
			int T0Ind_Last = (T0Ind==3), T0Ind_NextLast = ((T0Ind+1)==3);
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->H = (unsigned short int) (T0Ind_Last?3:7);
			AT_FORK(gap_ncore(), (void *) KerParSetBias_DP_fp, (void *) KerArg0);
			__CALL(KerParSetBias_DP_fp, KerArg0);
			{ /* Single iteration on D0 */
				int D0Ind_Last = 1, D0Ind_NextLast = 1;
				/*================================= Prepare Tiles ===================================*/
				_SN_In = 0;
				if (!(T0Ind_Last)) {
					_N_In = _N_In + (392); _SN_In = ((T0Ind_NextLast)?392:616); 
				} else if (!(1)) {
					_N_In = _N_In + (-1176); _SN_In = (616); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+616*((D0Ind_Total+1)%2)),
							_SN_In, 0, &DmaR_Evt1);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (short int * __restrict__) (L1_Memory+0+616*((D0Ind_Total)%2));
				KerArg1->H = (unsigned short int) (T0Ind_Last?7:11);
				KerArg1->UsedH = (unsigned short int) (T0Ind_Last?7:11);
				KerArg1->Filter = (short int * __restrict__) (L1_Memory+1296+1600*((D1Ind_Total)%2));
				AT_FORK(gap_ncore(), (void *) KerParConv5x5Stride1_DP_fp, (void *) KerArg1);
				__CALL(KerParConv5x5Stride1_DP_fp, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				
				/*============================= End Update Arg Pipeline =============================*/
				D0Ind_Total++;
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			KerArg2->H = (unsigned short int) (T0Ind_Last?3:7);
			KerArg2->Out = (short int * __restrict__) (L1_Memory+4496+10752*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerDP_fp, (void *) KerArg2);
			__CALL(KerDP_fp, KerArg2);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+4496+10752*((T0Ind_Total)%2)),
					_SC_Out, 1152, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(T0Ind_Last)) {
				_C_Out = _C_Out + (336); _LC_Out = ((T0Ind_NextLast)?144:336); _SC_Out = (32*_LC_Out); 
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
		short int * __restrict__ In,
		short int * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 47680 bytes, L2 buffer: 22080 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaR_Evt2;
	AT_L2_EVENT DmaR_Evt3;
	AT_L2_EVENT DmaW_Evt1;
	KerSetBias_fpd_fp_T S_KerArg0, *KerArg0 = &S_KerArg0;
	KerConv_DP_fp_T S_KerArg1, *KerArg1 = &S_KerArg1;
	KerDP_fp_T S_KerArg2, *KerArg2 = &S_KerArg2;
	KerReLUPool_fp_T S_KerArg3, *KerArg3 = &S_KerArg3;

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
		[D1 Dim: 4][Tile0 Dim: 1][D0 Dim: 8]
	Ker Arg: Out, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 12800 [D1, 4 x 3200][Tile0, 1:[10x10], 2]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 4 x 3200][Tile0, 1:[10x10], 2]
		Tile0: [0, 3200, 200], Tile1: [3200, 3200, 200], Tile2; [6400, 3200, 200]
		T0: [D1: 0][Tile0: 0], T1: [D1: 1][Tile0: 0], T2: [D1: 2][Tile0: 0]
	Ker Arg: Bias, Tiled Space: D1
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 4 logical tiles, 4 physical tiles
			Total Size: 128 [D1, 4 x 32]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 4 x 32]
		Tile0: [0, 32, 32], Tile1: [32, 32, 32], Tile2; [64, 32, 32]
		T0: [D1: 0], T1: [D1: 1], T2: [D1: 2]
	Ker Arg: Filter, Tiled Space: D0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 32 logical tiles, 32 physical tiles
			Total Size: 102400 [D1, 4 x 25600][D0, 8 x 3200]
		KerArgItSpace (User Kernel Iter Order):
			[D1, 4 x 25600][D0, 8 x 3200]
		Tile0: [0, 3200, 200], Tile1: [3200, 3200, 200], Tile2; [6400, 3200, 200]
		T0: [D1: 0][D0: 0], T1: [D1: 0][D0: 1], T2: [D1: 0][D0: 2]
	Ker Arg: In, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 8 logical tiles, 8 physical tiles
			Total Size: 36864 [D0, 8 x 4608][Tile0, 1:[24x24], 2]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[24x24], 2][D0, 8 x 4608]
		Tile0: [0, 4608, 1152], Tile1: [4608, 4608, 1152], Tile2; [9216, 4608, 1152]
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
	KerArg0->Out = (int * __restrict__) (L1_Memory+22080);
	KerArg0->W = (unsigned short int) (20);
	KerArg0->H = (unsigned short int) (20);
	KerArg0->OutFeatures = (unsigned short int) (16);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg1->W = (unsigned short int) (24);
	KerArg1->UsedW = (unsigned short int) (24);
	KerArg1->H = (unsigned short int) (24);
	KerArg1->UsedH = (unsigned short int) (24);
	KerArg1->InFeatures = (unsigned short int) (4);
	KerArg1->OutFeatures = (unsigned short int) (16);
	KerArg1->Out = (int * __restrict__) (L1_Memory+22080);
	KerArg1->Norm = (unsigned char) (Norm);
	KerArg1->TotalInFeatures = (short int) (4);
	KerArg1->Pad = (v4s) 0;
	KerArg1->Orientation = (unsigned char) (1);
	KerArg2->In = (int * __restrict__) (L1_Memory+22080);
	KerArg2->W = (unsigned short int) (20);
	KerArg2->H = (unsigned short int) (20);
	KerArg2->Out = (short int * __restrict__) (L1_Memory+22080);
	KerArg2->Norm = (unsigned char) (Norm);
	KerArg2->InFeatures = (unsigned short int) (16);
	KerArg2->LB = (int) (0);
	KerArg2->UB = (int) (32767);
	KerArg3->In = (short int * __restrict__) (L1_Memory+22080);
	KerArg3->W = (unsigned short int) (20);
	KerArg3->UsedW = (unsigned short int) (20);
	KerArg3->H = (unsigned short int) (20);
	KerArg3->UsedH = (unsigned short int) (20);
	KerArg3->OutFeatures = (unsigned short int) (16);
	KerArg3->Pad = (v4s) 0;
	KerArg3->Orientation = (unsigned char) (1);
	KerArg3->Oper = (unsigned char) (1);
	KerArg3->LB = (int) (0);
	KerArg3->UB = (int) (32767);
	/*================================= Read Tiles Prolog ===============================*/
	_C_Out=0; _SC_Out=3200; _LC_Out=200;
	_SP_Out=0;
	_N_Bias=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9216+0), 32, 0, &DmaR_Evt1);
	_N_Filter=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9280+0), 3200, 0, &DmaR_Evt2);
	_N_In=0;
	AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+0), 4608, 1152, 1152, 0, &DmaR_Evt3);
	/*============================= End Read Tiles Prolog ===============================*/
	for (D1Ind=0; D1Ind<4; D1Ind++, D1Ind_Total++) { /* Iteration on D1 */
		int D1Ind_Last = (D1Ind==3), D1Ind_NextLast = ((D1Ind+1)==3);
		/*================================= Prepare Tiles ===================================*/
		_SN_Bias = 0;
		if (!(D1Ind_Last)) {
			_N_Bias = _N_Bias + (32); _SN_Bias = (32); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read Bias */
		if (_SN_Bias) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+_N_Bias), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9216+32*((D1Ind_Total+1)%2)),
					_SN_Bias, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1, T0Ind_NextLast = 1;
			/*====================== Call Kernel LOC_D0_PROLOG =========================*/
			KerArg0->Bias = (short int * __restrict__) (L1_Memory+9216+32*((D1Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParSetBias_DP_fp, (void *) KerArg0);
			__CALL(KerParSetBias_DP_fp, KerArg0);
			for (D0Ind=0; D0Ind<8; D0Ind++, D0Ind_Total++) { /* Iteration on D0 */
				int D0Ind_Last = (D0Ind==7), D0Ind_NextLast = ((D0Ind+1)==7);
				/*================================= Prepare Tiles ===================================*/
				_SN_Filter = 0;
				if (!(D0Ind_Last)) {
					_N_Filter = _N_Filter + (3200); _LN_Filter = (200); _SN_Filter = (16*_LN_Filter); 
				} else if (!((1))) {
					_N_Filter = _N_Filter + (-22400); _LN_Filter = (200); _SN_Filter = (16*_LN_Filter); 
				} else if (!(D1Ind_Last)) {
					_N_Filter = _N_Filter + (25600)+(-22400); _LN_Filter = (200); _SN_Filter = (16*_LN_Filter); 
				}
				_SN_In = 0;
				if (!(D0Ind_Last)) {
					_N_In = _N_In + (4608); _LN_In = (1152); _SN_In = (4*_LN_In); 
				} else if (!(D1Ind_Last)) {
					_N_In = _N_In + (-32256); _LN_In = (1152); _SN_In = (4*_LN_In); 
				}
				/*============================= End Prepare Tiles ===================================*/
				/*================================= Read Tiles ======================================*/
				AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
				if (_SN_Filter) {
					AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+_N_Filter), ((AT_L2_INT_ADDR_TYPE) L1_Memory+9280+3200*((D0Ind_Total+1)%2)),
							_SN_Filter, 0, &DmaR_Evt2);
				}
				AT_L2_WAIT(0, &DmaR_Evt3); /* Wait previous DMA read In */
				if (_SN_In) {
					AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) In+_N_In), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0+4608*((D0Ind_Total+1)%2)),
							_SN_In, 1152, _LN_In, 0, &DmaR_Evt3);
				}
				/*============================= End Read Tiles ======================================*/
				/*====================== Call Kernel LOC_D0 =========================*/
				KerArg1->In = (short int * __restrict__) (L1_Memory+0+4608*((D0Ind_Total)%2));
				KerArg1->Filter = (short int * __restrict__) (L1_Memory+9280+3200*((D0Ind_Total)%2));
				AT_FORK(gap_ncore(), (void *) KerParConv5x5Stride1_DP_fp, (void *) KerArg1);
				__CALL(KerParConv5x5Stride1_DP_fp, KerArg1);
				/*================================= Update Arg Pipeline =============================*/
				
				
				/*============================= End Update Arg Pipeline =============================*/
			} /* End iteration on D0 */
			/*====================== Call Kernel LOC_D0_EPILOG =========================*/
			AT_FORK(gap_ncore(), (void *) KerDP_IO_fp, (void *) KerArg2);
			__CALL(KerDP_IO_fp, KerArg2);
			KerArg3->Out = (short int * __restrict__) (L1_Memory+15680+3200*((T0Ind_Total)%2));
			AT_FORK(gap_ncore(), (void *) KerParPool2x2Stride2_fp, (void *) KerArg3);
			__CALL(KerParPool2x2Stride2_fp, KerArg3);
			/*================================= Write Tiles =====================================*/
			if (_SP_Out) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write Out */
			AT_L2_COPY2D(0, ((AT_L2_EXT_ADDR_TYPE) Out+_C_Out), ((AT_L2_INT_ADDR_TYPE) L1_Memory+15680+3200*((T0Ind_Total)%2)),
					_SC_Out, 200, _LC_Out, 1, &DmaW_Evt1);
			/*============================= End Write Tiles =====================================*/
			/*================================= Update Arg Pipeline =============================*/
			_SP_Out = _SC_Out;_LP_Out = _LC_Out;
			/*============================= End Update Arg Pipeline =============================*/
			/*================================= Prepare Tiles ===================================*/
			_SC_Out = 0;
			if (!(D1Ind_Last)) {
				_C_Out = _C_Out + (3200); _LC_Out = (200); _SC_Out = (16*_LC_Out); 
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
		short int * __restrict__ In,
		short int * __restrict__ Out)

{
	/* Shared L1: 16000 bytes, L2 buffer: 16000 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerReLUPool_fp_T S_KerArg0, *KerArg0 = &S_KerArg0;

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
			Total Size: 12800 [D0, 1 x 12800][Tile0, 1:[10x10], 2]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 1 x 12800][Tile0, 1:[10x10], 2]
		Tile0: [0, 12800, 12800], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	Ker Arg: Out, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 3200 [D0, 1 x 3200][Tile0, 1:[5x5], 2]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 1 x 3200][Tile0, 1:[5x5], 2]
		Tile0: [0, 3200, 3200], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [D0: 0][Tile0: 0], T1: [D0: 0][Tile0: 0], T2: [D0: 0][Tile0: 0]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->In = (short int * __restrict__) (L1_Memory+0);
	KerArg0->W = (unsigned short int) (10);
	KerArg0->UsedW = (unsigned short int) (10);
	KerArg0->OutFeatures = (unsigned short int) (64);
	KerArg0->Out = (short int * __restrict__) (L1_Memory+12800);
	KerArg0->Pad = (v4s) 0;
	KerArg0->Orientation = (unsigned char) (1);
	KerArg0->Oper = (unsigned char) (0);
	KerArg0->LB = (int) (-32768);
	KerArg0->UB = (int) (32767);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0), 12800, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	/*============================= End Read Tiles Prolog ===============================*/
	{ /* Single iteration on D0 */
		int D0Ind_Last = 1;
		{ /* Single iteration on Tile0 */
			int T0Ind_Last = 1;
			/*====================== Call Kernel LOC_LOOP =========================*/
			KerArg0->H = (unsigned short int) (10);
			KerArg0->UsedH = (unsigned short int) (10);
			AT_FORK(gap_ncore(), (void *) KerParPool2x2Stride2_fp, (void *) KerArg0);
			__CALL(KerParPool2x2Stride2_fp, KerArg0);
		} /* End iteration on Tile0 */
	} /* End iteration on D0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+12800), 3200, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S4_Linear_10x64x5x5(
		short int * __restrict__ In,
		short int * __restrict__ Filter,
		short int * __restrict__ Bias,
		short int * __restrict__ Out,
		unsigned int Norm,
		unsigned int NormBias)

{
	/* Shared L1: 35240 bytes, L2 buffer: 35240 bytes */
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
			Total Size: 3200 [Tile0, 1:[1x1], 3200]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 1:[1x1], 3200]
		Tile0: [0, 3200, 3200], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: Filter, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 1 logical tiles, 1 physical tiles
			Total Size: 32000 [D0, 1 x 32000]
		KerArgItSpace (User Kernel Iter Order):
			[D0, 1 x 32000]
		Tile0: [0, 32000, 32000], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
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
	KerArg0->InSize = (unsigned short int) (1600);
	KerArg0->TotalInSize = (unsigned short int) (1600);
	KerArg0->OutSize = (unsigned short int) (10);
	KerArg0->Filter = (short int * __restrict__) (L1_Memory+3200);
	KerArg0->Bias = (short int * __restrict__) (L1_Memory+35200);
	KerArg0->Out = (short int * __restrict__) (L1_Memory+35220);
	KerArg0->Norm = (unsigned char) (Norm);
	KerArg0->NormBias = (unsigned char) (NormBias);
	KerArg0->LB = (int) (-32768);
	KerArg0->UB = (int) (32767);
	/*================================= Read Tiles Prolog ===============================*/
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) In+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+0), 3200, 0, &DmaR_Evt1);
	AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read In */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Filter+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+3200), 32000, 0, &DmaR_Evt2);
	AT_L2_WAIT(0, &DmaR_Evt2); /* Wait previous DMA read Filter */
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Bias+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+35200), 20, 0, &DmaR_Evt3);
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
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) Out+0), ((AT_L2_INT_ADDR_TYPE) L1_Memory+35220), 20, 1, &DmaW_Evt1);
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait DMA write Out */
	/*============================ End Write Tiles Epilog ===============================*/
}
void S5_SoftMax(
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
int mnistCNN_Construct()

{
	AT_HYPERFLASH_FS_FC_EVENT Uchan1;
	AT_HYPERFLASH_FS_CONF_T HyperFlashConf;
	int Error;
	AT_HYPERFLASH_FS_CONF_INIT(&HyperFlashConf, AT_MEM_L3_HFLASH, 0);
	AT_HYPERFLASH_FS_OPEN(&HyperFlash, &HyperFlashConf, "mnist_L3_Flash_Const.dat", &Error);
	if (Error) return 1;
	mnist_L2_Memory = (AT_L2_POINTER) AT_L2_ALLOC(0, 185876);
	L1_Memory = (AT_L1_POINTER) AT_L1_ALLOC(0, 47680);
	/* Moving Step1Weights, size 1600 from HyperFlash at 134400 to (size 1600) L2 at 184064 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 134400), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 184064), 1600, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step1Biases, size 64 from HyperFlash at 136128 to (size 64) L2 at 185792 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 136128), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 185792), 64, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step2Weights, size 102400 from HyperFlash at 0 to (size 102400) L2 at 49664 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 0), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 49664), 102400, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step2Biases, size 128 from HyperFlash at 136000 to (size 128) L2 at 185664 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 136000), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 185664), 128, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step4Weights, size 32000 from HyperFlash at 102400 to (size 32000) L2 at 152064 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 102400), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 152064), 32000, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	/* Moving Step4Biases, size 20 from HyperFlash at 136192 to (size 20) L2 at 185856 */
	AT_HYPERFLASH_FS_FC_COPY(&HyperFlash, ((AT_HYPERFLASH_FS_EXT_ADDR_TYPE) mnist_L3_Flash + 136192), ((AT_HYPERFLASH_FS_INT_ADDR_TYPE) mnist_L2_Memory + 185856), 20, 0, &Uchan1);
	AT_HYPERFLASH_FS_FC_WAIT(&HyperFlash, &Uchan1);
	return 0;
}
int mnistCNN_Destruct()

{
	AT_L2_FREE(0, mnist_L2_Memory, 185876);
	AT_L1_FREE(0, L1_Memory, 47680);
	AT_HYPERFLASH_FS_CLOSE(&HyperFlash);
	return 0;
}
int mnistCNN(
		short int *__restrict__ Input_1,
		short int *__restrict__ Output_1)

{
	S1_Conv2d_32x1x5x5_Relu(
		((short int *__restrict__) Input_1), /* In */
		((short int *__restrict__) (mnist_L2_Memory+184064)), /* Filter */
		((short int *__restrict__) (mnist_L2_Memory+185792)), /* Bias */
		((short int *__restrict__) (mnist_L2_Memory+12800)), /* Out */
		16, /* Norm */
		17 /* NormBias */
	);
	S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu(
		((short int *__restrict__) (mnist_L2_Memory+12800)), /* In */
		((short int *__restrict__) (mnist_L2_Memory+49664)), /* Filter */
		((short int *__restrict__) (mnist_L2_Memory+185664)), /* Bias */
		((short int *__restrict__) (mnist_L2_Memory+0)), /* Out */
		16, /* Norm */
		19 /* NormBias */
	);
	S3_MaxPool_2x2(
		((short int *__restrict__) (mnist_L2_Memory+0)), /* In */
		((short int *__restrict__) (mnist_L2_Memory+12800)) /* Out */
	);
	S4_Linear_10x64x5x5(
		((short int *__restrict__) (mnist_L2_Memory+12800)), /* In */
		((short int *__restrict__) (mnist_L2_Memory+152064)), /* Filter */
		((short int *__restrict__) (mnist_L2_Memory+185856)), /* Bias */
		((short int *__restrict__) (mnist_L2_Memory+0)), /* Out */
		16, /* Norm */
		11 /* NormBias */
	);
	S5_SoftMax(
		((short int *__restrict__) (mnist_L2_Memory+0)), /* In */
		((short int *__restrict__) Output_1), /* Out */
		10 /* Norm */
	);
	return 0;
}
