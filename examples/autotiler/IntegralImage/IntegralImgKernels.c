#include "IntegralImgKernels.h"
L1_CL_MEM AT_L1_POINTER II_L1_Memory;
L2_MEM AT_L2_POINTER II_L2_Memory;
void BodyIntegralImage(
		unsigned char *  __restrict__ ImageIn,
		unsigned int *  __restrict__ IntegralImage)

{
	/* Shared L1: 39284 bytes, L2 buffer: 0 bytes */
	/* Local variables used by this kernel */
	AT_L2_EVENT DmaR_Evt1;
	AT_L2_EVENT DmaW_Evt1;
	KerPrimeImage_ArgT S_KerArg0, *KerArg0 = &S_KerArg0;
	KerProcessImage_ArgT S_KerArg1, *KerArg1 = &S_KerArg1;

	/* Iteration space related variables */
	int T0Ind, T0Ind_Total=0, T0Ind_Last, T0Ind_NextLast;
	/* User kernel arguments related variables */
	unsigned int _N_ImageIn;
	unsigned int _SN_ImageIn;
	unsigned int _C_IntegralImage;
	unsigned int _SP_IntegralImage, _SC_IntegralImage;
	/*============================= Ker Arg Iter Spaces =========================================
	User Kernel Iteration Space:
		[Tile0 Dim: 6]
	Ker Arg: KerIn, Tiled Space: Tile0
		Min Pipe Depth: 0, Max Pipe Depth: 1
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 19481 [Tile0, 6:[161x24, 4:161x24, 161x1], 1]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 6:[161x24, 4:161x24, 161x1], 1]
		Tile0: [0, 3864, 3864], Tile1: [3864, 3864, 3864], Tile2; [7728, 3864, 3864]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	Ker Arg: KerBuffer, Tiled Space: Buffer
		Min Pipe Depth: 0, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 1 physical tiles
			Total Size: 644 [Tile0, 6:[161x1, 4:161x1, 161x1], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 6:[161x1, 4:161x1, 161x1], 4]
		Tile0: [0, 644, 644], Tile1: [0, 0, 0], Tile2; [0, 0, 0]
		T0: [Tile0: 0], T1: [Tile0: 0], T2: [Tile0: 0]
	Ker Arg: KerOut, Tiled Space: Tile0
		Min Pipe Depth: -1, Max Pipe Depth: 0
		KerArgItSpace: 6 logical tiles, 6 physical tiles
			Total Size: 77924 [Tile0, 6:[161x24, 4:161x24, 161x1], 4]
		KerArgItSpace (User Kernel Iter Order):
			[Tile0, 6:[161x24, 4:161x24, 161x1], 4]
		Tile0: [0, 15456, 15456], Tile1: [15456, 15456, 15456], Tile2; [30912, 15456, 15456]
		T0: [Tile0: 0], T1: [Tile0: 1], T2: [Tile0: 2]
	======================== End Ker Arg Iter Spaces =========================================*/
	/*=========================== Call Kernel, Invariant assignment =====================*/
	KerArg0->W = (unsigned int) (161);
	KerArg1->W = (unsigned int) (161);
	/*================================= Read Tiles Prolog ===============================*/
	_N_ImageIn=0;
	AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ImageIn+0), ((AT_L2_INT_ADDR_TYPE) II_L1_Memory+0+0), 3864, 0, &DmaR_Evt1);
	_C_IntegralImage=0; _SC_IntegralImage=15456;
	_SP_IntegralImage=0;
	/*============================= End Read Tiles Prolog ===============================*/
	/*====================== Call Kernel LOC_LOOP_PROLOG =========================*/
	KerArg0->KerBuffer = (unsigned int * __restrict__) (II_L1_Memory+7728);
	AT_FORK(gap_ncore(), (void *) KerPrime, (void *) KerArg0);
	__CALL(KerPrime, KerArg0);
	for (T0Ind=0; T0Ind<6; T0Ind++, T0Ind_Total++) { /* Iteration on Tile0 */
		int T0Ind_Last = (T0Ind==5), T0Ind_NextLast = ((T0Ind+1)==5);
		/*================================= Prepare Tiles ===================================*/
		_SN_ImageIn = 0;
		if (!(T0Ind_Last)) {
			_N_ImageIn = _N_ImageIn + (3864); _SN_ImageIn = ((T0Ind_NextLast)?161:3864); 
		}
		/*============================= End Prepare Tiles ===================================*/
		/*================================= Read Tiles ======================================*/
		AT_L2_WAIT(0, &DmaR_Evt1); /* Wait previous DMA read ImageIn */
		if (_SN_ImageIn) {
			AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) ImageIn+_N_ImageIn), ((AT_L2_INT_ADDR_TYPE) II_L1_Memory+0+3864*((T0Ind_Total+1)%2)),
					_SN_ImageIn, 0, &DmaR_Evt1);
		}
		/*============================= End Read Tiles ======================================*/
		/*====================== Call Kernel LOC_LOOP =========================*/
		KerArg1->In = (unsigned char * __restrict__) (II_L1_Memory+0+3864*((T0Ind_Total)%2));
		KerArg1->H = (unsigned int) (T0Ind_Last?1:24);
		KerArg1->IntegralImage = (unsigned int * __restrict__) (II_L1_Memory+8372+15456*((T0Ind_Total)%2));
		KerArg1->KerBuffer = (unsigned int * __restrict__) (II_L1_Memory+7728);
		AT_FORK(gap_ncore(), (void *) KerProcess, (void *) KerArg1);
		__CALL(KerProcess, KerArg1);
		/*================================= Write Tiles =====================================*/
		if (_SP_IntegralImage) AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write IntegralImage */
		AT_L2_COPY(0, ((AT_L2_EXT_ADDR_TYPE) IntegralImage+_C_IntegralImage), ((AT_L2_INT_ADDR_TYPE) II_L1_Memory+8372+15456*((T0Ind_Total)%2)),
				_SC_IntegralImage, 1, &DmaW_Evt1);
		/*============================= End Write Tiles =====================================*/
		/*================================= Update Arg Pipeline =============================*/
		
		_SP_IntegralImage = _SC_IntegralImage;
		/*============================= End Update Arg Pipeline =============================*/
		/*================================= Prepare Tiles ===================================*/
		_SC_IntegralImage = 0;
		if (!(T0Ind_Last)) {
			_C_IntegralImage = _C_IntegralImage + (15456); _SC_IntegralImage = ((T0Ind_NextLast)?644:15456); 
		}
		/*============================= End Prepare Tiles ===================================*/
	} /* End iteration on Tile0 */
	/*================================ Write Tiles Epilog ===============================*/
	AT_L2_WAIT(0, &DmaW_Evt1); /* Wait previous DMA write IntegralImage */
	/*============================ End Write Tiles Epilog ===============================*/
}
