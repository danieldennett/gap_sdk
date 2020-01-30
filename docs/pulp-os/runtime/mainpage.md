
# Introduction to the PULP OS

# How to start compiling and running applications

## Configure the GAP8 Development Environment

The following command configures the shell environment correctly for the GAP8 SDK. It must be done for each terminal session:

~~~~~shell
source ~/gap_sdk/sourceme.sh
~~~~~

Tip: You can add an "alias" command as following in your .bashrc file:

~~~~~shell
alias GAP_SDK='cd ~/gap_sdk && source sourceme.sh'
~~~~~

Typing GAP_SDK will now change to the gap_sdk directory and execute the source command.

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

## How to compile and run the “Hello World” example on GAPUINO Devlopment Board

The hello world example gets each core to print out a hello message. We will go through this example in detail so that you can learn how to schedule tasks on the cluster.

Like other examples and applications, this example is located in the "examples" folder:

~~~~~shell
	cd examples/pulp-examples/hello_world
~~~~~

Then execute this command for compiling and running the example on GAPUINO:

~~~~~shell
	make clean all run
~~~~~

You can also compile and execute the example separately.

~~~~~shell
	make clean all
	make run
~~~~~

In the result, you will see the Hello from all the cores in FC and in Cluster, with their Cluster IDs and Core IDs.

All the examples can be built and run with the same procedure.

### Explanation of hello world example

While reviewing this please refer to the runtime API reference.

The hello world `main()` function is executed on the FC. The runtime uses a simple yet powerful asynchronous event system to synchronize activity between tasks on the FC and the cluster. The first thing the example does is to get a pointer to the default scheduler created when the runtime starts.

~~~~~c
  rt_event_sched_t * p_sched = rt_event_internal_sched();
~~~~~

You can create more schedulers if your application requires more complex hierarchical scheduling structures.

Each scheduler can queue a certain amount of events. To save memory a fixed amount of space is reserved for queued events. To make sure that we have an adequate amount of queued event space we next add room for 4 queued events.

~~~~~c
  if (rt_event_alloc(p_sched, 4)) return -1;
~~~~~

Now we allocate an event and bind it to a function that will be called when it completes, `end_of_call()`. We will use this event a little bit later. Note that we bind the CID constant as an argument for the end_of_call function.

~~~~~c
  rt_event_t *p_event = rt_event_get(p_sched, end_of_call, (void *) CID)
~~~~~

Since our hello world example is going to dispatch a task onto the cluster we first need to switch it on. The `rt_cluster_mount()` call will block until the cluster is switched on when used with a NULL 4th parameter. Many of the asynchronous event based APIs can take NULL as the event argument which transforms them into blocking calls. Whenever blocked the core is clock gated so it consumes minimal power.

~~~~~c
  rt_cluster_mount(MOUNT, CID, 0, NULL);
~~~~~

Each cluster task is a function called on one of the cluster cores. A cluster function first executes on cluster core 0 which acts as a dispatcher to the other cores. Each cluster core function requires an allocated portion of memory to use as a call stack. We may want to allocate slightly more stack to core 0 to cover the extra tasks associated with dispatching to and collecting results from the other cores. In the case of the hello world example there is not much to do so we allocate the same amount of stack for all the cores.

We allocate the stacks in the cluster level 1 shared memory (`RT_ALLOC_CL_DATA`) and we get the number of cores in the cluster using `rt_nb_pe()`.

~~~~~c
  void *stacks = rt_alloc(RT_ALLOC_CL_DATA, STACK_SIZE*rt_nb_pe());
  if (stacks == NULL) return -1;
~~~~~

Now we can actually call the function that we want to execute on the cluster using the `rt_cluster_call()` function. This call will start the function `cluster_entry()` on core 0 of the cluster and will trigger the event allocated in `p_event` when the function `cluster_entry()` exits on core 0. Most APIs that use events can take a NULL pointer for the event parameter. In this case the function will block until completed.

We pass the pointer to the stack area that we have allocated and indicate the size of the stack for core 0 and the size for the other cores and the number of cores that we want to use for this function (all of them).

Just after calling the cluster we trigger events on our scheduler using `rt_event_execute()`. If no events are ready then this call will block and the core will sleep until one is.

~~~~~c
  rt_cluster_call(NULL, CID, cluster_entry, NULL, stacks,
  	STACK_SIZE, STACK_SIZE, rt_nb_pe(),
    p_event);
  rt_event_execute(p_sched, 1);
~~~~~

The function `cluster_entry()` now executes on core 0. We print a message so that you can see the order of execution and start the `hello()` function on all the cores.

~~~~~c
  rt_team_fork(8, hello, (void *)0x0);
~~~~~

The `rt_team_fork()` function will block on core 0 until all the cores have exited the `hello()` function. The `hello()` function is now executed on all cores **including core 0**.

Each core in the cluster prints our its core and cluster IDs.

Control now returns to after the `rt_team_fork()` call which then exits triggering the `p_event` event on the FC which calls the function `end_of_call()`. This function executes on the FC and prints out the FC cluster and core IDs. The `rt_event_execute()` then returns since an event has been serviced. Often we would enclose the call to this function in a loop since we would want to continue waiting for and consuming events but in this simple case we then just turn the cluster off and return terminating the program.

Here is an example of the output:

~~~~~sh
Entering main controller
Entering cluster on core 0
[clusterID: 0x 0] Hello from core 1
[clusterID: 0x 0] Hello from core 0
[clusterID: 0x 0] Hello from core 2
[clusterID: 0x 0] Hello from core 3
[clusterID: 0x 0] Hello from core 4
[clusterID: 0x 0] Hello from core 5
[clusterID: 0x 0] Hello from core 6
[clusterID: 0x 0] Hello from core 7
Leaving cluster on core 0
[clusterID: 0x20] Hello from core 0
Leaving main controller
~~~~~

Notice the control flow, main on FC, then cluster_entry, then hello on all the cores, then return to cluster_entry, then end_of_call and then back to main.

## How to console through uart

In pulp os, the printf will go through JTAG by default. If you want to console via uart, please define parameter "__rt_iodev" in your program as below:

~~~~c
unsigned int __rt_iodev=RT_IODEV_UART;
~~~~

This global variable will trigger the console on serial port.


## How to allocate memory

As mentioned before, there are three areas of memory in GAP8, the FC L1 memory, the shared L1 cluster memory and the L2 memory. The L2 memory is the home for all code and data. The L1 areas are the location for temporary blocks of data that requires very fast access from the associated core. The shared L1 cluster memory is only accessible when the cluster has been mounted.

All memory allocations are executed by the FC. As opposed to a classic malloc function the amount of memory allocated must be supplied both when allocated and when freed. This reduces the meta data overhead associated with the normal malloc/free logic.

The most portable memory allocator is the one provided by rt_alloc and rt_free.

~~~~~c
void* rt_alloc(rt_alloc_e flags, int size)
void rt_free(rt_alloc_e flags, void * chunk, int size)
~~~~~

This takes a first parameter that indicates what the memory is intended to contain. In GAP8 the following areas are used:

| L2 | FC L1 | Cluster L1 |
|:--------------------:|:----------------:|:----------------:|
| RT_ALLOC_FC_CODE | RT_ALLOC_FC_DATA | RT_ALLOC_CL_DATA |
| RT_ALLOC_FC_RET_DATA |  |  |
| RT_ALLOC_CL_CODE |  |  |
| RT_ALLOC_L2_CL_DATA |  |  |
| RT_ALLOC_PERIPH |  |  |

## How to use the event and thread scheduler APIs

The asynchronous interactions between the fabric controller, the cluster and the peripherals are managed with events on the fabric controller side.

An event is a function callback which can be pushed to an event scheduler, for a deferred execution. All events in the same scheduler are executed in-order, in a FIFO manner.

In order to manage several levels of event priorities, there may be several event schedulers at the same time. The application is entered with one event scheduler already created in the runtime, which is the one which can be used as the default scheduler. However the application can then create multiple event schedulers in order to build a more complex multi-priority scenario.

Event schedulers are only executed when specific actions are executed by the application. They can be either invoked explicitly by calling rt_event_execute or implicitly when for example no thread is active and the application is blocked waiting for something.

