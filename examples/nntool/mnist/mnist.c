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
#endif  /* __EMUL__ */

/* Autotiler includes. */
#include "mnist.h"
#include "mnistKernels.h"
#include "ImgIO.h"

#define pmsis_exit(n) exit(n)

#ifndef STACK_SIZE
#define STACK_SIZE      1024
#endif

#define CAMERA
// #define MNIST_16BIT
// #define PRINT_IMAGE

AT_HYPERFLASH_FS_EXT_ADDR_TYPE mnist_L3_Flash = 0;

// Softmax always outputs Q15 short int even from 8 bit input
L2_MEM short int *ResOut;
#ifdef QUANT_16BIT
  typedef short int image_in_t;
#else
  #ifdef QUANT_8BIT
  typedef signed char image_in_t;
  #endif
#endif

#ifdef __EMUL__
#undef PERF
#endif

#define AT_INPUT_SIZE (AT_INPUT_WIDTH*AT_INPUT_HEIGHT*AT_INPUT_COLORS)
#define AT_INPUT_SIZE_BYTES (AT_INPUT_SIZE*sizeof(image_in_t))

#ifdef PERF
L2_MEM rt_perf_t *cluster_perf;
#endif

//UART init param
L2_MEM struct pi_uart_conf uart_conf;
L2_MEM struct pi_device uart;
L2_MEM uint8_t rec_digit = 0;

//camera init parameters
#define CAM_WIDTH    324
#define CAM_HEIGHT   244

L2_MEM struct pi_device himax;
// L2_MEM unsigned char *imgBuff0;
L2_MEM struct pi_himax_conf cam_conf;
L2_MEM uint8_t errors = 0;

static void RunMnist()

L2_MEM image_in_t *ImageIn;

char *ImageName = NULL;

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
  //Checki Results            
  //here the highest probability of the 10 digits is selected
  //from here the result can also be send to the CF via UART
  short int highest = ResOut[0];
  for(int i = 1; i < 10; i++) {
    if(ResOut[i] > highest) {
      highest = ResOut[i];
      rec_digit = i;
    }
  }
  printf("\n");

  printf("Recognized digit: %d with softmax output %d\n", rec_digit, highest);
  for(int j = -10; j < 10; j++){
    printf("digit %d and its softmax output %d\n", j, ResOut[j]);
  }
#else
  printf("image loading disabled so no sensible result\n");
#endif
}

