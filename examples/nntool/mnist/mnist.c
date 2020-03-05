/*
 * Copyright (C) 2017 GreenWaves Technologies
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 *
 */

#ifndef __EMUL__
/* PMSIS includes. */
#include "pmsis.h"
#include "pmsis/drivers/uart.h"
#include "bsp/camera/himax.h"
// #include "tools/frame_streamer.h"
#endif  /* __EMUL__ */

/* Autotiler includes. */
#include "mnist.h"
#include "mnistKernels.h"
#include "ImgIO.h"

#define pmsis_exit(n) exit(n)

#ifndef STACK_SIZE
#define STACK_SIZE      1024
#endif

#ifndef TENSOR_DUMP_FILE
  #define TENSOR_DUMP_FILE "tensor_dump_file.dat"
#endif

#ifdef QUANT_16BIT
  typedef short int image_in_t;
#else
  #ifdef QUANT_8BIT
  typedef unsigned char image_in_t;
  #endif
#endif

#ifdef __EMUL__
#undef PERF
#endif

#define AT_INPUT_SIZE (AT_INPUT_WIDTH*AT_INPUT_HEIGHT*AT_INPUT_COLORS)
#define AT_INPUT_SIZE_BYTES (AT_INPUT_SIZE*sizeof(image_in_t))


// Init memory pointers
// Softmax always outputs Q15 short int even from 8 bit input
L2_MEM short int *ResOut;
L2_MEM image_in_t *ImageIn;
L2_MEM image_in_t *ImageInCam;
AT_HYPERFLASH_FS_EXT_ADDR_TYPE mnist_L3_Flash = 0;

//UART init param
L2_MEM struct pi_uart_conf uart_conf;
L2_MEM struct pi_device uart;
L2_MEM uint8_t rec_digit = -1;

// #define PRINT_IMAGE
// #define REAL_TIME

//camera init parameters
// #define CAMERA
#define CAM_WIDTH    324
#define CAM_HEIGHT   244

//cropping option for camera 
// #define CAM_CROP_W   150 
// #define CAM_CROP_H   150
#define AT_CAMERA_INPUT_SIZE_BYTES (CAM_WIDTH*CAM_HEIGHT*sizeof(image_in_t))

L2_MEM struct pi_device himax;
L2_MEM struct pi_himax_conf cam_conf;
L2_MEM pi_cl_alloc_req_t alloc_req;
// L2_MEM struct pi_task_t cam_task;

char *ImageName = NULL;

int MemoryPrint(int W, int H, image_in_t *ImageIn)
{
  for (int i=0; i<H; i++)
  {
    for (int j=0; j<W; j++)
    {
      printf("%03d, ", ImageIn[W* i + j]);
    }
    printf("\n");
  }
}

static void cluster()
{
  printf("Running on cluster\n");
#ifdef PERF
  printf("Start timer\n");
  gap_cl_starttimer();
  gap_cl_resethwtimer();
#endif
  mnistCNN(ImageIn, ResOut);
  printf("Runner completed\n");

#ifndef NO_IMAGE
  //Check i Results            
  short int highest = ResOut[0];
  for(int i = 1; i < 10; i++) {
    if(ResOut[i] > highest) {
      highest = ResOut[i];
      rec_digit = i;
    }
  }
  printf("Recognized digit: %d with softmax output %d\n", rec_digit, highest);
  // for(int j = -10; j < 10; j++){
  //   printf("digit %d and its softmax output %d\n", j, ResOut[j]);
  // }
#else
  printf("image loading disabled so no sensible result\n");
#endif
}

