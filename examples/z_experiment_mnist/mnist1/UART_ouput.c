// Change this to UART, this is from PULPDronet which used SPI
// pulp-dronet has a continuous output, but MNIST has a 1 output which is binary
// similar to the collision probability 

// adjust to the right amount of outputs that is being send to the Crazyflie
static short int		SPIM_tx[9];


// At dense layer 1
	SPIM_tx[0] = L2_output[16][0];

// At dense layer 2
	SPIM_tx[1] = L2_output[17][0];

/* --------------------------- SPIM CONFIGURATION --------------------------- */

#ifdef SPI_COMM
	// configure the UART device
	uart_config_t uart_conf;
	// get default configuration
	UART_GetDefaultConfig(&uart_conf);
   	uart_conf.baudRate_Bps = 115200U;
    uart_conf.parityMode   = uUART_ParityDisabled;
    uart_conf.stopBitCount = uUART_OneStopBit;

	// open the device
	UART_Init(NULL, &spim_conf, NULL);
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