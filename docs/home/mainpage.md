
# Introduction of the GAP8 IoT application processor

GAP8 is a RISC-V and PULP (Parallel Ultra-Low-Power Processing Platform) open-source platform based IoT application processor. It enables cost-effective development, deployment and autonomous operation of intelligent devices that capture, analyze, classify and act on the fusion of rich data sources such as images, sounds or vibrations. In particular, GAP8 is uniquely optimized to execute a large spectrum of image and audio algorithms including convolutional neural network inference with extreme energy efficiency. This allows industrial and consumer product manufacturers to integrate artificial intelligence and advanced classification into new classes of wireless edge devices for IoT applications including image recognition, counting people and objects, machine health monitoring, home security, speech recognition, consumer robotics and smart toys.

## GAP8 Micro-architecture

GAP8's hierarchical, demand-driven architecture enables ultra-low-power operation by combining:

+ A series of highly autonomous smart I/O peripherals for connection to cameras, microphones and other capture and control devices.
+ A fabric controller core for control, communications and security functions.
+ A cluster of 8 cores with an architecture optimized for the execution of vectorized and parallelized algorithms combined with a specialized Convolutional Neutral Network accelerator (HWCE).

All cores and peripherals are power switchable and voltage and frequency adjustable on demand. DC/DC regulators and clock generators with ultra fast reconfiguration times are integrated. This allows GAP8 to adapt extremely quickly to the processing/energy requirements of a running application. All elements share access to a L2 memory area. The cluster cores and HWCE share access to a L1 memory area and instruction cache. Multiple DMA units allow autonomous, fast, low power transfers between memory areas. A memory protection unit is included to allow secured execution of applications on GAP8.

All 9 cores share the same extended RISC-V instruction set architecture. The I (integer), C (compressed instruction), M (Multiplication and division) extensions and a portion of the supervisor ISA subsets are supported. These are extended with specific instructions to optimize the algorithms that GAP8 is targeted at. These extensions include zero overhead hardware loops, pointer post/pre modified memory accesses, instructions mixing control flow with computation (min, max, etc), multiply/subtract and accumulate, vector operations, fixed point operations, bit manipulation and dot product. All of these instruction extensions are optimized by the compiler or can be used 'by hand'.

## GAP8 Features

+ 1+8 high performance extended RISC-V ISA based cores.
	+ 1 - A high performance micro-controller
	+ 8 - 8 cores that execute in parallel for compute intensive tasks
+ A hardware Convolution Engine (HWCE) for Convolutional Neural Networks based applications.
+ A level 2 Memory (512KB) for all the cores
+ A level 1 Memory (64 KB) shared by all the cores in Cluster
+ A level 1 memory (8  KB) owned by FC
+ A smart, lightweight and completely autonomous DMA (micro-DMA) capable of handling complex I/O schemes.
+ A multi-channel 1D/2D cluster-DMA controls the transactions between the L2 Memory and L1 Memory.
+ A rich set of peripheral interface
+ 2 programmable clocks
+ Memory Protection Unit

![GAP8 Layout](./images/GAP8Layout.png){ width=480px }

As shown in the GAP8 block diagram above, GAP8 has a rich set of I/O interfaces, which includes:

 Interface | Number | Description
--- | --- | ---
 LVDS      | 1 | A 128 Mb/s interface for RF
 ORCA	   | 1 | A low debit interface for RF.
 I2C	   | 2 | Standard I2C interfaces.
 I2S	   | 2 | Standard I2S interface for connecting digital audio devices.
 CPI 	   | 1 | A parallel interface for connecting camera
 HyperBus  | 1 | A high speed memory bus interface
 SPI-M     | 2 | A quad SPI-M and an additional SPI-M
 SPI-S     | 1 | A SPI Slave
 UART      | 1 | A standard UART interface
 GPIOs     | 32| General Purpose Input Output

Other peripherals:

 Peripherals | Number | Description
|---|:---:|---|
 RTC 		 | 1      | 32K real-time clock
 PWM 		 | 4      | PWMs, 12 Output channels


## The Fabric Controller and The Cluster

As mentioned in previous section, GAP8 has 8+1 high performance cores, which play two different roles. The cluster contains 8 cores that can execute in parallel, and provide high performance calculation for image processing, audio processing or signal modulation, etc. The single core, referred to as the “Fabric Controller” or "FC", is used as micro-controller. It is in charge of controlling all the operations of GAP8, like the micro-DMA to capture 1 image from the CPI interface, starting up the cluster and dispatching a job to it, etc. You can think of the cluster as a 'peripheral' of the FC.

