#include "cropping.h"

#if CROPPING == 1
	unsigned char * origin 		= (unsigned char *) L2_image;
	unsigned char * ptr_crop 	= (unsigned char *) L2_image;
	int 			init_offset = CAM_FULLRES_W * LL_Y + LL_X; 
	int 			outid 		= 0;
	
	for(int i=0; i<CAM_CROP_H; i++) {	
		rt_event_execute(NULL, 0);
		unsigned char * line = ptr_crop + init_offset + CAM_FULLRES_W * i;
		for(int j=0; j<CAM_CROP_W; j++) {
			origin[outid] = line[j];
			outid++;
		}
	}
#endif

#if defined(CROPPING) && CROPPING == 0
	rt_img_slice_t slicer;
	slicer.slice_ll.x = LL_X;
	slicer.slice_ll.y = LL_Y;
	slicer.slice_ur.x = UR_X;
	slicer.slice_ur.y = UR_Y;
	
	rt_cam_control(camera, CMD_START, 0);
	rt_cam_control(camera, CMD_SLICE, &slicer);
#endif