int test_mnist(void)
{
    printf("Entering main controller\n");
#ifndef DONT_DUMP
    if (dt_open_dump_file(TENSOR_DUMP_FILE))
#endif 

    unsigned int Wi = 0, Hi = 0;
    /* Input image size. */
  // #if !defined CAMERA
    unsigned int W = 324, H = 244;
  // #endif

  // #if defined CAMERA  
  //   unsigned int W = 324, H = 244;
  // #endif

    #if !defined(__EMUL__)
    #if !defined(NO_IMAGE) && !defined(LINK_IMAGE_HEADER)
    BRIDGE_Init();
    printf("Connecting to bridge !\n");
    BRIDGE_Connect(1, NULL);
    printf("Connected to bridge !\n");
    #endif  /* NO_IMAGE && LINK_IMAGE_HEADER */
    #endif  /* __EMUL__ */

// HIMAX CAMERA STUFF
    #if defined(CAMERA)
    printf("[CAMERA] Start\n");
    // unsigned char *ImageInChar = (unsigned char *) pi_l2_malloc(sizeof(MNIST_IMAGE_IN_T) * W * H);
    unsigned char *ImageInChar = (unsigned char *)pi_l2_malloc((CAM_WIDTH*CAM_HEIGHT)*sizeof(MNIST_IMAGE_IN_T));
    // unsigned char *imgBuff0 = (unsigned char *) pi_l2_malloc((CAM_WIDTH*CAM_HEIGHT)*sizeof(unsigned char));
    if (ImageInChar == NULL)
    {
      printf("[CAMERA] Failed to allocate memory for image\n");
    }

    pi_himax_conf_init(&cam_conf);
    cam_conf.i2c_itf = 0;
    pi_open_from_conf(&himax, &cam_conf);

    if (pi_camera_open(&himax))
    {
      printf("[CAMERA] Failed to open camera driver\n");
      errors++;
      goto end;
    }
    pi_camera_control(&himax, PI_CAMERA_CMD_START, 0);
    pi_time_wait_us(4000000);
    pi_camera_capture(&himax, ImageInChar, CAM_WIDTH*CAM_HEIGHT);
    pi_camera_control(&himax, PI_CAMERA_CMD_STOP, 0);
    // pi_camera_close(&himax);
  
  #endif 

#if !defined(CAMERA)
// allocating memory for the manual image upload 
    unsigned char *ImageInChar = (unsigned char *) pi_l2_malloc(sizeof(MNIST_IMAGE_IN_T) * W * H);
    if (ImageInChar == NULL)
    {
        printf("Failed to open tensor dump file %s.\n", TENSOR_DUMP_FILE);
        exit(-2);
    }
#if !defined(NO_IMAGE)              
    printf("Reading image\n");
    //Reading Image from Bridge
#ifdef QUANT_8BIT
  #define SHIFT 1
  #define SHORT 0
#else
  #define SHORT 1
  #define SHIFT 0
#endif

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
#endif  /* NO_IMAGE */
#endif  /* NO CAMERA */

#if defined(PRINT_IMAGE)
    for (int i=0; i<H; i++)
    {
        for (int j=0; j<W; j++)
        {
            printf("%03d, ", ImageInChar[W*i + j]);
        }
        printf("\n");
    }
#endif  /* PRINT_IMAGE */

    ResOut = (short int *) AT_L2_ALLOC(0, 10 * sizeof(short int));
    if (ResOut == NULL)
    {
        printf("Failed to allocate Memory for Result (%d bytes)\n", 10*sizeof(short int));
        pmsis_exit(-3);
    }

//  UART configure init
    pi_uart_conf_init(&uart_conf);
    uart_conf.enable_tx = 1;
    uart_conf.enable_rx = 0;
    #if !defined(__PULP_OS__)
    conf.src_clock_Hz = pi_fll_get_frequency(FLL_SOC);
    #endif  /* __PULP_OS__ */
    pi_open_from_conf(&uart, &uart_conf);
    if (pi_uart_open(&uart))
    {
        printf("Uart open failed !\n");
        pmsis_exit(-1);
    }

    #if !defined(__EMUL__)
    /* Configure And open cluster. */
    struct pi_device cluster_dev;
    struct pi_cluster_conf cl_conf;
    cl_conf.id = 0;
    pi_open_from_conf(&cluster_dev, (void *) &cl_conf);
    if (pi_cluster_open(&cluster_dev))
    {
        printf("Cluster open failed !\n");
        pmsis_exit(-4);
    }
    #endif  /* __EMUL__ */

    printf("Constructor\n");
    // IMPORTANT - MUST BE CALLED AFTER THE CLUSTER IS SWITCHED ON!!!!
    if (mnistCNN_Construct())
    {
        printf("Graph constructor exited with an error\n");
        pmsis_exit(-5);
    }

    printf("Call cluster\n");
    #if !defined(__EMUL__)
    struct pi_cluster_task task = {0};
    task.entry = cluster;
    task.arg = NULL;
    task.stack_size = (unsigned int) STACK_SIZE;
    task.slave_stack_size = (unsigned int) SLAVE_STACK_SIZE;

    pi_cluster_send_task_to_cl(&cluster_dev, &task);
    #else
    cluster();
    #endif
    pi_uart_write(&uart, &rec_digit, 1);
    mnistCNN_Destruct();
    

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



  #if defined CAMERA
  end:
    pi_l2_free(ImageInChar, (CAM_WIDTH*CAM_HEIGHT)*sizeof(unsigned char));
    pi_camera_close(&himax);
  #endif


    pi_uart_close(&uart);
#ifndef __EMUL__    
  // Close the cluster
    pi_cluster_close(&cluster_dev);
#endif  /* __EMUL__ */
#ifndef DONT_DUMP
    dt_close_dump_file();
#endif
    AT_L2_FREE(0, ImageIn, AT_INPUT_SIZE_BYTES);
    AT_L2_FREE(0, ResOut, 10 * sizeof(short int));
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
    ImageName = "../../../samples/5223_5.pgm";
    #endif  /* LINK_IMAGE_NAME */
    printf("\n\n\t *** NNTOOL Mnist Example ***\n\n");
    return pmsis_kickoff((void *) test_mnist);
}
#endif  /* (__EMUL__) && (LINK_IMAGE_HEADER) */
