# This file is a makefile included from the top level makefile which
# defines the sources built for the target.

# Define the prefix to this directory. 
# Note: The name must be unique within this build and should be
#       based on the root of the project

TARGET_OSC_PATH = OSC

# Add tropicssl include to all objects built for this target
INCLUDE_DIRS += inc/OSC

# C source files included in this build.
CSRC += $(TARGET_SRC_PATH)/OSC/OSCMatch.c

# C++ source files included in this build.
CPPSRC += $(TARGET_SRC_PATH)/OSC/OSCData.cpp
CPPSRC += $(TARGET_SRC_PATH)/OSC/OSCMessage.cpp
CPPSRC += $(TARGET_SRC_PATH)/OSC/OSCBundle.cpp

# ASM source files included in this build.
# ASRC +=

