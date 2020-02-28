#include "pmsis.h"

PI_L1 spinlock_t spinlock;
PI_L1 int32_t tas_addr;

void cluster_delegate(void *arg)
{
    printf("Cluster master core entry\n");
    /* Task dispatch to cluster cores. */
    cl_sync_init_spinlock(&spinlock, &tas_addr);
    pi_cl_team_fork(pi_cl_cluster_nb_cores(), cluster, arg);
    printf("Cluster master core exit\n");
}

	volatile int iter = 0;
    while(1) {

		// wait on input image transfer 
		while(imgTransferDone==0) {
			rt_event_yield(NULL);
		}
		imgTransferDone=0;

		// event_cluster = rt_event_get_blocking(NULL);
		// event_capture = rt_event_get(NULL, enqueue_capture, NULL);

		// execute the function "RunPULPDronet" on the cluster
		rt_cluster_call(NULL, CID, (void *) RunPULPDronet, NULL, stacks, STACK_SIZE, STACK_SIZE, rt_nb_pe(), event_cluster);
            
        pi_task_wait_on(event_capture);
		// rt_event_wait(event_cluster);

        printf("Cluster master core entry\n");
        /* Task dispatch to cluster cores. */
        cl_sync_init_spinlock(&spinlock, &tas_addr);
        pi_cl_team_fork(pi_cl_cluster_nb_cores(), cluster, arg);
        printf("Cluster master core exit\n");

		pi_uart_write(&uart, &rec_digit, 1);


        uint32_t core_id = pi_core_id(), cluster_id = pi_cluster_id();
        sprintf(hello, "[0x%X 0x%X] Hello World!\n", cluster_id, core_id);
        // pi_uart_write(&uart, hello, strlen(hello));

        struct pi_device cluster_dev = {0};
        struct pi_cluster_conf cl_conf = {0};

        /* Init cluster configuration structure. */
        pi_cluster_conf_init(&cl_conf);
        cl_conf.id = 0;                /* Set cluster ID. */
        /* Configure & open cluster. */
        pi_open_from_conf(&cluster_dev, &cl_conf);
        if (pi_cluster_open(&cluster_dev))
        {
            printf("Cluster open failed !\n");
            pmsis_exit(-2);
        }

        /* Prepare cluster task and send it to cluster. */
        struct pi_cluster_task cl_task = {0};
        cl_task.entry = cluster_delegate;
        cl_task.arg = NULL;

        pi_cluster_send_task_to_cl(&cluster_dev, &cl_task);

        pi_cluster_close(&cluster_dev);


#ifdef VERBOSE
	#ifdef DEBUG
		printf("Result[steer][coll]:\t%f\t%f\n", fixed2float(SPIM_tx[0], NORM_ACT), fixed2float(SPIM_tx[1], NORM_ACT));
	#else
		printf("Result[steer][coll]:\t%d\t%d\n", SPIM_tx[0], SPIM_tx[1]);
	#endif
#endif

		iter++;
    printf("iterations %d", iter);
	}