The pool of events which can be pushed to schedulers must be explicitly managed by the application. There is by default no event allocated. They must be first allocated before they can be pushed to schedulers. This allows the application to decide the optimal number of events that it needs depending on the various actions which can occur.

When a scheduler is invoked, it executes all its pending events, unless the current thread is preempted to schedule another thread. To execute an event, it removes it from the queue and executes the associated callback with its argument, until the callback returns. This scheduler can not execute any other event until the callback has returned.

Each thread is assigned an event scheduler. By default, when a thread is created, it is assigned the internal event scheduler. Another scheduler can then be assigned by the application. This scheduler is the one implicitly invoked when the thread is calling a blocking operation such as waiting for an event.

The runtime on the fabric controller is always running a thread scheduler. This scheduler is priority-based and preemptive. Only the ready threads of the highest priority are scheduled following a round-robin policy. It is intended that threads be preempted at a fixed frequency in order to let other threads of the same priority run however this feature is not present in the runtime in the first release of the SDK.

If the application only uses one thread and uses events to schedule tasks (cooperative multi-tasking) it will always run in the main thread. If the application needs to use preemptive threading then it can use the thread API to create new threads. You can find an example of threads and events in the `examples/threads_events` directory.

## How to use the DMA

The DMA unit that is directly used by the programmer is the cluster-DMA. The micro-DMA is used by peripheral drivers.

The cluster-DMA is used to move data between its 'home location' in L2 memory and the shared L1 cluster memory.

DMA transactions are executed in parallel with code executing on any of the cores. Cluster DMA unit transactions can only be carried out on the cluster. The API should not be used on the FC.

### Transfer identifier synchronization

Generally core 0 will be used to set up DMA transfers and then dispatch tasks to the other cores on completion. Each transfer is assigned a transaction identifier. Optionally a new transfer can be merged with a previous one i.e. they shared the same transaction identifier. The `rt_dma_wait()` function waits until a specific transaction ID has completed.

### 1D/2D copies

The DMA queuing function has a 1D and 2D variant. A 1D DMA operation is a classic linear copy. The 2D functions allow a 2D tile where lines have spaces between them to be copied in a single DMA transfer to a continuous memory space.

### Note

Take care to declare `rt_dma_copy_t copy` as a variable and use its address in functions requesting `rt_dma_copy_t *copy`.

By example, you can find in `app_release/examples/dma/test.c`:
~~~~~c
rt_dma_copy_t dmaCp;
rt_dma_memcpy((int)l2_buff0, (int)cluster_l1_buff, BUFF_SIZE, RT_DMA_DIR_EXT2LOC, 0, &dmaCp);

// Wait for the operation to finish
rt_dma_wait(&dmaCp);
~~~~~


## How to synchronize the cluster cores

Since the cluster is viewed as a peripheral to the fabric controller the synchronization model between the two is simple. You start a task on the cluster and it reports a result with an event.

For some algorithms on the cluster you will launch a task on all the cores and each core will carry out the task on its data but may need to synchronize with the other cores at some intermediate step in the operation.

There is a simple scheme to do this using the `rt_team_barrier()` function. If a cluster core calls this function it will block (clock gated and therefore in a low power state) until all cluster cores in the team have executed `rt_team_barrier()`. The *team* is composed of all the cores that were involved in the last `rt_team_fork()`.

You can see this mechanism in operation in the example `examples/cluster_sync`.

## Cluster HW Convolution Engine

The GAP8 cluster includes a hardware convolution engine which accelerates convolution operations used in CNN. This is not supported in the GVSOC simulator so is not documented in this SDK release.

## How to measure performance

You can measure performance in two ways, performance counters for cycle level performance and the 32kHz clock for overall execution time.

### Performance counters

The cluster and the FC have a range of performance counters that can be used to measure cycle accurate performance on the GVSOC simulator and on the GAP8 hardware.

The following counters are supported on GAP8:

~~~~~c
  RT_PERF_CYCLES        /* Total number of cycles (also includes the cycles where the core is sleeping). */
  RT_PERF_ACTIVE_CYCLES /* Counts the number of cycles the core was active (not sleeping). */
  RT_PERF_INSTR         /* Counts the number of instructions executed. */
  RT_PERF_LD_STALL      /* Number of load data hazards. */
  RT_PERF_JR_STALL      /* Number of jump register data hazards. */
  RT_PERF_IMISS         /* Cycles waiting for instruction fetches, i.e. number of instructions wasted due to non-ideal caching. */
  RT_PERF_LD            /* Number of data memory loads executed. Misaligned accesses are counted twice. */
  RT_PERF_ST            /* Number of data memory stores executed. Misaligned accesses are counted twice. */
  RT_PERF_JUMP          /* Number of unconditional jumps (j, jal, jr, jalr). */
  RT_PERF_BRANCH        /* Number of branches. Counts both taken and not taken branches. */
  RT_PERF_BTAKEN        /* Number of taken branches. */
  RT_PERF_RVC           /* Number of compressed instructions executed. */
  RT_PERF_LD_EXT        /* Number of memory loads to EXT executed. Misaligned accesses are counted twice. Every non-TCDM access is considered external (cluster only). */
  RT_PERF_ST_EXT        /* Number of memory stores to EXT executed. Misaligned accesses are counted twice. Every non-TCDM access is considered external (cluster only). */
  RT_PERF_LD_EXT_CYC    /* Cycles used for memory loads to EXT. Every non-TCDM access is considered external (cluster only). */
  RT_PERF_ST_EXT_CYC    /* Cycles used for memory stores to EXT. Every non-TCDM access is considered external (cluster only). */
  RT_PERF_TCDM_CONT     /* Cycles wasted due to TCDM/log-interconnect contention (cluster only). */
  RT_PERF_CSR_HAZARD    /* Cycles wasted due to CSR access. */
~~~~~

On GAP8 you can monitor `RT_PERF_CYCLES` and one other event simultaneously on FC and cluster. On the GVSOC simulator it is possible to measure all events simultaneously.

To use the performance counters first initialize an rt_perf_t structure:

~~~~~c
  rt_perf_t *perf;
  perf = rt_alloc(RT_ALLOC_L2_CL_DATA, sizeof(rt_perf_t));
  rt_perf_init(perf);
~~~~~

Now set the events that you want to monitor:

~~~~~c
  rt_perf_conf(perf, (1<<RT_PERF_CYCLES) | (1<<RT_PERF_INSTR));
~~~~~

You can now start and stop the counters around any code that you want to measure.

~~~~~c
  rt_perf_start(perf);
  /* code to monitor */
  rt_perf_stop(perf);
~~~~~

The counters are cumulative so you can do this multiple times. Whenever you want to get a measurement you call `rt_perf_save()` and then `rt_perf_get()` to get individual values:

~~~~~c
    rt_perf_save(perf);
    int instr = rt_perf_get(perf, RT_PERF_INSTR);
    int cycles = rt_perf_get(perf, RT_PERF_CYCLES);
~~~~~

You can reset the counters at any time with:

~~~~~c
  rt_perf_reset(perf);
~~~~~

Remember that you need to call these functions on the core that you want to monitor.

### 32kHz timer

The 32kHz timer allows you to measure overall execution time. The timer has a resolution of approximately 31us. You can get the value of the 32kHz timer using the function `rt_time_get_us()`.

## Using peripherals

### HyperBus

The HyperBus interface allows external flash and RAM to be connected to GAP. It draws upon the legacy features of both parallel and serial interface memories, while enhancing system performance, ease of design, and system cost reduction. For GAP8 HyperBus, the read throughput is up to `50 MB/s` (Max Frequency = 25 MHz).

