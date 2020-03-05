
#ifndef PULP_DRONET_CONFIG
#define PULP_DRONET_CONFIG

/****************************** USER PARAMETERS *******************************/
#define VERBOSE					// Enables additional information
#define CROPPING 		1			// Image cropping enable: 0 in HW, 1 in SW
#define CAM_FULLRES_W	324			// HiMax full width 324
#define CAM_FULLRES_H	244			// HiMax full height 244
#define CAM_CROP_W		200			// Cropped camera width 
#define CAM_CROP_H		200			// Cropped camera height 

/***************************** PRIVATE PARAMETERS *****************************/
#define STACK_SIZE		1200		// Stack size per Core
#define MOUNT			1			// Cluster mount command
#define UNMOUNT			0			// Cluster unmount command
#define CID				0			// Cluster ID

#define	CROPPING_X		1			// Cropping area X (Horizontal/Width): 0=Left, 1=Central, 2=Right
#define	CROPPING_Y		1			// Cropping area Y (Vertical/Height): 0=Top, 1=Central, 2=Bottom
/******************************************************************************/

/****************************** Cropping Setting *******************************
 * PULP-DroNet default cropping is Central/Bottom (X/Y)						   *
 *																			   *
 *											(X)								   *
 *						|	0:Left	|	1:Central	|	2:Right		|		   *
 *						 ___________________________________________		   *
 *			0:Top		|___________|_______________|_______________|		   *
 *	(Y)		1:Central	|___________|_______________|_______________|		   *
 *			2:Bottom	|___________|____Default____|_______________|		   *
 *																			   *
 ******************************************************************************/

#if CROPPING_X==0 		// X Left [0-200]
#define LL_X			0 								// left x coordinate 0
#elif CROPPING_X==1		// X Central [62-262]
#define LL_X			((CAM_FULLRES_W-CAM_CROP_W)/2) 	// left x coordinate 62
#elif CROPPING_X==2		// X Right [124-324]
#define LL_X			(CAM_FULLRES_W-CAM_CROP_W) 		// left x coordinate 124
#endif

#if CROPPING_Y==0 		// Y Top [0-200]
#define LL_Y			0								// up y coordinate 0
#elif CROPPING_Y==1 	// Y Central [22-222]
#define LL_Y			((CAM_FULLRES_H-CAM_CROP_H)/2)	// up y coordinate 22
#elif CROPPING_Y==2 	// Y Bottom [44-244]
#define LL_Y			(CAM_FULLRES_H-CAM_CROP_H)		// up y coordinate 44
#endif

#define UR_X			CAM_CROP_W+LL_X					// right x coordinate
#define UR_Y			CAM_CROP_H+LL_Y 				// bottom y coordinate

/******************************************************************************/

#if !defined(CROPPING) || CROPPING==1
#define CAM_WIDTH		CAM_FULLRES_W
#define CAM_HEIGHT		CAM_FULLRES_H
#else
#define CAM_WIDTH		CAM_CROP_W
#define CAM_HEIGHT		CAM_CROP_H
#endif