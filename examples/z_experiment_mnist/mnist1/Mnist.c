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
#include "Gap.h"
#include "MnistKernels.h"
#include "ImgIO.h"

#if defined(__PULP_OS__)
#include "bridge_stubs.h"
#endif  /* __PULP_OS__ */

#ifdef __EMUL__
#define pmsis_exit exit
#ifdef PERF
#undef PERF
#endif
#endif

#define CAMERA
// #define MNIST_16BIT
// #define PRINT_IMAGE

#define STACK_SIZE      2048

AT_HYPERFLASH_FS_EXT_ADDR_TYPE Mnist_L3_Flash = 0;

#ifdef __EMUL__
  #include <sys/types.h>
  #include <unistd.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include <sys/param.h>
  #include <string.h>
  #ifndef TENSOR_DUMP_FILE
    #define TENSOR_DUMP_FILE "tensor_dump_file.dat"
  #endif
#endif

// Softmax always outputs Q15 short int even from 8 bit input
L2_MEM short int *ResOut;
#ifdef MNIST_16BIT
  typedef short int MNIST_IMAGE_IN_T;
#else
  #ifdef MNIST_8BIT
  typedef signed char MNIST_IMAGE_IN_T;
  #endif
#endif
L2_MEM MNIST_IMAGE_IN_T *ImageIn;

char *ImageName = NULL;

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

{
  printf("Running on cluster\n");
#ifdef PERF
  gap_cl_starttimer();
  gap_cl_resethwtimer();
#endif
  MnistCNN(ImageIn, ResOut);
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
  for(int j = 0; j < 10; j++){
    printf("digit %d and its softmax output %d\n", j, ResOut[j]);
  }
#else
  printf("image loading disabled so no sensible result\n");
#endif
}

void test_mnist(void)
{
    printf("Entering main controller\n");


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
    pi_time_wait_us(2000000);
    pi_camera_capture(&himax, ImageInChar, CAM_WIDTH*CAM_HEIGHT);
    pi_camera_control(&himax, PI_CAMERA_CMD_STOP, 0);
    // pi_camera_close(&himax);
  
  #endif 

    #if !defined(CAMERA)
// allocating memory for the manual image upload 
    unsigned char *ImageInChar = (unsigned char *) pi_l2_malloc(sizeof(MNIST_IMAGE_IN_T) * W * H);
    if (ImageInChar == NULL)
    {
        printf("Failed to allocate Memory for Image (%d bytes)\n", sizeof(MNIST_IMAGE_IN_T) * W * H);
        pmsis_exit(-1);
    }
    #if !defined(NO_IMAGE)              
    printf("Reading image\n");
    //Reading Image from Bridge
    if ((ReadImageFromFile(ImageName, &Wi, &Hi, ImageInChar, W*H*sizeof(unsigned char))==0) || (Wi!=W) || (Hi!=H))
    {
        printf("Failed to load image %s or dimension mismatch Expects [%dx%d], Got [%dx%d]\n", ImageName, W, H, Wi, Hi);
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

    ResOut = (short int *) pi_l2_malloc(10 * sizeof(short int));
    if (ResOut == NULL)
    {
        printf("Failed to allocate Memory for Result (%d bytes)\n", 10*sizeof(short int));
        pmsis_exit(-3);
    }

    #if defined(MNIST_8BIT)
    printf("Preprocess 8 bit\n");
    ImageIn = (MNIST_IMAGE_IN_T *) ImageInChar;
    for (int i=W*H-1; i>=0; i--)
    {
        ImageIn[i] = (MNIST_IMAGE_IN_T) (ImageInChar[i]>>1);
    }
    #else
    #if defined(MNIST_16BIT)
    printf("Preprocess 16 bit\n");
    ImageIn = (MNIST_IMAGE_IN_T *)ImageInChar;
    for (int i=W*H-1; i>=0; i--)
    {
        ImageIn[i] = ImageInChar[i] << 7;
    }
    #else
    #error No bit size selected
    #endif  /* MNIST_16BIT */
    #endif  /* MNIST_8BIT */

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
    #endif  /* __EMUL__ */
    pi_open_from_conf(&cluster_dev, (void *) &cl_conf);
    if (pi_cluster_open(&cluster_dev))
    {
        printf("Cluster open failed !\n");
        pmsis_exit(-4);
    }

    printf("Constructor\n");
    // IMPORTANT - MUST BE CALLED AFTER THE CLUSTER IS SWITCHED ON!!!!
    if (MnistCNN_Construct())
    {
        printf("Graph constructor exited with an error\n");
        pmsis_exit(-5);
    }

    printf("Call cluster\n");
    struct pi_cluster_task task = {0};
    task.entry = RunMnist;
    task.arg = NULL;
    task.stack_size = (unsigned int) STACK_SIZE;

    pi_cluster_send_task_to_cl(&cluster_dev, &task);
    pi_uart_write(&uart, &rec_digit, 1);

    MnistCNN_Destruct();

    #if defined(PERF)
    {
        unsigned int TotalCycles = 0, TotalOper = 0;
        printf("\n");
        for (unsigned int i=0; i<(sizeof(MnistPerf)/sizeof(unsigned int)); i++)
        {
            printf("%45s: %10d, Operation: %10d, Operation/Cycle: %f\n", MnistLName[i], MnistPerf[i], MnistOperCount[i], ((float) MnistOperCount[i])/ MnistPerf[i]);
            TotalCycles += MnistPerf[i]; TotalOper += MnistOperCount[i];
        }
        printf("\n");
        printf("%45s: %10d, Operation: %10d, Operation/Cycle: %f\n", "Total", TotalCycles, TotalOper, ((float) TotalOper)/ TotalCycles);
        printf("\n");
    }
    #endif  /* PERF */

    pi_uart_close(&uart);

  #if defined CAMERA
  end:
    pi_l2_free(ImageInChar, (CAM_WIDTH*CAM_HEIGHT)*sizeof(unsigned char));
    pi_camera_close(&himax);
  #endif


    // Close the cluster
    pi_cluster_close(&cluster_dev);

    #if defined(__EMUL__)
    dt_close_dump_file();
    #else
    #if !defined(NO_IMAGE) && !defined(LINK_IMAGE_HEADER)
    BRIDGE_Disconnect(NULL);
    #endif  /* NO_IMAGE && LINK_IMAGE_HEADER */
    #endif  /* __EMUL__ */

    printf("Ended\n");

    pmsis_exit(0);
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
    if (dt_open_dump_file(TENSOR_DUMP_FILE))
    {
        printf("Failed to open tensor dump file %s.\n", TENSOR_DUMP_FILE);
        exit(-2);
    }
    printf("\n\n\t *** NNTOOL Mnist Example ***\n\n");
    test_mnist();
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