Cores are identified using two identification numbers. The cluster ID which identifies the group of cores that the core belongs to and the core ID which identifies the core in that group. The IDs can be used to start a particular task on a core.

ENTITY | CLUSTER ID | CORE ID
|---|:---:|:---:|
CORE0  |    0x00    |   0x00
CORE1  |    0x00    |   0x01
CORE2  |    0x00    |   0x02
CORE3  |    0x00    |   0x03
CORE4  |    0x00    |   0x04
CORE5  |    0x00    |   0x05
CORE6  |    0x00    |   0x06
CORE7  |    0x00    |   0x07
FC     |    0x20    |   0x00

The table above shows:

 + Core 0-7: which have the same cluster ID, but have different core ID.
 + FC: which has the same core ID as the cluster core 0, but its cluster ID is equal to 0x20.

By default, the cluster is powered down and cannot be used. It must first be powered-up by the FC. Once the cluster is awake, its core 0 plays the role of "master". Core 0 is in charge of the following jobs:

 + The communication with the Fabric Controller. For example, getting a task from the FC, sending a data request to the FC, getting synchronized with the FC, etc.
 + Dispatching tasks/applications to other cores.

Before receiving a task from the core 0, the rest of the cores stay at a dispatch barrier which clock gates them (i.e. they are stopped and use only a low leakage current).

When a task/application is finished on the cluster, it should be shut down to save power. IT IS IMPORTANT TO MOVE DATA IN THE SHARED L1 MEMORY TO THE L2 MEMORY BEFORE YOU SHUTDOWN THE CLUSTER.

## Memory areas

There are 2 different levels of memory internal to GAP8. A larger level 2 area of 512KB which is accessible by all processors and DMA units and two smaller level 1 areas, one for the FC (16KB) and one shared by all the cluster cores (64KB). The shared level 1 memory is banked and cluster cores can usually access their bank in a single cycle. GAP8 can also access external memory areas over the HyperBus (Flash or RAM) or quad-SPI (Flash) peripherals. We refer to RAM accessed over the HyperBus interface as level 3 memory. Since the energy cost and performance cost of accessing external RAM over the HyperBus is very high compared to the internal memory generally this should be avoided as much as possible. Code is generally located in the L2 memory area. The instruction caches of the FC (4KB) and cluster (16KB) will automatically cache instructions as needed. The cluster instruction cache is shared between all the cores in the cluster. Generally the cluster cores will be executing the same area of code on different data so the shared cluster instruction cache exploits this to reduce memory accesses for loading instructions.

## Micro-DMA and cluster-DMA

To reduce power consumption GAP8 does not include data caches in its memory hierarchy. Instead GAP8 uses autonomous DMA units that can be used to transfer data to and from peripherals and in between internal memory areas.

Good management of memory is absolutely crucial to extracting the most energy efficiency from GAP8. GreenWaves supplies a tool, the GAP8 auto-tiler, which can significantly aid in managing memory transfers between the different memory areas.

The micro-DMA unit is used to transfer data to and from peripherals including level 3 memory. At the end of a transaction the FC can be woken up to queue a new task. To allow the micro-DMA to continue working at the end of a transaction up to 2 transfers can be queued for each peripheral. The micro-DMA schedules active transfers based on signals from the peripherals in a round-robin fashion. Generally the micro-DMA is not used directly by a programmer. It is used by the drivers for each of the peripherals.

As the micro-DMA, the cluster-DMA is a smart, lightweight and completely autonomous unit. It is used to transfer data between the L2 and L1 memory areas. It supports both 1D and 2D transfers and can queue up to 16 requests. The commands for the cluster-DMA unit are extremely short which minimizes SW overhead and avoids instruction cache pollution.
\newpage
# Introduction to the GAP SDK

