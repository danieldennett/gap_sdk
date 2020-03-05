# User Test
#------------------------------------------
TEST_C          = $(wildcard  ./*.c)

#INC            += $(GAP_SDK_HOME)/examples/mbed-examples/GAPUINO_BSP/Components/himax $(GAP_SDK_HOME)/examples/mbed-examples/Driver



# This is to store files on PC via the bridge
# The figure after -fileIO is optional (2 by default); it's a latency because program must be loaded first  
PLPBRIDGE_FLAGS += -fileIO 4

#This is to use OLIMEX JTAG probe:
PLPBRIDGE_FLAGS += -ftdi



include $(GAP_SDK_HOME)/tools/rules/mbed_rules.mk
