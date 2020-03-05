// Change this to UART, this is from PULPDronet which used SPI

// adjust to the right amount of outputs that is being send to the Crazyflie
static short int		SPIM_tx[2];
static short int		SPIM_rx[2];  //not used

// At dense layer 1
	SPIM_tx[0] = L2_output[16][0];

// At dense layer 2
	SPIM_tx[1] = L2_output[17][0];

/* --------------------------- SPIM CONFIGURATION --------------------------- */

#ifdef SPI_COMM
	// configure the SPI device
	rt_spim_conf_t spim_conf;
	// get default configuration
	rt_spim_conf_init(&spim_conf);
	spim_conf.max_baudrate = 2000000;
	spim_conf.id = 1; 
	spim_conf.cs = 0;
	spim_conf.wordsize = RT_SPIM_WORDSIZE_8;

	// open the device
	rt_spim_t *spim = rt_spim_open(NULL, &spim_conf, NULL);
#ifdef VERBOSE
	printf("SPI Master opening:\t\t\t%s\n", spim?"Ok":"Failed");
#endif
	if(spim == NULL) return -1;

#endif // SPI_COMM


// SPI write out result
#ifdef SPI_COMM
		// SPI write out result
		rt_spim_send(spim, SPIM_tx, SPIM_BUFFER*8, RT_SPIM_CS_AUTO, NULL);
#endif


// close SPI interface
#ifdef SPI_COMM
	// close SPI interface
	rt_spim_close(spim, NULL);
#endif