The GAP8 SDK allows you to compile and execute applications on the GAP8 IoT Application Processor. This SDK is an extract of the necessary elements from the pulp-sdk (https://github.com/pulp-platform/pulp-sdk) produced by the PULP project, to provide a development environment for the GAP8 series processors.

We provide you with all the necessary tools and two different operating systems for GAP8:

*   Tools
    -   GAP8 RISCV GNU toolchain: a pre-compiled toolchain inherited from RISC V project with support for our extensions to the RISC-V Instruction Set Architecture.
    -   PLPBRIDGE: a tool from the PULP project which allows communication between your PC and a GAP8 processor. Using this tool you can:
        * Program / control GAP8
        * Debug your application using GDB
        * Program the GAPuino flash memory with applications

*   Operating Systems
    -   PULP OS - The open source embedded RTOS produced by the PULP project
    -   Arm® Mbed™ OS - Arm Mbed OS is an open source embedded operating system. GreenWaves Technologies has ported it to GAP8 and VEGA.
    -   FreeRTOS - FreeRTOS is an open source real time operating system. GreenWaves Technologies has ported it to GAP8
    -   PMSIS - PMSIS is an open-source system layer which any operating system can implement to provide a common API to applications. We currently provide it fro PULP OS and FreeRTOS, and it is used by our applications to be portable.

\newpage
# Installing the GAP SDK

### Ubuntu 18.04

These instructions were developed using a fresh Ubuntu 18.04 Bionic Beaver 64-Bit virtual machine from https://www.osboxes.org/ubuntu/#ubuntu-1804-info

The following packages needed to be installed:

~~~~~shell
sudo apt-get install -y build-essential git libftdi-dev libftdi1 doxygen python3-pip libsdl2-dev curl cmake libusb-1.0-0-dev scons gtkwave libsndfile1-dev rsync autoconf automake texinfo libtool pkg-config
~~~~~

The precompiled toolchain should be clone by using git lfs, this should be installed by using the following command:

~~~~~shell
curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash
sudo apt-get install git-lfs
git lfs install
~~~~~

## Configure USB Port for GAPuino Evaluation Board and Gapoc board

For the USB serial connection our GAPuino and Gapoc boards use an FDDI 2 port serial to USB controller. This needs to be set up (the driver is installed in the apt-get install step above).

Add your username to the dialout group:

~~~~~shell
sudo usermod -a -G dialout <username>
~~~~~

Logout from your session and login again. Now create a udev rule for gapuino ftdi2232 usb chip

~~~~~shell
touch 90-ftdi_gapuino.rules
echo 'ATTRS{idVendor}=="0403", ATTRS{idProduct}=="6010", MODE="0666", GROUP="dialout"'> 90-ftdi_gapuino.rules
echo 'ATTRS{idVendor}=="15ba", ATTRS{idProduct}=="002b", MODE="0666", GROUP="dialout"'>> 90-ftdi_gapuino.rules
sudo mv 90-ftdi_gapuino.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules && sudo udevadm trigger
~~~~~

If you are using a Virtual Machine make sure that you give control of the FTDI device to your virtual machine. Plug the GAPuino into your USB port and then allow the virtual machine to access it. For example, for VirtualBox go to Devices->USB and select the device.

Please also make sure that your Virtual Machine USB emulation matches your PC USB version. A mismatch causes the USB interface to be very slow.

The following instructions assume that you install the GAP SDK into your home directory. If you want to put it somewhere else then please modify them accordingly.

### Ubuntu 16.04

You can follow the steps for Ubuntu 18.04 except for the following instructions.

After yu have install the system packages with apt-get, also create this symbolic link:

~~~~~shell
sudo ln -s /usr/bin/libftdi-config /usr/bin/libftdi1-config
~~~~~

## Download and install the toolchain:

Now clone the GAP8 SDK and the GAP8/RISC-V toolchain:

~~~~~shell
git clone https://github.com/GreenWaves-Technologies/gap_sdk.git
git clone https://github.com/GreenWaves-Technologies/gap_riscv_toolchain_ubuntu_18.git
~~~~~

In case you use an old git version, you may need to use these commands instead:

~~~~~shell
git clone https://github.com/GreenWaves-Technologies/gap_sdk.git
git lfs clone https://github.com/GreenWaves-Technologies/gap_riscv_toolchain_ubuntu_18.git
~~~~~

Install the toolchain:

~~~~~shell
cd ~/gap_riscv_toolchain_ubuntu_18
./install.sh
~~~~~

Initialize and download all the sub projects of the SDK:

~~~~~shell
cd ~/gap_sdk
git submodule update --init --recursive
~~~~~

## Install Python requirements

Our modules requires a few additional Python packages that you can install with this command from Gap SDK root folder:

~~~~~shell
pip3 install -r requirements.txt
~~~~~

In order to use the Gap tools for neural networks (nntool), we strongly encourage to install the Anaconda distribution ( Python3 ). You can find more information here: https://www.anaconda.com/.

Note that this is needed only if you want to use nntool, you can skip this steap otherwise. Once Anaconda is installed, you need to activate it and install python modules for this tool with this command:

~~~~~shell
pip install -r tools/nntool/requirements.txt
pip install -r requirements.txt
~~~~~

## Configure the SDK:

You can either source sourceme.sh in the root sdk folder and then select the right board from the list, or directly source the board config.

If you directly source the board config, you need to source the appropriate config file for the board that you have. The SDK supports 2 boards (gapuino and gapoc) and each of them can use version 1 or version 2 of the GAP8 chip. Boards bought before 10/2019 contains GAP8 version 1 and use a USB B plug for JTAG while the ones bought after contains version 2 and use a USB micro B for JTAG.

Hereafter you can find a summary of the available boards and their configuration file.

Board     | Chip       | Config file
|:-------:|:----------:|:----------------------:|
Gapuino   | GAP8 v1    | configs/gapuino.sh
Gapuino   | GAP8 v2    | configs/gapuino_v2.sh
Gapoc     | GAP8 v1    | configs/gapoc_a.sh
Gapoc     | GAP8 v2    | configs/gapoc_a_v2.sh

Once the proper config file is sourced, you can proceed with the SDK build.

Note that after the SDK has been built, you can source another board config file to change the board configuration, in case you want to use a different board. In this case the SDK will have to be built again. As soon as the SDK has been built once for a board configuration, it does not need to be built again for this configuration, unless the SDK is cleaned.


## Build the SDK:

You can build the SDK with this command:

~~~~~shell
make all
~~~~~

GVSOC virtual platform will be installed during the previous build.

## Getting the Autotiler

The autotiler code generation library needs to be downloaded by this following commands:

~~~~~shell
cd ~/gap_sdk/
make autotiler
~~~~~

During the download, you will be asked several simple questions and then you will receive an email with an URL.
Please copy the url in the mail to the last question, then the download and installation would be started automatically.
If you have already answered these questions, this enquiry will be skipped.

For offering you the best support, please answer the questions.

## Getting nntool

You first need to install Anaconda and Python dependencies (see above).

Then execute:

~~~~~shell
cd ~/gap_sdk/
make nntool
~~~~~

## Getting OpenOCD

OpenOCD for Gap8 has been introduced. Plpbridge is still used by default to load binaries until the next SDK, in which OpenOCD will become the default bridge. There are a few applications which require OpenOCD, as they are using OpenOCD semi-hosting to transfer files with the workstation.

If you want to install it, execute this command from the gap SDK folder:

~~~~~shell
cd ~/gap_sdk/
make openocd
~~~~~

Applications using it are already configured in their Makefile to use it. If you also want to force it for other examples and applications, you have to define this environment variable:

~~~~~shell
export GAP_USE_OPENOCD=1
~~~~~

# Compiling, running and debugging programs

The following command configures the shell environment correctly for the GAP8 SDK. **It must be done for each terminal session**:

~~~~~shell
source ~/gap_sdk/configs/gapuino.sh
~~~~~

The configuration file you should sourced may be different, depending on the board you are using (see above).

Tip: You can add an "alias" command as following in your .bashrc file:

~~~~~shell
alias GAP_SDK='cd ~/gap_sdk && source sourceme.sh'
~~~~~

Typing GAP_SDK will now change to the gap_sdk directory and execute the source command.

Finally try a test project. First connect your GAPuino to your PCs USB port and then type:

~~~~~shell
cd ~/gap_sdk/examples/pmsis/helloworld
make clean all run
~~~~~

After the build you should see:
~~~~~
     *** PMSIS HelloWorld ***

     Entering main controller
     [32 0] Hello World!
     Cluster master core entry
     [0 7] Hello World!
     [0 0] Hello World!
     [0 4] Hello World!
     [0 5] Hello World!
     [0 3] Hello World!
     [0 1] Hello World!
     [0 2] Hello World!
     [0 6] Hello World!
     Cluster master core exit
     Test success !
     Detected end of application, exiting with status: 0
     Loop exited
     commands completed
~~~~~

## Debugging Programs

The JTAG bridge allows for printf's in your code to generate messages on your PC. GAP8 also has support for gdb server over JTAG for both pulp-os and mbed-os programs.

To enable gdbserver run mode execute the make target gdbserver:

~~~~~shell
make clean all gdbserver
~~~~~

The program will load and gdbserver will be started. Connect to the target with gdb from another shell (remember to source the sourceme.sh file in the root of the SDK in your new shell). Change directory to the directory containing your test program and then:

~~~~~shell
riscv32-unknown-elf-gdb BUILD/GAP8/GCC_RISCV/test
~~~~~

Once gdb has loaded connect to the gdbserver on the target.

~~~~~
(gdb) target remote localhost:1234
Remote debugging using localhost:1234
~~~~~

## Using the virtual platform

You can also run this example on the Gap virtual platform with this command:

~~~~~shell
make clean all run platform=gvsoc
~~~~~

You can also generate VCD traces to see more details about the execution:

~~~~~shell
make clean all run platform=gvsoc runner_args=--vcd
~~~~~

You should see a message from the platform telling how to open the profiler.

## Using the flasher (Hyperflash)

Add the following line into your application Makefile:

~~~~~shell
PLPBRIDGE_FLAGS += -f <file1> <file2> <file3> ......
~~~~~

Without any further options the files are flashed but your application is not executed.

If you want to execute your application after flashing use the following options:

If you want to boot your application from jtag:

~~~~~shell
PLPBRIDGE_FLAGS += -jtag
~~~~~

If you wan to boot your application from hyperflash:
~~~~~shell
PLPBRIDGE_FLAGS += -hyper
~~~~~

These two flags cannot be used at the same time.

If you choose to boot your application from HyperFlash, and you want to view the output of printf's in your code then you can use a terminal program, like "cutecom":

~~~~~shell
sudo apt-get install -y cutecom
cutecom&
~~~~~

Then please configure your terminal program to use /dev/ttyUSB1 with a 115200 baud rate, 8 data bits and 1 stop bit.

## Using the file IO

Through our new bridge tool (pulp-rt-bridge), you could load/store from/to in your PC.
For example, if you want to use an image locally, but run the algorithm with this image on GAP8. You could use this functionality to do it.

To trigger this function in bridge, you need to add this flag in your Makefile:

~~~~~shell
PLPBRIDGE_FLAGS += -fileIO <option>
~~~~~

The option should be a number, which would be used for let the bridge wait for \<option\> seconds. Because of the synchronization between this bridge and our debug bridge, we need to wait until the program has been load and executed on GAP8. If you don't put a number, the default value is 2 (seconds).

Otherwise, you could run this service manually:

In another terminal:

Configure the environment, and then:

~~~~~shell
plpbridge-rt --binary=<path of your binary file> --chip=gap
~~~~~

Please read the example for more information: examples/pulp-examples/kernel/bridge

For your information, this new bridge is quite slow, please be patient when you are using it. We will resolve this problem as soon as possible.

## Documentation

Build the documentation:

~~~~~shell
cd gap_sdk
make docs
~~~~~

If you haven't download and install the autotiler, you will probably have some warnings when you build the docs.
All the documentations are available on our website: https://greenwaves-technologies.com/en/sdk/

You can read the documentation by opening gap_doc.html in the docs folder in your browser:

~~~~~shell
firefox docs/gap_doc.html
~~~~~

If you would like PDF versions of the reference manuals you can do:

~~~~~shell
cd docs
make pdf
~~~~~

## Upgrading/Downgrading the SDK

If you want to upgrade/downgrade your SDK to a new/old version:

~~~~~shell
cd gap_sdk
git checkout master && git pull
git checkout <release tag name>
git submodule update --init --recursive
make clean all
~~~~~

Please check our release tags here to ensure the version:
https://github.com/GreenWaves-Technologies/gap_sdk/releases



## What is in the gap8_sdk folder?

This folder contains all the files of the GAP8 SDK, the following table illustrate all the key files and folders:

Name | Descriptions
------------ |---------------------------------------------------------------------------
docs | Runtime API, auto-tiler and example application documentation
pulp-os | a simple, PULP Runtime based, open source operating system for GAP8.
mbed-os | ARM Mbed OS based operating system
sourceme.sh | A script for configuring the GAP SDK environment
examples | Examples of runtime API usage
tools | All the tools necessary for supporting the GAP8 usage

