MCU          = atmega328
ARCH         = AVR8
F_CPU        = 16000000
OPTIMIZATION = s
TARGET       = solderocket
SRC          = $(TARGET).c
CC_FLAGS     =
LD_FLAGS     =

# Default target
all:

# Include DMBS build script makefiles
DMBS_PATH   ?= ./dmbs/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk
