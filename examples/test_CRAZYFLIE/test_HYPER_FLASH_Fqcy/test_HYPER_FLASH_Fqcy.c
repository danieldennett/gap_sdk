// Comment out following #define to stop displaying read back memory contents
#define HYPERBUS_DEBUG

// Define here target SOC frequency, Memory will run at same speed (166MHz max. to stay in Cypress's sign-off spec)
#define FQCY_MHZ 120



#include "hyperbus_api.h"

#define BUFFER_SIZE             256      

#define DEVICE_READY_OFFSET     7
#define ERASE_STATUS_OFFSET     5
#define PROGRAM_STATUS_OFFSET   4

/* Write and read address */
#define SA                      0x4000


    
typedef struct {
    uint16_t data;
    uint16_t addr;
} cmdSeq;

/* Chip erase */
// GAP_L2_DATA  cmdSeq eraseSeq[6] = {{0xAA, 0x555}, {0x55, 0x2AA}, {0x80, 0x555}, {0xAA, 0x555}, {0x55, 0x2AA}, {0x10, 0x555}}; 

/* Sector erase */
GAP_L2_DATA  cmdSeq Erase_Seq[6] = {{0xAA, 0x555}, {0x55, 0x2AA}, {0x80, 0x555}, {0xAA, 0x555}, {0x55, 0x2AA}, {0x30, SA}};

GAP_L2_DATA  cmdSeq VCR_Seq[4]   = {{0xAA, 0x555}, {0x55, 0x2AA}, {0x38, 0x555}, {0x8e0b, 0x0}};
// (XC) change VCR -- specifically (attempt to) change Read Latency (0x00=5 cycles to 0x0B=16cycles)
//GAP_L2_DATA  cmdSeq VCR_Seq[4]   = {{0xAA, 0x555}, {0x55, 0x2AA}, {0x38, 0x555}, {0x8eBb, 0x0}};

GAP_L2_DATA  cmdSeq Reg_Seq     = {0x70, 0x555};

/* Write 512/4 = 128 word to Sector addr 0x4xxx */
GAP_L2_DATA  cmdSeq WB_Seq[4]   = {{0xAA, 0x555}, {0x55, 0x2AA}, {0x25, SA}, {BUFFER_SIZE - 1, SA}};
GAP_L2_DATA  cmdSeq WP_Seq[3]   = {{0xAA, 0x555}, {0x55, 0x2AA}, {0xA0, 0x555}};
GAP_L2_DATA  cmdSeq BPC_Seq     = {0x29, SA};

/* The high 16 bits for txHyperbusSamples is useless */
GAP_L2_DATA  uint16_t txHyperbusSamples[BUFFER_SIZE];
GAP_L2_DATA  uint16_t rxHyperbusSamples[BUFFER_SIZE];

uint16_t read_status(hyperbus_t *obj)
{
    /* Read status register */
    hyperbus_write(obj, Reg_Seq.addr << 1, Reg_Seq.data, uHYPERBUS_Mem_Access, uHYPERBUS_Flash);

    uint16_t status_reg = hyperbus_read(obj, 0, uHYPERBUS_Mem_Access, uHYPERBUS_Flash);

    #ifdef HYPERBUS_DEBUG
    printf("Read status_reg = 0x%04x\n", status_reg);
    #endif

    return status_reg;
}


// -------------------------------------------------

void conf_flash(hyperbus_t *obj)
{

// -- Addition XC

    /* Set VCR to appropriate read clock cycle latency -- was fixed to 5 delay cycles (ok for 50MHz only) in original code */
    
    uint32_t  fqcy = FQCY_MHZ;
    uint32_t   latency_cycles;
    
    if (fqcy >156 )
        latency_cycles = 16;
    else if (fqcy >145 )
        latency_cycles = 15;
    else if (fqcy >125 )
        latency_cycles = 14;            
    else if (fqcy >135 )
        latency_cycles = 13;
    else if (fqcy >114 )
        latency_cycles = 12;
    else if (fqcy >104 )
        latency_cycles = 11;
    else if (fqcy > 93)
        latency_cycles = 10;
    else if (fqcy > 83)
        latency_cycles = 9;
    else if (fqcy > 72)
        latency_cycles = 8;
    else if (fqcy > 62)
        latency_cycles = 7;
    else if (fqcy > 52)
        latency_cycles = 6;
    else 
        latency_cycles = 5;
                      
    VCR_Seq[3].data = (0x8e0b & 0xFF0F) | ((latency_cycles -5) <<4);
    printf("\nHyperFlash read latency set to %d clock cycles\n", latency_cycles); 
    
// -- End addition XC
            
    for (int i = 0; i < 4; i++)
        hyperbus_write(obj, VCR_Seq[i].addr << 1, VCR_Seq[i].data, uHYPERBUS_Mem_Access, uHYPERBUS_Flash);
}


// -------------------------------------------------

void erase_page_in_flash(hyperbus_t *obj, uint32_t flash_addr)
{
    /* Erase sector 0x0E00 */
    for ( int i = 0; i < 5; i++) {
        hyperbus_write(obj, Erase_Seq[i].addr << 1, Erase_Seq[i].data, uHYPERBUS_Mem_Access, uHYPERBUS_Flash);
    }

    hyperbus_write(obj, flash_addr, Erase_Seq[5].data, uHYPERBUS_Mem_Access, uHYPERBUS_Flash);
}


