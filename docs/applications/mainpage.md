# Canny Edge Detection

## Introduction

Canny Edge Detection is a popular edge detection algorithm. It is a technique to extract useful structural information from different vision objects and dramatically reduce the amount of data to be processed. It has been widely applied in various computer vision systems.

With this application example, we show how this algorithm can be implemented on GAP8.

Thanks to GAP8's extended instruction set, we can finish a MAC operation (Multiply-Accumulate) of 2 four-vectors in only one cycle. To achieve maximum performance we use this by vectorizing the main operations of algorithm.

For instance, in the first step of the algorithm, which applies a Gaussian Filter (5 * 5) to smooth the image we have used seven 4 element vectors to represent the 5 * 5 filter. Please read the comments in the source code for further information.

## How to build and execute the application

Please configure your shell for the SDK by running the `GAP` command as indicated in the GAP8 SDK Getting Started Guide.

To run the example type:

~~~~~sh
make clean all run
~~~~~

## Result

The result shows you the number of cycles used in each part of the algorithm when running the 8 cluster cores. The example operate on Pedestrian.pgm image and produces img_OUT.ppm which contains the image after canny edge elaboration.

An example of the output is:

~~~~~sh
Start execution on GAP8
Start of application
bridge connecting
Tcp_listener: client connected (fd 12)
client connected
Bridge connected, waiting for load image
Image ../../../Pedestrian.pgm, [W: 320, H: 240], Gray, Size: 76800 bytes, Loaded sucessfully
cluster master start
Canny Edge Detector running on 8 cores, Source Mono image[W=320, H=240]
Entering Master, W=320, H=240, 17 iterations, read stripes: 21 lines, wrote stripe: 13 lines
Conv5x5_Byte_Vectorial                :     473669 Cycles
EdgeIntensityAndOrientation_Vectorial :     738829 Cycles
CannyRemoveNonMax                     :     530711 Cycles
CannyBlobAnalysis                     :     380492 Cycles
CannyRemoveWeakEdges                  :      27190 Cycles
Total                                 :    2150891 Cycles
Total with Master                     :    1197378 Cycles
imgName: ../../../img_OUT.ppm
Writing image  [#                             ]
Writing image  [####                          ]
Writing image  [#######                       ]
Writing image  [###########                   ]
Writing image  [##############                ]
Writing image  [#################             ]
Writing image  [#####################         ]
Writing image  [########################      ]
Writing image  [###########################   ]
Detected end of application, exiting with status: 0
Loop exited
commands completed
~~~~~

With a core frequency of 100MHz this would execute in \~12ms if all 8 cores are used.
# Face Detection

## Introduction

The face detection example included in the GAP8 SDK is based on Viola-Jones face detection algorithm.

Algorithms kernels:

1. Image Resize
2. Integral Image
3. Squared Integral Image
4. Cascade Detector

## Algorithm configuration:

The detector is configured to detect faces from 64x48 px size to 40x30 px size. This is achieved using 3 pyramid layers.


## How to Build and Execute the Application

Please configure your shell for the SDK by running the `GAP` command as indicated in the GAP8 SDK Getting Started Guide.

To run the example type:

~~~~~sh
make clean all run
~~~~~

### Change Input-Output Configuration

The example can run with two input-output configurations:

1.  **Input and Output from file on host PC (default configuration)**
2.  **Input from Camera and output to LCD screen**

There are no additional requirements for mode 1, while for mode two the Himax camera must be connected to cpi camera interface and an additional LCD screen must be mounted on the arduino connector. The driver included in this example supports [2.8 TFT LCD Shield based on ili9341 controller](https://www.adafruit.com/products/1947).

To select between confiration 1 and 2 a define must be changed within the file main.c.
To select configuration 1 (deafult) set macro:

~~~~~c
#define FROM_FILE 1
~~~~~
while to select configuration 2:

~~~~~c
#define FROM_FILE 0
~~~~~

After changing configuration the project must be rebuilt and run:

~~~~~sh
make clean all run
~~~~~

### Change Input Image

To change the input image the following two steps are required.

In the main.c file change the name of the input file:

~~~~c
char *Imagefile = "MyNewImage.ppm";
~~~~

So far only images of 324x244 size are supported.
# Multiscale Pedestrian Detection

## Introduction

The Pedestrian detection example included in the GAP8 SDK is implemented using a fixed point implementation of Histogram of Oriented Gradients (HOG) (See citation 1 below). The machine learning scheme used to classify HoG descriptor is a window-based boosting technique (See citation 2 below). [Figure 1] shows an algorithm overview.

![Figure 1 - Algorithm overview](./images/pedestrian_overview.png){ width=500px }

The diagram shows the main steps of Pedestrian Detection on GAP8. First the image is sub-sampled multiple times to create an pyramid of images. Then on each layer of the pyramid the HOG features are computed. A fixed size sliding windows detector is employed on each of the feature pyramid layers to determine if the features window belongs to a pedestrian or not. The detector compares the feature with a pre-trained model with a fast boosted decision tree scheme.

### Histogram of Oriented Gradients (HoG)

Histogram of oriented gradients (HOG) is a well known features extractor algorithm. The algorithm is composed of two hierarchical parts. The image is divided in tiles (in this example 8 x 8 pixels) called Cells and then on each cell the Histogram of Gradients is computed on 9 different angles. Thus the histogram has 9 bins.

Once this step is done for each cell, the cells are grouped in blocks (in this example 2 x 2 cells with stride of one cell). For each block the feature is normalized and an output of 36 bins (4 * 9) is produced.

For a more detailed explanation of the algorithm you can refer to these resources:

* [Learn OpenCV Website](https://www.learnopencv.com/histogram-of-oriented-gradients/)
* [Original Dalal and Triggs Paper](http://ieeexplore.ieee.org/document/1467360/)

### Sliding Window Detector

The sliding windows detector tests for the existence of a pedestrian in each location in the input feature with a given stride. The windows is 128 x 64 pixels which gives a feature size of 3780 values. The machine learning algorithm used to classify pedestrians is based on small boosted trees.

\newpage


## Algorithm configuration:

The detector is configured to recognize pedestrian from 96x48 pixel size to 176x88 pixel size. This is achieved using 6 pyramid layers.


## How to Build and Execute the Application

Please configure your shell for the SDK by running the `GAP` command as indicated in the GAP8 SDK Getting Started Guide.

To run the example type:

~~~~~sh
make clean all run
~~~~~

### Change Input-Output Configuration

The example can run with two input-output configurations:

1.  **Input and Output from file on host PC (default configuration)**
2.  **Input from Camera and output to LCD screen**

There are no additional requirements for mode 1, while for mode two the Himax camera must be connected to cpi camera interface and an additional LCD screen must be mounted on the arduino connector. The driver included in this example supports [2.8 TFT LCD Shield based on ili9341 controller](https://www.adafruit.com/products/1947).

To select between confiration 1 and 2 a define must be changed within the file HoGEstim.c.
To select configuration 1:

~~~~~c
#define FROM_FILE 1
~~~~~
and to select configuration 2

~~~~~c
#define FROM_FILE 0
~~~~~

After changing configuration the project must be rebuilt and run:

~~~~~sh
make clean all run
~~~~~

### Change Input Image

To change the input image the following two steps are required.

In the main.c file change the name of the input file:

~~~~c
char *Imagefile = "MyNewImage.ppm";
~~~~

So far only images of 324x244 size are supported.

## Citations

1. Dalal, Triggs, "Histograms of Oriented Gradients for Human Detection",  [Link](https://www.google.fr/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0ahUKEwjY4JWW35jXAhVEOMAKHc7tDbkQFggqMAA&url=https%3A%2F%2Fhal.inria.fr%2Finria-00548512%2Fdocument%2F&usg=AOvVaw1O2xQKr0vIq4SsT-R30rK4)
2. Y. Freund, R.E. Schapire, "A Decision-Theoretic Generalization of On-Line Learning and an Application to Boosting",  [Link](https://www.google.fr/url?sa=t&rct=j&q=&esrc=s&source=web&cd=1&ved=0ahUKEwi8q7fI4ZjXAhXFXRoKHUYEDLUQFggqMAA&url=http%3A%2F%2Fwww.face-rec.org%2Falgorithms%2FBoosting-Ensemble%2Fdecision-theoretic_generalization.pdf&usg=AOvVaw0L8cuezqaEAoVdWJhXaxho)
