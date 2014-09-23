# QCC arm COMPILER SETTINGS
#  Standard form of tool.mk file configured for qnx arm v7
#  Note: Make sure QNX_HOST is set to install version desired

export PATH := $(QNX_HOST)/usr/bin;$(QNX_HOST)/unsupported/Python25;$(PATH)
export MAKEFLAGS := $(QNX_TARGET)/usr/include

# Tools and Options
LIB_EXT = a
SO_EXT = so
LIB_PREFIX = lib
OBJ_EXT = o
BIN_EXT = 
CC = ${QNX_HOST}/usr/bin/qcc -c
CPP = ${QNX_HOST}/usr/bin/qcc -c
AS = ${QNX_HOST}/usr/bin/ntoarmv7-as
AR = ${QNX_HOST}/usr/bin/ntoarmv7-ar
STRIP = ${QNX_HOST}/usr/bin/ntoarmv7-strip
LD = qcc
# Add addtion Linker Flags....
# If your application contains c++ code you will need 
# the -lang-c++
LD_CPP = -lang-c++

# Compile options and commands
TARGET_FLG = -Vgcc_ntoarmv7le_gpp

# Set Debug Flag
ifdef DEBUG
CC_DEBUG_FLG = -g
DEBUG_EXT=_g
endif

# Set Profiling Flag
ifdef PROFILING
CC_PROFILING_FLG = -p
endif

# Set Optimization Flag 
ifeq ($(OPTIMIZATION),OPTIMIZE)
CC_OPT_FLG = -O2
else
ifeq ($(OPTIMIZATION),FULL_OPTIMIZE)
CC_OPT_FLG = -O3
else
ifeq ($(OPTIMIZATION),NONE)
CC_OPT_FLG = 
endif 
endif
endif


# Compiler Options
CC_OFLAG = -o 
CC_IFLAG = -I
CC_FLAGS = $(TARGET_FLG) -w9 -Wc,-Wall -shared $(CC_OPT_FLG) $(CC_PROFILING_FLG) $(CC_DEBUG_FLG) -DOS_POSIX -DEXTENDED_POLICY
# To build aoa support need to uncomment next line
#export BUILD_AOA_SUPPORT := TRUE
ifdef BUILD_AOA_SUPPORT
CC_FLAGS += -DAOA_SUPPORT
endif
CC_TARGET = $(@)
CC_CMD = $(CC) $(CC_FLAGS) $(DEFS) $(INCLUDES) $(@F:.$(OBJ_EXT)=.c) -o $(@) 
CPP_CMD = $(CPP) $(CC_FLAGS) $(DEFS) $(INCLUDES) $(@F:.$(OBJ_EXT)=.cpp) -o $(@) 
CXX_CMD = $(CPP) $(CC_FLAGS) $(DEFS) $(INCLUDES) $(@F:.$(OBJ_EXT)=.cxx) -o $(@)
CCC_CMD = $(CPP) $(CC_FLAGS) $(DEFS) $(INCLUDES) $(@F:.$(OBJ_EXT)=.cc) -o $(@) 
 
# Archive Options  (Static Library)
AR_OFLAG = 
AR_FLAGS = -rP
AR_CMD = $(AR) $(AR_FLAGS) $(AR_OFLAG)$(@) $^

# Linker Options
# Executable
# Used in the form of $(LD_CMD) -L[Library Paths] -l[libraries] [objects] $(LD_OFLAG) executable_name.$(BIN_EXT)
LD_FLAGS = $(TARGET_FLG) $(CC_PROFILING_FLG) $(CC_DEBUG_FLG) $(LD_CPP)
LD_OFLAG = -o 
LD_CMD = $(LD) -L$(LIB_PATH) $(LD_FLAGS)
EXEC_CMD = $(LD_CMD) $(LD_OFLAG) $(@) $(OBJECTS) $(addprefix -L , $(LIB_PATHS)) -Bstatic $(addprefix -l , $(STATIC_LIBS)) -Bdynamic $(addprefix -l , $(DYNAMIC_LIBS))

# To create shared objects use the linker command but add -shared
SO_CMD = $(LD) $(LD_FLAGS) -shared 
SLIB_CMD = $(SO_CMD) $(OBJECTS) $(LD_OFLAG) $(@) $(addprefix -L, $(LIB_PATHS)) \
		-Bstatic $(addprefix -l,$(STATIC_LIBS)) -Bdynamic $(addprefix -l,$(DYNAMIC_LIBS)) 

