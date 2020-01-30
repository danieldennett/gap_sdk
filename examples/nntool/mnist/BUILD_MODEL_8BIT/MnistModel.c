/*
 * Copyright (C) 2017 GreenWaves Technologies
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 *
 */

// Custom template file

#include <stdint.h>
#include <stdio.h>
#include "AutoTilerLib.h"
#include "CNN_Generators.h"

void MnistModel(unsigned int L1Memory, unsigned int L2Memory, unsigned int L3Memory, unsigned int L3Flash)
{
    KernelOper_T Cop = KOP_CONV;

    SetSymbolDynamics();

    AT_SetGraphCtrl(AT_GRAPH_MONITOR_CYCLES, AT_OPT_ON);
    AT_SetGraphCtrl(AT_GRAPH_MONITOR_CVAR_NAME, "MnistPerf");
    AT_SetGraphCtrl(AT_GRAPH_PRODUCE_NODE_NAMES, AT_OPT_ON);
    AT_SetGraphCtrl(AT_GRAPH_PRODUCE_NODE_CVAR_NAME, "MnistLName");
    AT_SetGraphCtrl(AT_GRAPH_NOINLINE_NODE, AT_OPT_ON);

    AT_SetGraphCtrl(AT_GRAPH_PRODUCE_OPERINFOS, AT_OPT_ON);
    AT_SetGraphCtrl(AT_GRAPH_PRODUCE_OPERINFOS_CVAR_NAME, "MnistOperCount");

//    AT_SetGraphCtrl(AT_GRAPH_REORDER_CONSTANT_IN, AT_OPT_ON);

    SetUsedFilesNames(0, 2, "CNN_BasicKernels.h", "Mnist.h");
    SetGeneratedFilesNames("MnistKernels.c", "MnistKernels.h");

    SetMemoryDeviceInfos(4,
        AT_MEM_L1, L1Memory, "Mnist_L1_Memory", 0, 0,
        AT_MEM_L2, L2Memory, "Mnist_L2_Memory", 0, 0,
        AT_MEM_L3_HRAM, L3Memory, "Mnist_L3_Memory", 0, 0,
        AT_MEM_L3_HFLASH, L3Flash, "Mnist_L3_Flash", "Mnist_L3_Flash_Const.dat", 0
    );

    SetAT_TestFile("AT_MnistTest.inc");

    LoadCNNLibrary();

    // input_1
    // DEPTHWISE_CONV_2D_0_0_fusion
    CNN_ConvolutionPoolReLU("S1_Conv2d_32x1x5x5_MaxPool_2x2_Relu", 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 32, 28, 28,
        KOP_CONV, 5, 5, 1, 1, 1, 1, 0,
        KOP_MAXPOOL, 2, 2, 1, 1, 2, 2, 0, KOP_RELU);
    // CONV_2D_0_2_fusion
    CNN_ConvolutionPoolReLU("S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu", 0, 1, 1, 1, 1, 1, 1, 1, 1, 32, 64, 12, 12,
        KOP_CONV_DP, 5, 5, 1, 1, 1, 1, 0,
        KOP_MAXPOOL, 2, 2, 1, 1, 2, 2, 0, KOP_RELU);
    // FULLY_CONNECTED_0_4
    CNN_LinearReLU("S3_Linear_10x64x4x4", 0, 1, 1, 1, 1, 1, 1, 1, 1, 1024, 10,
        KOP_LINEAR, KOP_NONE);
    // SOFTMAX_0_5
    CNN_SoftMax("S4_SoftMax", 0, 1, 2, 1, 1, 10, KOP_SOFTMAX);
    // output_1

#define GRAPH
#ifdef GRAPH
    CreateGraph("MnistCNN",
        /* Arguments either passed or globals */
        CArgs(8,
            TCArgInfo("signed char *__restrict__", "Input_1", ARG_SCOPE_ARG, ARG_DIR_IN, AT_MEM_L2, AT_MEM_L2, 0),
            TCArgInfo("signed char *__restrict__", "Step1Weights", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step1Weights.tensor", 1, 1, 8, 6)),
            TCArgInfo("signed char *__restrict__", "Step1Biases", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step1Biases.tensor", 1, 1, 8, 5)),
            TCArgInfo("signed char *__restrict__", "Step2Weights", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step2Weights.tensor", 1, 1, 8, 6)),
            TCArgInfo("signed char *__restrict__", "Step2Biases", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step2Biases.tensor", 1, 1, 8, 3)),
            TCArgInfo("signed char *__restrict__", "Step3Weights", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step3Weights.tensor", 1, 1, 8, 6)),
            TCArgInfo("signed char *__restrict__", "Step3Biases", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step3Biases.tensor", 1, 1, 8, 1)),
            TCArgInfo("short int *__restrict__", "Output_1", ARG_SCOPE_ARG, ARG_DIR_OUT, AT_MEM_L2, AT_MEM_L2, 0)
        ),
        /* Locals, allocated dynamically */
        CArgs(3,
            TCArgInfo("signed char *__restrict__", "OutputStep2", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char *__restrict__", "OutputStep3", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char *__restrict__", "OutputStep4", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0)
        )
    );

    // Node S1_Conv2d_32x1x5x5_MaxPool_2x2_Relu inq 7 weightsq 6 outq 5
    AddNode("S1_Conv2d_32x1x5x5_MaxPool_2x2_Relu", Bindings(6, GNodeArg(GNA_IN, "Input_1", 0), GNodeArg(GNA_IN, "Step1Weights", 0), GNodeArg(GNA_IN, "Step1Biases", 0), GNodeArg(GNA_OUT, "OutputStep2", 0), Imm(8), Imm(8)));
    // Node S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu inq 5 weightsq 6 outq 3
    AddNode("S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu", Bindings(6, GNodeArg(GNA_IN, "OutputStep2", 0), GNodeArg(GNA_IN, "Step2Weights", 0), GNodeArg(GNA_IN, "Step2Biases", 0), GNodeArg(GNA_OUT, "OutputStep3", 0), Imm(8), Imm(8)));
    // Node FULLY_CONNECTED_0_4 inq 3 weightsq 6 outq 1
    AddNode("S3_Linear_10x64x4x4", Bindings(6, GNodeArg(GNA_IN, "OutputStep3", 0), GNodeArg(GNA_IN, "Step3Weights", 0), GNodeArg(GNA_IN, "Step3Biases", 0), GNodeArg(GNA_OUT, "OutputStep4", 0), Imm(8), Imm(8)));
    // Node SOFTMAX_0_5 inq 1 outq 15
    AddNode("S4_SoftMax", Bindings(3, GNodeArg(GNA_IN, "OutputStep4", 0), GNodeArg(GNA_OUT, "Output_1", 0), Imm(1)));
    CloseGraph();
#endif
}

int main(int argc, char **argv)

{
    if (TilerParseOptions(argc, argv)) {
            printf("Failed to initialize or incorrect output arguments directory.\\n"); return 1;
    }
    MnistModel(52000, 300*1024, 8*1024*1024, 20*1024*1024);
    GenerateTilingCode();
    return 0;
}