int test_mnist(void)
{
  printf("Entering main controller\n");
#ifndef DONT_DUMP
  if (dt_open_dump_file(TENSOR_DUMP_FILE))
  {
    printf("Failed to open tensor dump file %s.\n", TENSOR_DUMP_FILE);
    exit(-2);
  }
#endif 

#ifdef QUANT_8BIT
  #define SHIFT 1
  #define SHORT 0
#else
  #define SHORT 1
  #define SHIFT 0
#endif

// HIMAX CAMERA init, configure, execute 
#if defined(CAMERA)
  ImageInCam = (image_in_t *) pi_l2_malloc(AT_CAMERA_INPUT_SIZE_BYTES);
  if (ImageInCam == NULL) 
  {
    printf("[CAMERA] Failed to allocate memory for image\n");
  }

  pi_himax_conf_init(&cam_conf);
  cam_conf.i2c_itf = 0;
  cam_conf.format = PI_CAMERA_QQVGA;        //160 by 120
  pi_open_from_conf(&himax, &cam_conf);
  printf("[CAMERA] Open");
  if (pi_camera_open(&himax))
  {
    printf("[CAMERA] Failed to open camera driver\n");
    pmsis_exit(-7);
  }
  // static pi_buffer_t buffer;
  // pi_buffer_init(&buffer, PI_BUFFER_TYPE_L2, ImageInCam);
  // pi_buffer_set_format(&buffer, CAM_WIDTH, CAM_HEIGHT, 1, PI_BUFFER_FORMAT_GRAY); 

  pi_camera_control(&himax, PI_CAMERA_CMD_START, 0);
  printf("[CAMERA] Start\n");
  pi_time_wait_us(1000000);
  pi_camera_capture(&himax, ImageInCam, CAM_WIDTH*CAM_HEIGHT);
  // pi_task_t *task_capture;
  // task_capture = pi_task_block();
  // pi_camera_capture_async(&himax, ImageInCam, CAM_WIDTH*CAM_HEIGHT, task_capture);
  // pi_task_wait_on(task_capture);
  pi_camera_control(&himax, PI_CAMERA_CMD_STOP, 0);
  printf("Captured image\n");
#endif

#if defined(PRINT_IMAGE) && defined(CAMERA)
  printf("AFTER Camera capture\n");
  MemoryPrint(CAM_WIDTH, CAM_HEIGHT, ImageInCam);
#endif  /* PRINT_IMAGE */  
  
// Using images from PC
#if !defined(NO_IMAGE) && !defined(CAMERA)          
  printf("Reading image\n");
  //Reading Image from Bridge
  if (!(ImageIn = (image_in_t *) AT_L2_ALLOC(0, AT_INPUT_SIZE_BYTES))) {
      printf("Failed to allocate %ld bytes for %s\n", AT_INPUT_SIZE_BYTES, ImageName);
      pmsis_exit(-1);
  }

  if (ReadImageFromFile(ImageName, AT_INPUT_WIDTH, AT_INPUT_HEIGHT, AT_INPUT_COLORS, ImageIn, AT_INPUT_SIZE_BYTES, SHIFT, SHORT))
  {
      printf("Failed to load image %s\n", ImageName);
      pmsis_exit(-2);
  }    
  printf("Finished reading image\n");
#endif  /* NO_IMAGE && NO CAMERA */

#if defined(PRINT_IMAGE)
  MemoryPrint(AT_INPUT_WIDTH, AT_INPUT_HEIGHT, ImageIn);
#endif  /* PRINT_IMAGE */  

// Allocate memory for output of Mnist network
  // ResOut = (short int *) AT_L2_ALLOC(0, 10 * sizeof(short int));
  ResOut = (short int *) pi_l2_malloc(10 * sizeof(short int));
  if (ResOut == NULL)
  {
      printf("Failed to allocate Memory for Result (%d bytes)\n", 10*sizeof(short int));
      pmsis_exit(-3);
  }

//  UART init and configure
  pi_uart_conf_init(&uart_conf);
  uart_conf.enable_tx = 1;
  uart_conf.enable_rx = 0;
  pi_open_from_conf(&uart, &uart_conf);
  printf("[UART] Open\n");
  if (pi_uart_open(&uart))
  {
      printf("[UART] open failed !\n");
      pmsis_exit(-1);
  }
// Cluster init and configure
#if !defined(__EMUL__)
  struct pi_device cluster_dev;
  struct pi_cluster_conf cl_conf;
  cl_conf.id = 0;
  pi_open_from_conf(&cluster_dev, (void *) &cl_conf);
  printf("[CLUSTER] Open\n");
  if (pi_cluster_open(&cluster_dev))
  {
      printf("[CLUSTER] open failed !\n");
      pmsis_exit(-4);
  }
  
#endif  /* __EMUL__ */

  printf("Constructor\n\n");
  // IMPORTANT - MUST BE CALLED AFTER THE CLUSTER IS SWITCHED ON!!!!
  if (mnistCNN_Construct())
  {
      printf("Graph constructor exited with an error\n");
      pmsis_exit(-5);
  }

#if !defined(__EMUL__)
  printf("Setup cluster task\n");
  struct pi_cluster_task task = {0};
  // pi_l2_malloc(&task, sizeof(pi_cluster_task));
  task.entry = cluster;
  task.arg = NULL;
  task.stack_size = (unsigned int) STACK_SIZE;
  task.slave_stack_size = (unsigned int) SLAVE_STACK_SIZE;
  // printf("reached");
  #if !defined (REAL_TIME)
  pi_cluster_send_task_to_cl(&cluster_dev, &task);
  #endif
#else 
  cluster()
#endif  /*! __EMUL__ */  

#if !defined (REAL_TIME)
  printf("Send uart byte to Crazyflie: %d\n\n",rec_digit);
  pi_uart_write(&uart, &rec_digit, 1);          
#endif

#if defined (REAL_TIME)
  for (int i=0; i<4; i++)
  {   
  #if defined(CAMERA)
    pi_camera_capture(&himax, ImageInCam, CAM_WIDTH*CAM_HEIGHT);
    printf("Captured image\n");
  #endif 

    // Print image
  #if defined(PRINT_IMAGE)
    printf("Image found in memory and used by cluster task\n");
    MemoryPrint(AT_INPUT_WIDTH, AT_INPUT_HEIGHT, ImageIn);
  #endif  /* PRINT_IMAGE */

  // Cluster task
    printf("Call cluster\n");
    pi_cluster_send_task_to_cl(&cluster_dev, &task);
    printf("Send uart byte to Crazyflie: %d\n\n",rec_digit);
    pi_uart_write(&uart, &rec_digit, 1);          
  }   // looping
#endif

  mnistCNN_Destruct();

// Performance check
#ifdef PERF
{
  unsigned int TotalCycles = 0, TotalOper = 0;
  printf("\n");
  for (int i=0; i<(sizeof(AT_GraphPerf)/sizeof(unsigned int)); i++) {
    printf("%45s: Cycles: %10d, Operations: %10d, Operations/Cycle: %f\n", AT_GraphNodeNames[i], AT_GraphPerf[i], AT_GraphOperInfosNames[i], ((float) AT_GraphOperInfosNames[i])/ AT_GraphPerf[i]);
    TotalCycles += AT_GraphPerf[i]; TotalOper += AT_GraphOperInfosNames[i];
  }
  printf("\n");
  printf("%45s: Cycles: %10d, Operations: %10d, Operations/Cycle: %f\n", "Total", TotalCycles, TotalOper, ((float) TotalOper)/ TotalCycles);
  printf("\n");
}
#endif

#ifndef __EMUL__    
// Close the cluster
  pi_cluster_close(&cluster_dev);
  printf("[CLUSTER] closed\n");
#endif  /* __EMUL__ */

  pi_uart_close(&uart);
  printf("[UART] closed\n");

// Closing camera, uart, cluster
#if defined (CAMERA)
  pi_l2_free(ImageInCam, AT_CAMERA_INPUT_SIZE_BYTES);
  pi_camera_close(&himax);
  printf("[CAMERA] closed\n");
#endif

#if !defined(CAMERA)
  pi_l2_free(ImageIn, AT_INPUT_SIZE_BYTES);
#endif

#ifndef DONT_DUMP
  dt_close_dump_file();
#endif

  // pi_l2_free(ImageIn, AT_INPUT_SIZE_BYTES);
  pi_l2_free(ResOut, 10 * sizeof(short int));
  printf("Ended\n");

  pmsis_exit(0);
  return 0;
}

#if defined(__EMUL__) && !defined(LINK_IMAGE_HEADER)
int main(int argc, char *argv[])
{
  if (argc < 2)
  {
      printf("Usage: mnist [image_file]\n");
      exit(-1);
  }
  ImageName = argv[1];
  printf("\n\n\t *** NNTOOL Mnist Example ***\n\n");
  test_mnist();
  return 0;
}
#else
int main()
{
  #if defined(LINK_IMAGE_NAME)
  #define __STRING1(__s) #__s
  #define __STRING(__s) __STRING1(__s)
  ImageName = __STRING(LINK_IMAGE_NAME);
  #else
  // ImageName = "../../../samples/3362_6.pgm";
  ImageName = "../../../samples/gimp16_2.pgm";
  #endif  /* LINK_IMAGE_NAME */
  printf("\n\n\t *** NNTOOL Mnist Example ***\n\n");

  return pmsis_kickoff((void *) test_mnist);
}
#endif  /* (__EMUL__) && (LINK_IMAGE_HEADER) */