![Hyperbus Diagram [http://www.cypress.com/products/hyperbus-memory]](images/hyperbus_diagram.png){ width=300px }

#### Connection

For GAP8 HyperBus usage, we recommend implementing HyperFlash and HyperRAM together. For example : GAP8 is connected with S71KS256SC0 [1.8 V, 512 Mbit HyperFlash and 64 Mbit HyperRAM]

![HyperBus Connection](images/hyperbus_connection.png){ width=300px }

#### Micro-DMA HyperBus Driver

In GAP8 all peripheral access, including HyperBus, is controlled by the micro-DMA unit.

The HyperBus micro-DMA implementation has two channels which both support Flash or RAM. It is normal to configure the two channels as one for Flash and the other for RAM. For example, one for S26KS512S [1.8 V, 512 Mbit (64 Mbyte), HyperFlash™] and the other for S27KS0641 [1.8 V, 128 Mbit (16 Mbyte), HyperRAM™ Self-Refresh DRAM].

In the HyperBus driver, the default channel allocation is channel 0 for HyperRAM and channel 1 for HyperFlash however users can adjust the channel allocation to match their requirements. The driver is responsible for configuring the HyperBus and reading and writing in HyperFlash and HyperRAM.
It supports 4 or 2 byte aligned and unaligned access in the following way:

~~~~c
if (READ) {
   if (BYTES_LENGTH % 4 == 0) {
      // Read BYTES_LENGTH
   } else {
      // Read the last 4 bytes for remainder
      // Read the rest (BYTES_LENGTH - BYTES_LENGTH % 4) bytes
   }
}

if (WRITE) {
   if (BYTES_LENGTH % 4 == 0) {
      // Write BYTES_LENGTH
   } else {
      // Read the last 4 bytes for remainder, and Renew the data
      // Write back the last 4 bytes
      // Write the rest (BYTES_LENGTH - BYTES_LENGTH % 4) bytes
   }
}
~~~~


#### Examples

Here are two HyperBus examples, one for HyperRAM and the other for HyperFlash. You can find the examples in the SDK/examples/peripherals/hyper folder.

##### HyperRAM example

To indicate to GVSOC that it needs to simulate the HyperRAM, the following line has been added to the Makefile:

~~~~~sh
  override CONFIG_OPT += hyperram
~~~~~

- The HyperRam example has `read` and `write` functions
    + Firstly, it opens and configures the HyperRAM device. The function `rt_hyperram_open` opens the correct RAM according to the given device name.
    + Then the function `rt_hyperram_write` writes 1024 bytes from address 0 in HyperRAM. When it finishes, it puts the function `end_of_tx` in event queue.
    + Then the function `rt_hyperram_write` writes 1024 bytes from address 1024 in HyperRAM. When it finishes, it puts the function `end_of_tx` in event queue.
    + When the two write requests end and the two `end_of_tx` functions are executed, two read requests read from address 0 and address 1024 separately
      and will put two functions `end_of_rx` in event queue.
    + When the two read requests end and the two `end_of_rx` functions are executed then the program exits.

##### HyperFlash example

To indicate to GVSOC that it needs to simulate the HyperFlash the following line has been added to the Makefile:

~~~~~sh
override CONFIG_OPT += hyperflash
~~~~~

- The HyperFlash example only has a `read` function.
    + Firstly, it opens and configures the HyperFlash device. The `rt_flash_open` function opens the correct Flash according to device name.
    + Then the function `rt_flash_read` reads 128 bytes from address 0 in HyperFlash, when it finishes, it puts the function `end_of_rx` in event queue.
    + Finally the function `rt_flash_read` reads 128 bytes from address 128 in HyperFlash, when it finishes, it puts the function `end_of_rx` in event queue.
    + When the two read requests end and the two `end_of_rx` functions are executed, then the program exits.

#### File system

You may want to locate some external files in the HyperFlash simulated by GVSOC so you can use them in a test.

Since the HyperFlash is used by the simulator for the boot code we have provided a read only file system that can be initialized by GVSOC to allow the added files to be read.

The example in the SDK/examples/file_system folder shows how to use the mini file system to locate and read two simulated files.

To use the file system with GVSOC, you need to put this in your makefile:

~~~~~c
	override CONFIG_OPT += hyperflash hyperflash/fs/files=<path to the file you want to load into the file system>
~~~~~

This will tell GVSOC to load this file into the file system, which can then be mounted, opened, seeked and read by using the file system API.

As the example, you need to use these API as below:

1. Define the file system configuration structure and initialize it with default values

~~~~~c
  		rt_fs_conf_t conf;
  		rt_fs_conf_init(&conf);
~~~~~

2. Mount a file-system from hyper_flash.

~~~~~c
  		rt_fs_t *fs = rt_fs_mount("hyperflash", &conf, event);
~~~~~

3. Open one file in this file system

~~~~~c
  		file = rt_fs_open(fs, "file_name", flag, event);
~~~~~

4. Read data from this file

~~~~~c
		int size = rt_fs_read(file, &buff_of_recv, SIZE, event);
~~~~~

5. Close the file when it is not needed anymore

~~~~~c
		rt_fs_close (file, event);
~~~~~

6. Unmount the opened file-system

~~~~~c
		rt_fs_unmount(fs, event);
~~~~~

### Sound interface (I2S)

There is an example of using the I2S interface to capture a sound in the directory: SDK/examples/i2s.
The example illustrates how to use the I2S driver to connect to digital microphones.

GAP8 embeds 2 I2S modules. Each modules can be connected to 2 microphones so 4 digital microphone can be connected to GAP8.
Each modules supports PDM or PCM transfers, sampling frequency initialization, and decimation specification in case of PDM transfer

#### Driver Description

The following structure is used to configure each I2S module:

~~~~~c
typedef struct rt_i2s_conf_s {
  unsigned short frequency;
  unsigned short  decimation_log2;
  unsigned char  pdm;
  unsigned char  dual;
} rt_i2s_conf_t;
~~~~~

By default, this structure is initialized with :

~~~~~c
  conf->frequency = 44100;
  conf->decimation_log2 = 8;
  conf->pdm = 1;
  conf->dual = 0;
~~~~~

The default settings are:

+ 44.1kHz sampling frequency is selected
+ PDM mode is selected
+ 64 decimation is selected
+ Single digital microphone is connected

Initialize an I2S configuration with default values.

~~~~~c
void rt_i2s_conf_init(rt_i2s_conf_t *conf);
~~~~~

Open an I2S device.

~~~~~c
rt_i2s_t *rt_i2s_open(char *dev_name, rt_i2s_conf_t *conf, rt_event_t *event);
~~~~~

Capture a sequence of samples.

~~~~~c
static inline void rt_i2s_capture(rt_i2s_t *handle, void *buffer, size_t size, rt_event_t *event);
~~~~~

Close an opened I2S device.

~~~~~c
void rt_i2s_close(rt_i2s_t *handle, rt_event_t *event);
~~~~~

#### Example description

The example initializes the configuration structure, opens the microphone, and starts the transfer. The transfer is double buffered, optimizing data transfer.

#### Test description
In the folder of examples ($SDK/examples/peripherals/I2S), we provides you a test of a pdm microphones. This test shows you how to use the I2S interface to capture sounds. Thanks to the software model on the platform GVSOC, we can run the microphones without HW.

In this example, a 500Hz sin wave is genarated with sox. The sampling frequency is 44.1kHz. The format of this wave is the standard ".wav".
The model of gvsoc will read the stim.wav, and send it to GAP8 via I2S interface. After 1024 samples are received the test will apply a check for verifing the reception is correct.

#### Befor starting
Be sure that sox and libsndfile are installed :

~~~~~c
    sudo apt-get install sox libsndfile1-dev
~~~~~

#### Test on GVSOC
For testing this example or your application with one microphone on GVSOC platform, please:

1. Prepare the "stim.wav" file with the wanted sample frequency:

~~~~~c
    make gen all SAMPLING_FREQ=44100
~~~~~

2. Run the test

~~~~~c
    make run SAMPLING_FREQ=44100 FORMAT=wav > samples_wav.txt
~~~~~

At the end of the test, a file storing the received samples is created : samples_waves.hex

3. Check that the received samples are OK
Use this file to check that the received frequency is correct

~~~~~c
    extract_freq.py --samples=samples_waves.txt --frequency=500 --error=0.1 --sample-freq=44100
~~~~~

#### Makefile useful tips for I2S

You can specify the following options in the CONFIG_OPT variable in the Makefile:

+ its frequency (in Hz) : --freq=
+ if the transfer is PDM (is PCM if not specifed) : --pdm
+ if 2 micro are connected to the I2S interface : --lr
+ the stimulus file for the single or the left microphone : stim_left=
+ the stimulus file for the right microphone in the case that 2 microphones are connected

For example :

~~~~~c
override CONFIG_OPT += microphone system/microphone/stim_left=$(CURDIR)/stim.txt
~~~~~

### Camera Interface (CPI)

GAP8 provides a 8 bits parallel interface to connect with an imager, such as HIMAX HM01B0 and Omnivision OV7670. Drivers for these two image sensors have already been implemented in the runtime. There is a high level API in the runtime which abstracts the interface to these cameras.

To use the camera API, the following sequence should be respected:

1. Initialize the configuration structure
2. Open the camera

~~~~~c
	rt_camera_t * 	rt_camera_open (char *dev_name, rt_cam_conf_t *conf, rt_event_t *event)
~~~~~

3. Configure the camera by using the control function (optional)

~~~~~c
	static void  	rt_cam_control (rt_camera_t *handle, rt_cam_cmd_e cmd, void *arg)
~~~~~

4. Capture the image

~~~~~c
    static void 	rt_camera_capture (rt_camera_t *handle, void *buffer, size_t size, rt_event_t *event)
~~~~~

5. Start/Stop the camera by using the control function

6. Close the camera if we don't need it any more.

~~~~~c
	static void 	rt_camera_close (rt_camera_t *handle, rt_event_t *event)
~~~~~

#### Camera Example

In the folder of examples (SDK/examples/peripherals/camera_HIMAX), there is an example of using the camera API to capture an image from a Himax imager. Thanks to the emulation in the GVSOC simulator, we can run the camera without hardware.

In this example, the GVSOC reads the file imgTest0.pgm, and sends it to GAP8 via the CPI interface. Once the transfer is finished, the test will apply a check for verifying that the received image is correct. This example takes 2 pictures with different modes:

 * Full image mode: Taking a full picture with size (324 * 244) by default
 * Sliced image mode: Taking a picture in an interest area only. The interest area is defined by your configuration:

~~~~~c
		   // Activate the slice mode by configuring the camera with CMD_SLICE and the structure slicer:
		   // The structure of slicer contains two corner coordinates:
		   // ll - lower left corner
		   // ur - upper right corner
		   rt_img_slice_t slicer;
		   slicer.slice_ll.x = LL_X;
		   slicer.slice_ll.y = LL_Y;
		   slicer.slice_ur.x = UR_X;
		   slicer.slice_ur.y = UR_Y;
		   rt_cam_control(camera1, CMD_SLICE, &slicer);
~~~~~

#### Test on GVSOC

To test this example or your application with camera on GVSOC platform:

1. Prepare one or several pgm images:
Please note, the HIMAX model only supports QVGA resolution (324 * 244) as the HIMAX image sensor.
This means, the model will always send you 324 * 244 bytes of image whether the pgm image is larger or smaller.

 - If the image is larger, the pixels out of the range (W > 324, H > 244) will be dropped

 - If the image is smaller, the rest of the pixels will be filled by 0xFF.
 	For example, we use an image pgm with size (3 * 3), the image sent by model will be like this:

      						W=324
      		0x55, 0x55, 0x55, 0xFF, 0xFF, 0xFF, ... 0xFF
      		0x55, 0x55, 0x55, 0xFF, 0xFF, 0xFF, ... 0xFF
      		0x55, 0x55, 0x55, 0xFF, 0xFF, 0xFF, ... 0xFF   H=244
      		0xFF, 0xFF, 0xFF, ....                  0xFF
      		............................................
      		0xFF....................................0xFF

    If you want to use more than 1 image in your application, please name all your image like this: imgTest0.pgm, imgTest1.pgm, imgTest2.pgm ...
Then the model will start from the imgTest0.pgm until the last image, and restart from imgTest0.img.

2. Put this in your Makefile:

~~~~~makefile
  	override CONFIG_OPT += camera=himax system/camera/image-stream=$(CURDIR)/imgTest%d.pgm
~~~~~


## How to create your first application

### Source of application

Firstly, please include the runtime header file "rt/rt_api.h", which include sall the libraries in the runtime.

~~~~~c
	#include "rt/rt_api.h"
~~~~~

Secondly, please refer to the reference documentation for the GAP8 runtime API.

### Makefile

The Makefile calls the compiler to compile the application with the arguments defined in the Makefile, like how to optimize, which libraries need to be linked, etc.

The name of the final binary can be specified with PULP_APP variable :

~~~~~c
PULP_APP = test
~~~~~

The source files that must be compiled to build the binaries can be specified with the PULP_APP_FC_SRCS variable :

~~~~~c
PULP_APP_FC_SRCS = test.c
~~~~~

The flags (optimize, debug, defines, include paths for examples) that needed to be send to the gcc compiler can be specified with the PULP_CFLAGS variables :

~~~~~c
PULP_CFLAGS = -O3 -g -DDUMP_LAYER -Iinc_gen
~~~~~

The flags (library paths, libraries for example) that needed to be send to the gcc linker can be specified with the PULP_CFLAGS variables :

~~~~~c
PULP_LDFLAGS += -L$(PULP_SDK_HOME)/install/ws/lib -ldebugbridge-dpi
~~~~~

The flabs that need to be passed to the gvsoc simulator can be specified with the CONFIG_OPT variable. Some uses of this variable can be seen in the hyperram, cpi, i2s or file system examples :

~~~~~c
override CONFIG_OPT += hyperram
override CONFIG_OPT += hyperflash
override CONFIG_OPT += hyperflash hyperflash/fs/files=$(CURDIR)/test.c hyperflash/fs/files=$(CURDIR)/hello.txt hyperflash/fs/files$(CURDIR)/hello2.txt
override CONFIG_OPT += camera=himax system/camera/image-stream=$(CURDIR)/imgTest.pgm
override CONFIG_OPT += camera=ov7670 system/camera/image=$(CURDIR)/frame0.img

~~~~~

At the end of your Makefile, please don't forget to add the following include:

~~~~~makefile
	include $(PULP_SDK_HOME)/install/rules/pulp_rt.mk
~~~~~

# Example Applications

Several larger examples are available in the applications directory. These are compiled and run as the other application.

In the first version of the SDK we provide 4 complete applications that highlight examples of various types of image processing:

- applications/cannyEdgeDetection : Shows a vectorized and parallelized version of a classic image edge detection algorithm. This example uses the simulation of the Camera Parallel Interface to read in an image and carry out edge detection.
- applications/cifar10 : Implements a CNN that can find the class (dog, cats, etc) of an object in an image stream.
- applications/mnist : Implements a CNN that can find a digit (0-9) in an image stream.
- applications/countingPeople : Implements an algorithm based on HoG and boosting to count the number of people in an image. This is for people standing in front of the camera when the image is taken.

\newpage

# Advanced topics


## Using vectors

### Introduction

This section is derived from the GCC documentation.

The first step in using these extensions is to provide the necessary data types. This should be done using an appropriate typedef:

~~~~c
typedef short int           v2s __attribute__ ((vector_size (4))); // A vector of 2 shorts
typedef signed char         v4s __attribute__ ((vector_size (4))); // A vector of 4 chars
typedef unsigned short int  v2u __attribute__ ((vector_size (4))); // A vector of 2 unsigned shorts
typedef unsigned char       v4u __attribute__ ((vector_size (4))); // A vector of 4 unsigned chars
~~~~

The `int` type specifies the base type, while the attribute specifies the vector size for the variable, measured in bytes.  For example, the declaration above causes the compiler to set the mode for the `v4s` type to be 4 bytes wide and divided into `char` sized units.  For a 32-bit `int` this means a vector of 4 units of 1 byte, and the corresponding mode of `foo` is V4QI in GCC internal parlance.

The `vector_size` attribute is only applicable to integral and float scalars, although arrays, pointers, and function return values are allowed in conjunction with this construct. Only sizes that are a power of two are currently allowed.

All the basic integer types can be used as base types, both as signed and as unsigned: `char`, `short`, `int`, `long`, `long long`.  In addition, `float` and `double` can be used to build floating-point vector types.

Specifying a combination that is not valid for the current architecture causes GCC to synthesize the instructions using a narrower mode.  For example, if you specify a variable of type `V4SI` and your architecture does not allow for this specific SIMD type, GCC produces code that uses 4 `SIs`.

The types defined in this manner can be used with a subset of normal C operations.  Currently, GCC allows using the following operators on these types: `+, -, *, /, unary minus, ^, |, &, ~, %'.

The operations behave like C++ `valarrays`.  Addition is defined as the addition of the corresponding elements of the operands.  For
 example, in the code below, each of the 4 elements in A is added to the corresponding 4 elements in B and the resulting vector is stored in C.

~~~~c
     typedef char v4s __attribute__ ((vector_size (4)));

     v4qi a, b, c;

     c = a + b;
~~~~

Subtraction, multiplication, division, and the logical operations operate in a similar manner.  Likewise, the result of using the unary minus or complement operators on a vector type is a vector whose elements are the negative or complemented values of the corresponding elements in the operand.

It is possible to use shifting operators `<<`, `>>` on integer-type vectors. The operation is defined as following: `{a0, a1, ..., an} >> {b0, b1, ..., bn} == {a0 >> b0, a1 >> b1, ..., an >> bn}`. Vector operands must have the same number of elements.

For convenience, it is allowed to use a binary vector operation where one operand is a scalar. In that case the compiler transforms the scalar operand into a vector where each element is the scalar from the operation. The transformation happens only if the scalar could be safely converted to the vector-element type.  Consider the following code.

~~~~c
     typedef char v4s __attribute__ ((vector_size (4)));

     v4s a, b, c;
     int I;

     a = b + 1;    /* a = b + {1,1,1,1}; */
     a = 2 * b;    /* a = {2,2,2,2} * b; */

     a = I + a;    /* Error, cannot convert int to char. */
~~~~

Vectors can be subscripted as if the vector were an array with the same number of elements and base type.  Out of bound accesses invoke undefined behavior at run time.  Warnings for out of bound accesses for vector subscription can be enabled with `-Warray-bounds`. For example you could sum up all the elements of a given vector as shown in the following example:

~~~~c
     typedef char v4s __attribute__ ((vector_size (4)));

     v4s A, B[10];
     int R1, R2;
     int index;

     R1 = A[0] + A[1] + A[2] + A[3];
     R2 = B[index][0] + B[index][1] + B[index][2] + B[index][3];
~~~~

Vector subscripts are endian independent (GCC code generation generates different code for big and little endian). GAP8 is little endian.

Vector comparison is supported with standard comparison operators: `==, !=, <, <=, >, >=`. Comparison operands can be vector expressions of integer-type or real-type. Comparison between integer-type vectors and real-type vectors are not supported.  The result of the comparison is a vector of the same width and number of elements as the comparison operands with a signed integral element type.

Vectors are compared element-wise producing 0 when comparison is false and -1 (constant of the appropriate type where all bits are set) otherwise. Consider the following example.

~~~~c
     typedef char v4s __attribute__ ((vector_size (4)));

     v4s a = {1,2,3,4};
     v4s b = {3,2,1,4};
     v4s c;

     c = a >  b;     /* The result would be {0, 0,-1, 0}  */
     c = a == b;     /* The result would be {0,-1, 0,-1}  */
~~~~

The following example illustrates how to properly compare vectors, in particular how to infer a test that need to be satisfied on a all vector elements and a test that need to be satisfy on at least one vector element.

~~~~c
     typedef short v2s __attribute__ ((vector_size (4)));
     
     v2s R;
     int a;
     
     void foo(v2s V1, v2s V2)
     
     {
             R = (V1 > V2);          /* The result is a vector */
     
             if (V1 > V2)            /* Error: used vector type where scalar is required */
                     a = 1;
     
             if ((int)(V1 > V2))     /* Correct, equivalent to ((int)(V1>V2) != 0) */
                     a = 2;          /* Test will pass if at least one of the 2 comparisons */
                                     /* is true */
     
             if ((int)(V1 > V2)==-1) /* Correct,  Test will pass if the 2 comparisons are true */
                     a = 3;          
     }
~~~~

In C++, the ternary operator `?:` is available. `a?b:c`, where `b` and `c` are vectors of the same type and `a` is an integer vector with the same number of elements of the same size as `b` and `c`, computes all three arguments and creates a vector `{a[0]?b[0]:c[0], a[1]?b[1]:c[1], ...}`.  Note that unlike in OpenCL, `a` is thus interpreted as `a != 0` and not `a < 0`.  As in the case of binary operations, this syntax is also accepted when one of `b` or `c` is a scalar that is then transformed into a vector. If both `b` and `c` are scalars and the type of `true?b:c` has the same size as the element type of `a`, then `b` and `c` are converted to a vector type whose elements have this type and with the same number of elements as `a`.

Vector shuffling is available using functions `__builtin_shuffle (vec, mask)` and `__builtin_shuffle (vec0, vec1, mask)`.  Both functions construct a permutation of elements from one or two vectors and return a vector of the same type as the input vector(s).  The MASK is an integral vector with the same width (W) and element count (N) as the output vector.

The elements of the input vectors are numbered in memory ordering of VEC0 beginning at 0 and VEC1 beginning at N.  The elements of MASK are considered modulo N in the single-operand case and modulo 2*N in the two-operand case.

Consider the following example,

~~~~c
     typedef char v4s __attribute__ ((vector_size (4)));

     v4qi a = {1,2,3,4};
     v4qi b = {5,6,7,8};
     v4qi mask1 = {0,1,1,3};
     v4qi mask2 = {0,4,2,5};
     v4qi res;

     res = __builtin_shuffle (a, mask1);       /* res is {1,2,2,4}  */
     res = __builtin_shuffle (a, b, mask2);    /* res is {1,5,3,6}  */
~~~~

Note that `__builtin_shuffle` is intentionally semantically compatible with the OpenCL `shuffle` and `shuffle2` functions.

You can declare variables and use them in function calls and returns, as well as in assignments and some casts.  You can specify a vector type as a return type for a function.  Vector types can also be used as function arguments.  It is possible to cast from one vector type to another, provided they are of the same size (in fact, you can also cast vectors to and from other datatypes of the same size).

The previous example is rewritten using a user function to initialize the masks:

~~~~c
     typedef char v4s __attribute__ ((vector_size (4)));

     void   __attribute__ ((noinline)) set_mask(v4s *M, v4s val)

     {
             *M = val;
     }

     void main()

     {
          v4s a = {1,2,3,4};
          v4s b = {5,6,7,8};
          v4s mask1;
          v4s mask2;
          v4s res;

          set_mask(&mask1, (v4s) {0,1,1,3});       /* Note that the immediate aggregate has
							  to be explictly casted to v4s */
          set_mask(&mask2, (v4s) {0,4,2,5});

          res = __builtin_shuffle (a, mask1);       /* res is {1,2,2,4}  */
          printf("Res={%d,%d,%d,%d}\n", res[0], res[1], res[2], res[3]);

          res = __builtin_shuffle (a, b, mask2);    /* res is {1,5,3,6}  */
          printf("Res={%d,%d,%d,%d}\n", res[0], res[1], res[2], res[3]);
     }
~~~~

You cannot operate between vectors of different lengths or different signedness without a cast.

### Handling alignment (auto vectorization)

Even though GAP8 has full support for unaligned accesses GCC will generate better code if you provide an alignment hint:

~~~~c
     void foo(char * __restrict__ a, char * __restrict__ b)
     
     {
             int i;
     
             char *x = __builtin_assume_aligned(a, 4);
             char *y = __builtin_assume_aligned(b, 4);
     
             for (i = 0; i < SIZE; i++) {
                     x[i] = x[i] + y[i + 1];
             }
     }
~~~~

If you don't, this loop will be vectorized but a complex peeling scheme will be applied ... Performance is reasonable but code size is very bad.

Note also the `__restrict__` qualifier, it is essential otherwise you have to assume that a and b can point to the same location making the vector transformation illegal.

### Code example using vector notation

Searching a given char (Pat) in a string (A):
  
Here is another example using plain vectors and bit manipulation function. It returns the index of the first matching instance of Pat in A (compared to a sequential search the execution time is divided by 3). This code will also compile as is on any processor supported by GCC.

~~~~c
     int  __attribute__ ((noinline)) Search_Pattern_Scalar(v4s *A, int Size, char Pat)
     
     {
             v4s R;
             int i;
     
             if (Size <= 0) return -1;
             for (i=0; i<Size; i++) {
                     if (((int) (R=A[i]==(v4qi){Pat,Pat,Pat,Pat})) != 0)
                             return (4*i + (__builtin_ffs((int) (A[i]==Pat))>>3));
             }
     
             return -1;
     }
~~~~

Salient features are:

+ if (((int) (R=A[i]==(v4s){Pat,Pat,Pat,Pat})) != 0)  
  Test if Pat matches on at least one subscript of A[i] and store comparison result into R (a vector)
+ return (4*i + (__builtin_ffs((int) (A[i]==Pat))>>3))  
  Returns the byte position. Since R elements are either 0 or -1 we want to find the first 1 starting from LSB if endian is little
+ if (Size <= 0) return -1  
  A trick to let GCC knows that Size in loop is > 0, code is more compact

###	Auto vectorization code examples

GCC activates auto vectorization when -O3 is passed. You should be aware that at this level of optimization code-bloat might be an issue since loop unrolling, loop peeling and loop realignment lead to a lot of code replication.
As a rule of thumb using vector notation as explained in section 3 combined with -O2 is often a better compromise for an embedded target. Beside full target portability is not impacted since these GCC vector extensions are supported on any existing GCC target.

Performing sum of product:

~~~~c
     void foo(char * __restrict__ c1, char * __restrict__ c2, char * __restrict__ c3)
     
     {
             int i;
     
             char * cc1 = __builtin_assume_aligned(c1, 4);
             char * cc2 = __builtin_assume_aligned(c2, 4);
             char * cc3 = __builtin_assume_aligned(c3, 4);
     
             for (i=0; i<SIZE; i++) // SIZE=128
                     cc1[i] = cc1[i] + (cc2[i] + cc3[i]);
     }
~~~~

GCC for Gap8 with -O3 will produce the following fully vectorized loop

~~~~assembly
foo:
        lp.setupi       x1,32,(.L5)      # loop setup, lc+le set
.L2:
        lw      a5,0(a0)
        p.lw    a4,4(a1!)       	    # load post inc
        p.lw    a6,4(a2!)       	    # load post inc
        pv.add.b        a5,a4,a5         # Vect Op Vect
        pv.add.b        a5,a5,a6         # Vect Op Vect
.L5:
        p.sw    a5,4(a0!)       # store post inc
        ret
~~~~

Performing min/max:

~~~~c
     void foo1(char * __restrict__ a, char * __restrict__ b, char * __restrict__ c)
     {
             int i, j;
     
             char *x = __builtin_assume_aligned(a, 4);
             char *y = __builtin_assume_aligned(b, 4);
             char *z = __builtin_assume_aligned(c, 4);
     
             for (i = 0; i < SIZE; i++) { // SIZE=128
                     z[i] = (y[i] > x[i]) ? x[i] : y[i];
             }
     }
~~~~

GCC for Gap8 with -O3 will produce the following fully vectorized loop

~~~~assembly
foo1:
        lp.setupi       x1,32,(.L9)      # loop setup, lc+le set
.L7:
        p.lw    a3,4(a1!)       # load post inc
        p.lw    a5,4(a0!)       # load post inc
        pv.minu.b       a5,a5,a3         # VectU Op Vect
.L9:
        p.sw    a5,4(a2!)       # store post inc
        ret
~~~~

The last example illustrates the limits of auto vectorization. The following code performs a simple 2D matrix to 2D matrix sum. GCC partially manages to auto vectorize it: the inner loop is fully vectorized but the outer loop is a total mess with lot of code to try to realign the data through partial loop peeling. The exec time will be reasonably acceptable but code-bloat is enormous.

~~~~c
     void foo3(char * __restrict__ a, char * __restrict__ b)
     {
             int i, j;
     
             char *x = __builtin_assume_aligned(a, 4);
             char *y = __builtin_assume_aligned(b, 4);
     
             for (j = 0; j < SIZE; j++) {
                     for (i = 0; i < SIZE; i++) {
                             x[i + j * SIZE] += y[i + j * SIZE];
                     }
             }
     }
~~~~

The following code does the same thing but using direct vector notation. SIZE/4
is to account for the fact that a vector contains 4 elements.

~~~~c
     void foo4(v4qi * __restrict__ a, v4qi * __restrict__ b)
     {
             int i, j;
     
             for (j = 0; j < SIZE; j++) {
                     for (i = 0; i < SIZE/4; i++) {
                             a[i + j * SIZE/4] += b[i + j * SIZE/4];
                     }
             }
     }
~~~~

Performance is slightly better than auto vectorized version but code size is 10x smaller! Here is code generated for the entire function.

~~~~assembly
foo4:
        add     a0,a0,128
        lp.setupi       x0,128,(.L46)    # loop setup, lc+le set
.L39:
        add     a3,a0,-128
        mv      a2,a1
        lp.setupi       x1,32,(.L45)     # loop setup, lc+le set
.L40:
        lw      a5,0(a3)
        p.lw    a6,4(a2!)       	# load post inc
        pv.add.b        a5,a5,a6     # Vect Op Vect
.L45:
        p.sw    a5,4(a3!)       # store post inc
        add     a1,a1,128
.L46:
        add     a0,a0,128
        ret
~~~~
## Example of optimization with vectors and parallelization

### Introduction

In this section we will illustrate step by step how to optimize a
5 x 5, 2D convolution operating on byte inputs and using byte filter
coefficients.

All cycle measurements in these examples were obtained from actual measurements on simulation of GAP8.

We start from a straightforward single core, fixed point implementation in both a basic and then partially unrolled version. Then we use vectorization to improve performance and then further refine the vectorized version. The first step speeds up the code by a factor of 2.84 and the second step by a factor of 2.6. The combination of the two techniques runs with 7.3 times the speed of the straight scalar implementation on one core.

We then show how this single core code is modified to run on the 8 cores of
GAP8's cluster, increasing speed by an almost ideal factor of 8. The
combination of single core centric optimizations with parallelization
leads to an increase in performance of 56.8 times versus the basic single core implementation. It runs in as little as 2.5 processor cycles per full 5 x 5 convolution.

A 5 x 5, 2D convolution is simply a dot product (the sum of the multiplication of each element with the corresponding element) between all the values in
the 5 x 5 coefficient matrix and the content of the 5 x 5 square below the
coefficient matrix in the input plane.

We obtain all the convolution outputs by sliding the fixed value 5 x 5 coefficient matrix in the 2 directions of the input plane by a step of 1. If the input plane has dimension \[WxH\] the dimension of the output plane will be \[(W-4)x(H-4)\].

We assume input data and filter coefficients are fixed point numbers in
the \[-1.0 .. 1.0\] range, in Q^7^ format[^1]. When two Q^7^ numbers are
multiplied the result is a Q^14^ number, to normalize it back to Q^7^ we
simply have to shift its content by 7 bits to the right.

A single 5 x 5 convolution output is the result of a sum of 25 products
where each product has arguments in the \[-1.0 .. 1.0\] range, therefore
a convolution output is in the \[-25.0 .. 25.0\] range. In order to
avoid overflow we need to right shift the result by 7 + 5, 7 coming from the Q^7^ format of the inputs and 5 from ceil(log~2~(25)).

We choose to move down columns first, so we slide the window vertically down
the whole height of the input plane before moving to the next vertical
strip. This is illustrated in the diagram below:

![Diagram of convolution](images/convolution.png){ width=460px }

### Basic scalar implementation

~~~~~c
void __attribute__ ((noinline)) Straight2D_5x5_Convolution( 
  signed char *__restrict__ In, 
  signed char *__restrict__ Out, 
  signed char *__restrict__ Coeff, 
  unsigned int W, 
  unsigned int H) 

{ 
  int i, j, k0, k1; 

  for (i=0; i<(W-4); i++) { 
    for (j=0; j<(H-4); j++) { 
      int Result = 0; 
      for (k1=0; k1<5; k1++) { 
        for (k0=0; k0<5; k0++) {
          Result = Result + In[(j+k1)*W+i+k0]*Coeff[5*k1+k0]; 
        } 
      } 
      Out[j*(W-4)+i] = Result>>(7+5); // Normalize and avoid overflow
    }
  } 
} 
~~~~~

For this first implementation the **number of cycles per convolution output is 142**.

Below we list the Extended ISA RISC-V assembly code generated by the compiler for the central part of the kernel where most of the complexity is:

~~~~~as
1c0012c8: 00df407b lp.setup x0,t5,1c0012e2
1c0012cc: 8896 mv a7,t0
1c0012ce: 887e mv a6,t6
1c0012d0: 005350fb lp.setupi x1,5,1c0012dc
1c0012d4: 00180e8b p.lb t4,1(a6!)
# t3 load
1c0012d8: 00188e0b p.lb t3,1(a7!)
# t3 used in MAC
1c0012dc: 43ce8333 p.mac t1,t4,t3
1c0012e0: 9fb6 add t6,t6,a3
1c0012e2: 0295 addi t0,t0,5
~~~~~

Firstly, notice that the compiler has optimized the loops using the GAP8 extended ISA loop setup instructions.

We see that the inner loop is iterated 5 times and at each iteration we
do 2 reads and a MAC (multiply/accumulate). Unfortunately the t3 input of the MAC comes from the load in the instruction right before it so we have a 1 cycle stall since the data is not immediately ready. The total impact of this 1 cycle penalty is 25 cycles per convolution output. If we can get rid of this penalty we should be able to get down to 117 cycles per output.

The compiler is pipeline aware and schedules the code in order to minimize stall cycles. Unfortunately in this case the scheduling window is one iteration of the inner loop, there is simply no solution to remove this penalty. One possibility is to try unrolling the loop by compiling in -O2 mode to avoid the code bloating effect coming from systematic in-lining performed with -O3.

Another alternative is to perform partial unrolling of the loop manually. This is what we are going to do in the inner loop:

### Partially unrolled scalar implementation

~~~~c
for (i=0; i<(W-4); i++) { 
  for (j=0; j<(H-4); j++) { 
    int Result = 0; 
    for (k1=0; k1<5; k1++) { // Inner loop is unrolled
      Result = Result + In[(j+k1)*W+i+0]*Coeff[5*k1+0]; 
      Result = Result + In[(j+k1)*W+i+1]*Coeff[5*k1+1]; 
      Result = Result + In[(j+k1)*W+i+2]*Coeff[5*k1+2]; 
      Result = Result + In[(j+k1)*W+i+3]*Coeff[5*k1+3]; 
      Result = Result + In[(j+k1)*W+i+4]*Coeff[5*k1+4]; 
    } 
    Out[j*(W-4)+i] = Result>>(7+5); 
  } 
} 
~~~~

Running this new code we are now getting **97 cycles per convolution output** instead of 142. This is more than expected mostly because the generated code now is also more compact with optimal usage of post modified memory accesses. 

If we again dump the generated code we see that the inner loop is now
optimally scheduled with no more load use penalty. With this unrolled form we are now very close to the minimum number of instructions needed for a 5 x 5 convolution, 5 \* (10 loads + 5 macs) = 75. Here we have 80 (5 x 16) instructions executed:

~~~~~assembly
1c001372: 01f6c0fb lp.setup x1,a3,1c0013b0
1c001376: 00070e03 lb t3,0(a4)
1c00137a: 0058830b lb t1,5(a7!)
1c00137e: 00170d83 lb s11,1(a4)
1c001382: 0058038b lb t2,5(a6!)
1c001386: 00270283 lb t0,2(a4)
1c00138a: 00550f8b lb t6,5(a0!)
1c00138e: 00370f03 lb t5,3(a4)
1c001392: 00558e8b lb t4,5(a1!)
1c001396: 426e07b3 p.mac a5,t3,t1
1c00139a: 00470e03 lb t3,4(a4)
1c00139e: 0056030b lb t1,5(a2!)
1c0013a2: 9722 add a4,a4,s0
1c0013a4: 427d87b3 p.mac a5,s11,t2
1c0013a8: 43f287b3 p.mac a5,t0,t6
1c0013ac: 43df07b3 p.mac a5,t5,t4
1c0013b0: 426e07b3 p.mac a5,t3,t1
~~~~~

### First vectorial implementation

GAP8 has built-in support for vectors of 4 byte elements or 2 short elements. Among the supported vectorial instructions there is a one cycle dot product. In our case our input data and coefficients are bytes so they are perfect candidates for vectors of 4 elements. A dot product in this configuration computes 4 products and sums them into a 32 bit output optionally accumulating the sum with an existing value in a register in a single cycle.

If V~0~ and V~1~ are 2 vectors of 4 bytes each then:

Acc = Acc + V~0~\[0\] \* V~1~\[0\] + V~0~\[1\] \* V~1~\[1\] + V~0~\[2\] \* V~1~\[2\] + V~0~\[3\] \* V~1~\[3\].

Each individual product produces a 32 bit result and this is what makes this instruction extremely interesting because the width of its output is much larger than the width of its inputs avoiding overflow.

The diagram below shows how vectors can be used to compute a given convolution. V~0~, V~1~, V~2~, V~3~, V~4~ are vector accesses while B~0~ to B~4~ are regular byte accesses: 

![Vector layout](images/vector_layout_1.png){ width=300px }

Here is the code that uses this vector layout:

~~~~~c
void __attribute__ ((noinline)) VectorStraight2D_5x5_Convolution( 
  signed char *__restrict__ In, 
  signed char *__restrict__ Out, 
  signed char *__restrict__ Coeff, 
  unsigned int W, 
  unsigned int H) 
{ 
  int i, j, k0; 

  for (i=0; i<(W-4); i++) { 
    for (j=0; j<(H-4); j++) { 
      int Result = 0; 
      for (k0=0; k0<5; k0++) { 
        v4s Vin = *((v4s *) (&In[(j+k0)*W+i])); // Input vector
        v4s Vc  = *((v4s *) (&Coeff[k0*5])); // Coefficients accessed as vectors
        // We use a built-in to evaluate 4 of the 5 products in 1 cycle
        Result = gap8_sumdotp4(Vin, Vc, Result);
        // The last product is as usual 
        Result += In[(j+k0)*W+i+4]*Coeff[k0*5+4]; 
      } 
      Out[j*(W-4)+i] = Result>>(7+5); 
    } 
  } 
} 
~~~~~

The gap8_sumdotp4() macro expands to the appropriate gcc built-in. Refer to the API documentation for more information.

If we measure the number of cycles obtained with this new code the **number of cycles per convolution output is 50**.

### Aggressive vectorial optimization

We could likely save a little bit unrolling the inner loop as in the earlier scalar example however if we look more carefully at this code we can see an even greater optimization possibility by looking at the data that changes between 2 consecutive convolution evaluations. 

Obviously the filter stays constant for the entire input plane however 4/5th of the input plane data is also the same between convolution evaluations since we are just sliding the convolution window down by one value. If we can exploit this then we will not only save cycles but also reduce the memory accesses from shared level 1 memory by 80%.

The diagram bellow shows how the vectors are mapped for this strategy and how the vectors are updated when moving from one convolution to the next:

![Vector layout](images/vector_layout_2.png){ width=460px }

As you can see most of the vectors follow the natural order of the input. V~5~ is an exception since it is vertical. Only the first element of V~6~ is used. The right call-out box explains how we build next V~6~ from current V~5~ and V~6~. This process is called vectorial shuffling and is supported by a standard gcc built-in: \_\_builtin\_shuffle(V~0~, V~1~, ShuffleVector).

In the gcc built-in, the vector's elements are labeled from 0 to N-1 where N is the size of the vector. In our case N=4. ShuffleVector indicates which element to pick from V~0~ or V~1~ to produce the i^th^ output. The elements of V~0~ are numbered from 0 to 3 and V~1~ from 4 to 7, so for example \_\_builtin\_shuffle(V~0~, V~1~, (v4s) {7, 0, 1, 5}) will return {V~1~\[3\], V~0~\[0\], V~0~\[1\], V~1~\[1\]}. For our case, the permutation vector we need is (v2s) {1, 2, 3, 4}, a simple shift of one value.

Putting everything together we obtain the following code. As you will see there is a bit more going on since we have to take care of priming the vectors at the beginning of each vertical strip in the input data and we also have to use type casts to convert the input data from bytes to vectors:

~~~~~c
void __attribute__ ((noinline)) VectorOpt2D_5x5_Convolution( 
  signed char *__restrict__ In, 
  signed char *__restrict__ Out, 
  signed char *__restrict__ Coeff, 
  unsigned int W, 
  unsigned int H) 

{ 
  // If we promote all coefficients to vectors, notice that the layout has to 
  // be the same as the one we are using for the data
  v4s C0 = *((v4s *) &Coeff[5*0 + 0]); 
  v4s C1 = *((v4s *) &Coeff[5*1 + 0]); 
  v4s C2 = *((v4s *) &Coeff[5*2 + 0]); 
  v4s C3 = *((v4s *) &Coeff[5*3 + 0]); 
  v4s C4 = *((v4s *) &Coeff[5*4 + 0]); 
  v4s C5 = (v4s) {Coeff[5*0 + 4], Coeff[5*1 + 4], Coeff[5*2 + 4], Coeff[5*3 + 4]}; 
  v4s C6 = (v4s) (unsigned int) (((unsigned char *)Coeff)[5*4+4]); 
  int i, j, k0; 

  for (i=0; i<(W-4); i++) { 
    v4s V0; 
    // Here we have to prime the pipeline
    v4s V1 = *((v4s *) &In[(0)*W+i]); 
    v4s V2 = *((v4s *) &In[(1)*W+i]); 
    v4s V3 = *((v4s *) &In[(2)*W+i]); 
    v4s V4 = *((v4s *) &In[(3)*W+i]); 
    v4s V5 = (v4s) {0, In[(0)*W+i+4],  In[(1)*W+i+4],  In[(2)*W+i+4]}; 
    v4s V6 = (v4s) (unsigned int)(((unsigned char *)In)[(3)*W+i+4]); 
    for (j=0; j<(H-4); j++) { 
      int Result; 
      v4s *Vin = (v4s *) &In[j*W+i]; 

      // Implements the shuffle from previous output to current
      V0 = V1; V1 = V2; V2 = V3; V3 = V4;
      V5 = __builtin_shuffle(V5, V6, (v4s) {1,2,3,4}); 
      // Now these 2 reads is what is left from the original 50 ones
      V4 = Vin[0]; V6 = Vin[1]; 

     // Now we evaluate the 25 sums of products
      Result = gap8_dotp4(V0, C0); 
      Result = gap8_sumdotp4(V1, C1, Result); 
      Result = gap8_sumdotp4(V2, C2, Result); 
      Result = gap8_sumdotp4(V3, C3, Result); 
      Result = gap8_sumdotp4(V4, C4, Result); 
      Result = gap8_sumdotp4(V5, C5, Result); 
      Result = gap8_sumdotp4(V6, C6, Result); 

      Out[j*(W-4)+i] = Result>>(7+5); 
    } 
  } 
} 
~~~~~

Here is the generated assembly for this version looking only at what is done for the loop going through an entire vertical strip `(for (j=0; j<(H-4); j++) {...})`:

~~~~assembly
1c00145e:       01ea40fb                   lp.setup        x1,s4,1c00149a
1c001462:       987797d7                   pv.dotsp.b      a5,a5,t2
1c001466:       b88e17d7                   pv.sdotsp.b     a5,t3,s0
1c00146a:       000f2703                   lw      a4,0(t5)
1c00146e:       b89317d7                   pv.sdotsp.b     a5,t1,s1
1c001472:       b92897d7                   pv.sdotsp.b     a5,a7,s2
1c001476:       c9681657                   pv.shuffle2.b   a2,a6,s6
1c00147a:       b93717d7                   pv.sdotsp.b     a5,a4,s3
1c00147e:       004f2803                   lw      a6,4(t5)
1c001482:       b9f617d7                   pv.sdotsp.b     a5,a2,t6
1c001486:       b85817d7                   pv.sdotsp.b     a5,a6,t0
1c00148a:       40c7de93                   srai    t4,a5,0xc
1c00148e:       01daccab                   sb      t4,s9(s5!)
1c001492:       87f2                       mv      a5,t3
1c001494:       9f36                       add     t5,t5,a3
1c001496:       8e1a                       mv      t3,t1
1c001498:       8346                       mv      t1,a7
1c00149a:       88ba                       mv      a7,a4
~~~~

As you can see it is extremely compact.

If we measure the number of cycles obtained with this new code the **number of cycles per convolution output is 19**.

The table below summarizes the cycles per convolution for our 4 versions.

| Algorithm                  | cycles |
|:---------------------------|-------:|
| Initial version            |    142 |
| With unrolling             |    97  |
| Using vectors              |    50  |
| Aggressively using vectors |    19  |

### Parallel versions

So far we have been using a single core. Now we will see how to modify the code to use all the available cores in the GAP8 cluster.

To parallelize the code the most important thing to notice is that the evaluation of one vertical strip is completely independent from the evaluation of the other vertical strips in the input. Given this, it is straightforward to see that if we split the output plane on groups of vertical strips we will be able to provide work to all the cores that are available. The figure below
illustrates this. In the diagram below for simplicity we assume we have 3 cores:

![Parallelizing the strips](images/core_split.png){ width=300px }

The code below shows our initial basic scalar implementation extended to a parallel version:

~~~~~c
void __attribute__ ((noinline)) Straight2D_5x5_Convolution_Par(ArgConv_T *Arg) 
{ 
  // For parallel calls we use the rt_team_fork() dispatcher so in this case
  // arguments have to be grouped into a single structure and then we pass the
  // address of this structure to the function in the fork.
  // Here we simply extract arguments:
  signed char *__restrict__ In = Arg->In; 
  signed char *__restrict__ Out = Arg->Out; 
  signed char *__restrict__ Coeff = Arg->Coeff; 
  unsigned int W = Arg->W; 
  unsigned int H = Arg->H; 
  unsigned int CoreId = gap8_coreid();  // core number on which this code 
                                        // runs, in 0..MaxCore-1
  unsigned int Chunk = ChunkSize(W-4);  // Returns (W-4)/NumbeOfCores
  // Now we have the chunk size we can compute first and last positions
  // for this core
  unsigned int First = CoreId*Chunk, Last = Min(First+Chunk, W-4); 

  int i, j, k0, k1; 

  // for (i=0; i<(W-4); i++) { 
  for (i=First; i<Last; i++) { 
    for (j=0; j<(H-4); j++) { 
      int Result = 0; 
      for (k1=0; k1<5; k1++) { 
        for (k0=0; k0<5; k0++) {
          Result = Result + In[(j+k1)*W+i+k0]*Coeff[5*k1+k0]; 
        } 
      } 
      Out[j*(W-4)+i] = Result>>(7+5); 
    } 
  } 
  gap8_waitbarrier(0);  // Synchronizes with the other worker
} 
~~~~~

For all the other code variants the approach is identical.

The table bellow gives the number of cycles per convolution output for
our 4 versions, running on 1, 2, 4 and 8 cores.


|                            | 1 core | 2 cores | 4 cores | 8 cores |
|:---------------------------|:------:|:-------:|:-------:|:-------:|
| Initial version            |   142  |    71   |    35   |    19   |
| With unrolling             |   97.4 |   48.9  |   25.2  |   14.2  |
| Using vectors              |    50  |   25.2  |    13   |    7.4  |
| Aggressively using vectors |   19.2 |    9.5  |    4.8  |    2.5  |

You will notice that moving to a parallel implementation has nearly no impact on the performance on 1 core. Also the scaling factor is very close to the ideal as we increase cores, i.e. the cycles decrease proportionally to the number of cores used. In the most aggressive implementation we move from 19.2 down to 2.5 cycles per convolution as we move from 1 to 8 cores, a speed up of a factor 7.68.

If we omit the initial version and compare the result for 1 core with the unrolling version to 8 cores with the most aggressive version we move from 97.4 to 2.5 cycles, an improvement of nearly 39 times. 

The way to understand why we are seeing this much improvement is to look at the resources we are using. First we used 4 dimensional vectors, we can expect a 4 times improvement in performance. Next we parallelized the code with 8 cores, another 8 times faster on top of our original 4 times, so 32 times faster in total, consistent with our 39 times observed improvement.

[^1]: See [Q fixed point number format - https://en.wikipedia.org/wiki/Q_\(number_format\)](https://en.wikipedia.org/wiki/Q_\(number_format\))
