# Copyright (C) 2017 ETH Zurich, University of Bologna and GreenWaves Technologies
# All rights reserved.
#
# This software may be modified and distributed under the terms
# of the BSD license.  See the LICENSE file for details.

export GAP_USE_OPENOCD=1
io=host

PULP_APP = test
PULP_APP_FC_SRCS = test.c
PULP_CFLAGS = -O3 -g #-DPRINTF_UART

PLPBRIDGE_FLAGS += -f -jtag


include $(GAP_SDK_HOME)/tools/rules/pulp_rules.mk
