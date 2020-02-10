#include <stdint.h>
#include <stdio.h>
#include "AutoTilerLib.h"
#include "CNN_Generators.h"


void mnistModel(unsigned int L1Memory, unsigned int L2Memory, unsigned int L3Memory, unsigned int L3Flash)
{
    KernelOper_T Cop = KOP_CONV;

    // SetKernelOpts(KER_OPT_NONE, KER_OPT_BUFFER_PROMOTE);
    SetSymbolDynamics();

    SetUsedFilesNames(0, 2, "CNN_BasicKernels.h", "mnist.h");
    SetGeneratedFilesNames("mnistKernels.c", "mnistKernels.h");
    AT_SetGraphCtrl(AT_GRAPH_MONITOR_CYCLES, AT_OPT_ON);
    AT_SetGraphCtrl(AT_GRAPH_PRODUCE_NODE_NAMES, AT_OPT_ON);
    AT_SetGraphCtrl(AT_GRAPH_PRODUCE_OPERINFOS, AT_OPT_ON);

    SetMemoryDeviceInfos(4,
        AT_MEM_L1, L1Memory, "mnist_L1_Memory", 0, 0,
        AT_MEM_L2, L2Memory, "mnist_L2_Memory", 0, 1,
        AT_MEM_L3_HRAM, L3Memory, "mnist_L3_Memory", 0, 0,
        AT_MEM_L3_HFLASH, L3Flash, "mnist_L3_Flash", "mnist_L3_Flash_Const.dat", 0
    );

    LoadCNNLibrary();

    // generator for DEPTHWISE_CONV_2D_0_0_fusion
    CNN_ConvolutionPoolReLU("S1_Conv2d_32x1x5x5_Relu", 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 32, 28, 28,
        KOP_CONV_DP, 5, 5, 1, 1, 1, 1, 0,
        KOP_NONE, 0, 0, 0, 0, 0, 0, 0, KOP_RELU);
    // generator for CONV_2D_0_1_fusion
    CNN_ConvolutionPoolReLU("S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu", 0, 1, 1, 1, 1, 1, 1, 1, 1, 32, 64, 24, 24,
        KOP_CONV_DP, 5, 5, 1, 1, 1, 1, 0,
        KOP_MAXPOOL, 2, 2, 1, 1, 2, 2, 0, KOP_RELU);
    // generator for MAX_POOL_2D_0_3
    CNN_PoolReLU("S3_MaxPool_2x2", 0, 1, 1, 1, 1, 64, 64, 10, 10,
        KOP_MAXPOOL, 2, 2, 1, 1, 2, 2, 0, KOP_NONE);
    // generator for FULLY_CONNECTED_0_4
    CNN_LinearReLU("S4_Linear_10x64x5x5", 0, 1, 1, 1, 1, 1, 1, 1, 1, 1600, 10,
        KOP_LINEAR, KOP_NONE);
    // generator for SOFTMAX_0_5
    CNN_SoftMax("S5_SoftMax", 0, 1, 2, 1, 1, 10, KOP_SOFTMAX);

#define GRAPH
#ifdef GRAPH
    CreateGraph("mnistCNN",
        /* Arguments either passed or globals */
        CArgs(8,
            TCArgInfo("signed char *__restrict__", "Input_1", ARG_SCOPE_ARG, ARG_DIR_IN, AT_MEM_L2, AT_MEM_L2, 0),
            TCArgInfo("signed char *__restrict__", "Step1Weights", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step1Weights.tensor", 1, 1, 8, 7)),
            TCArgInfo("signed char *__restrict__", "Step1Biases", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step1Biases.tensor", 1, 1, 8, 7)),
            TCArgInfo("signed char *__restrict__", "Step2Weights", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step2Weights.tensor", 1, 1, 8, 7)),
            TCArgInfo("signed char *__restrict__", "Step2Biases", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step2Biases.tensor", 1, 1, 8, 7)),
            TCArgInfo("signed char *__restrict__", "Step4Weights", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step4Weights.tensor", 1, 1, 8, 7)),
            TCArgInfo("signed char *__restrict__", "Step4Biases", ARG_SCOPE_GLOBAL, ARG_DIR_CONSTIN, AT_MEM_L3_HFLASH, AT_MEM_UNDEF, ConstInfo("BUILD_MODEL_8BIT/tensors/Step4Biases.tensor", 1, 1, 8, 7)),
            TCArgInfo("short int *__restrict__", "Output_1", ARG_SCOPE_ARG, ARG_DIR_OUT, AT_MEM_L2, AT_MEM_L2, 0)
        ),
        /* Locals, allocated dynamically */
        CArgs(4,
            TCArgInfo("signed char *__restrict__", "OutputStep1", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char *__restrict__", "OutputStep2", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char *__restrict__", "OutputStep3", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0),
            TCArgInfo("signed char *__restrict__", "OutputStep4", ARG_SCOPE_LOCAL, ARG_DIR_INOUT, AT_MEM_UNDEF, AT_MEM_UNDEF, 0)
        )
    );

    // no concats in graph so not stacked tensors created

    // Node S1_Conv2d_32x1x5x5_Relu inq 7 weightsq 7 outq 6 biasesq 7
    AddNode("S1_Conv2d_32x1x5x5_Relu", Bindings(6, GNodeArg(GNA_IN, "Input_1", 0), GNodeArg(GNA_IN, "Step1Weights", 0), GNodeArg(GNA_IN, "Step1Biases", 0), GNodeArg(GNA_OUT, "OutputStep1", 0), Imm(8), Imm(9)));
    // Node S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu inq 6 weightsq 7 outq 4 biasesq 7
    AddNode("S2_Conv2d_64x32x5x5_MaxPool_2x2_Relu", Bindings(6, GNodeArg(GNA_IN, "OutputStep1", 0), GNodeArg(GNA_IN, "Step2Weights", 0), GNodeArg(GNA_IN, "Step2Biases", 0), GNodeArg(GNA_OUT, "OutputStep2", 0), Imm(9), Imm(12)));
    // Node MAX_POOL_2D_0_3 inq 4 outq 4
    AddNode("S3_MaxPool_2x2", Bindings(2, GNodeArg(GNA_IN, "OutputStep2", 0), GNodeArg(GNA_OUT, "OutputStep3", 0)));
    // Node FULLY_CONNECTED_0_4 inq 4 weightsq 7 outq 2
    AddNode("S4_Linear_10x64x5x5", Bindings(6, GNodeArg(GNA_IN, "OutputStep3", 0), GNodeArg(GNA_IN, "Step4Weights", 0), GNodeArg(GNA_IN, "Step4Biases", 0), GNodeArg(GNA_OUT, "OutputStep4", 0), Imm(9), Imm(4)));
    // Node SOFTMAX_0_5 inq 2 outq 15
    AddNode("S5_SoftMax", Bindings(3, GNodeArg(GNA_IN, "OutputStep4", 0), GNodeArg(GNA_OUT, "Output_1", 0), Imm(2)));
    CloseGraph();
#endif
}

int main(int argc, char **argv)

{
    if (TilerParseOptions(argc, argv)) {
            printf("Failed to initialize or incorrect output arguments directory.\n"); return 1;
    }
    mnistModel(52000, 300*1024, 8*1024*1024, 20*1024*1024);
    GenerateTilingCode();
    return 0;
}