// -------------------------------------------------

void write_page_in_flash(hyperbus_t *obj, uint32_t flash_addr)
{
    int i = 0;

    /* Write to Buffer command sequence */
    for (i = 0; i < 3; i++)
        hyperbus_write(obj, WP_Seq[i].addr << 1, WP_Seq[i].data, uHYPERBUS_Mem_Access, uHYPERBUS_Flash);

    /* Word Program */
    hyperbus_block_write(obj, flash_addr, (uint32_t *) txHyperbusSamples, BUFFER_SIZE * sizeof(uint16_t), uHYPERBUS_Mem_Access, uHYPERBUS_Flash);
}


// -------------------------------------------------

void read_page_from_flash(hyperbus_t *obj, uint32_t flash_addr)
{
    hyperbus_block_read(obj, flash_addr, (uint32_t *) rxHyperbusSamples, BUFFER_SIZE * sizeof(uint16_t), uHYPERBUS_Mem_Access, uHYPERBUS_Flash);
}


// -------------------------------------------------

int wait_process_end(hyperbus_t *obj, uint32_t err_bit)
{
    uint16_t reg;

    /* Wait the end of process
     * Status Register (SR)
     * bit 4 -> program status bit, 0-success ; 1-failure
     * bit 5 -> erase status bit,   0-success ; 1-failure
     * bit 7 -> device ready bit,   0-busy    ; 1-ready
     */
    do {
        reg = read_status(obj);

        if ( reg & (1 << err_bit))
        {
            printf("ERR bit = 1, an error occurs in  operation\n");
            return 1;
        }
    } while( !(reg & (1 << DEVICE_READY_OFFSET)));

    return 0;
}


// -------------------------------------------------

int test_hyperbus()
{
  static uint32_t iteration_num = 0;

  hyperbus_t hyperbus0;

  /* Hyperbus Flash Default latency is 5 clock, can not change */
  int latency = 0;

    /* HYPERBUS pins init, HYPERBUS udma channel init */
    hyperbus_init(&hyperbus0, HYPERBUS_DQ0, HYPERBUS_DQ1, HYPERBUS_DQ2, HYPERBUS_DQ3,
                  HYPERBUS_DQ4, HYPERBUS_DQ5, HYPERBUS_DQ6, HYPERBUS_DQ7,
                  HYPERBUS_CLK, HYPERBUS_CLKN, HYPERBUS_RWDS, HYPERBUS_CSN0, HYPERBUS_CSN1);

    /* Config memory access timing for TX and RX*/
    hyperbus_set_timing(&hyperbus0, 4, 4, 4, latency, 0, uHYPERBUS_Flash);
    hyperbus_set_timing(&hyperbus0, 4, 4, 4, latency, 1, uHYPERBUS_Flash);

    /* Initialize the samples */
    iteration_num++;
    for (int i = 0; i< BUFFER_SIZE; i++)
        txHyperbusSamples[i] = i + iteration_num;  // iteration_num is just to have difft buffer contenst at each iteration

    /* Configuration */
    conf_flash(&hyperbus0);

    /* Erase chip */
    erase_page_in_flash(&hyperbus0, SA);
    wait_process_end(&hyperbus0,  ERASE_STATUS_OFFSET);
    printf("Erase finished.\n");

    /* Write buffer no burst */
    write_page_in_flash(&hyperbus0, SA);
    wait_process_end(&hyperbus0,  PROGRAM_STATUS_OFFSET);
    printf("Write finished.\n");

    /* Read buffer burst */
    read_page_from_flash(&hyperbus0, SA);
    printf("Read finished.\n");

    /* Release hyperbus driver */
    hyperbus_free(&hyperbus0);

    /* Error check */
    int error = 0;

    for (int i = 0; i < BUFFER_SIZE; i++)
    {
//        if(rxHyperbusSamples[i] != (uint16_t) i) error++;
        if(rxHyperbusSamples[i] != txHyperbusSamples[i]) error++;
        
        
        #ifdef HYPERBUS_DEBUG
        printf("RX_DATA[%d] = 0x%04x\n", i, rxHyperbusSamples[i]);
        #endif
    }

    return error;
}


// ======================================================================

int main()
{
    printf("\n\n===  Fabric controller code execution for mbed_os HYPERBUS FLASH Customized Frequency test  ===\n\n");


    // ---------------------------------------------

    int voltage_mV = 1200;
    int fqcy_Hz = FQCY_MHZ * 1000000;
   
    
    /* Set voltage */
    if (PMU_SetVoltage(voltage_mV, 1)) {
            printf("Error when changing voltage\n");
            printf("Stop\n");
            return -1;
    }

    /* Set frequency */
    if (FLL_SetFrequency(uFLL_SOC, fqcy_Hz, 1) == -1) {
            printf("Error when changing frequency, check Voltage value!\n");
            printf("Stop\n");
            return -1;
    }
    printf("Frequency = %d Hz, Voltage = %d mv\n\n", FLL_GetFrequency(uFLL_SOC), voltage_mV);
     // ---------------------------------------------   
    

    int error;
    error = test_hyperbus();



    if (error)
    {
        printf("---Test failed\n");
//        return -1;
    }
    else
    {
        printf("+++Test success\n");
//        return 0;
    }

}
