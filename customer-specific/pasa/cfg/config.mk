#
# Standard Configuration Makefile version 2.0
#

.SUFFIXES:

# Setup configuration exports to make sure other "subprojects" have the correct configuration settings.
# Export shared variables
export DEFS
export LIBS
export LIB_PATHS

# Export setup
export MAKE_DEPS := $(PRJ_ROOT)cfg/depends.mk
export MAKE_CFG := $(TOOLS_CFG)config.mk

# Set the location of build files.  By default all build files become local to the initating location of the build so
#  one doesn't impact a referenced project
# Alteratively a directed path can also be supported.
ifndef PRJ_OUTPUT_ROOT
PRJ_OUTPUT_ROOT = $(PRJ_ROOT)
endif

# Select Compiler Tool by name and select the tool configuration file
# default behavior is to generate for arm
ifeq ($(TARGET),analysis)
export MAKE_TOOLS := $(TOOLS_CFG)analysis.mk
else
ifeq ($(TARGET),x86)
export MAKE_TOOLS := $(TOOLS_CFG)qnx_x86.mk
else
ifeq ($(TARGET),armle)
export MAKE_TOOLS := $(TOOLS_CFG)qnx_arm.mk
else
export MAKE_TOOLS := $(TOOLS_CFG)qnx_armv7.mk
TARGET = armle-v7
endif
endif
endif


# Set specific flags as path suffixes to make sure the right build options are picked up
ifdef DEBUG
BUILD_CFG+=/debug
endif
ifdef PROFILING
BUILD_CFG+=/profile
endif
ifndef BUILD_CFG
BUILD_CFG=/release
endif

# Set File Paths
export BIN_PATH := $(PRJ_OUTPUT_ROOT)bin/$(TARGET)$(BUILD_CFG)/
export BLD_PATH := $(PRJ_OUTPUT_ROOT)bld/$(TARGET)$(BUILD_CFG)/$(COMPONENT_NAME)/
export LIB_PATH := $(PRJ_OUTPUT_ROOT)lib/$(TARGET)$(BUILD_CFG)/
export SLIB_PATH := $(PRJ_OUTPUT_ROOT)dll/$(TARGET)$(BUILD_CFG)/
export BUILD_DIRS = $(BIN_PATH) $(LIB_PATH) $(SLIB_PATH) $(BLD_PATH)
export BUILD_ROOTS := $(addprefix $(PRJ_OUTPUT_ROOT), bin bld lib dll) 

# Set Staging Location - set to a default location to prevent accidents
ifndef STAGE_LOCATION
export STAGE_LOCATION = C:/temp/stage/
endif

# Now include the defintions that have been exported
include $(MAKE_TOOLS)

# Add additional configuration includes reflective of project and decisions expressed in include makefiles.  
#  So local includes, base infrastructure, and target platform.
#  This is also the variable to add local overriding header file locations for configuration in makefiles
#  prior to spawning to subprojects.  Which means if you want to utilize different header files than those 
#  the project you are going to build this will force an early search path to override with.
ifndef CFG_INCS
	export CFG_INCS += 
endif 

# Standard Targets (and defaults)
all: 

subdirs:

# Building necessary directories
build_dirs: $(BUILD_DIRS)

$(BUILD_DIRS):
	-mkdir -p $(@)

# Stage "public" directories	
stage:
	-rm -r $(STAGE_LOCATION)/$(COMPONENT_NAME)
	mkdir -p $(STAGE_LOCATION)/$(COMPONENT_NAME)
	cp -r ../bin ../dll ../lib ../inc ../cfg ../doc ../data $(STAGE_LOCATION)/$(COMPONENT_NAME)/

	
# Debug Headers	
banner: 
	@echo Making $(COMPONENT_NAME)
	@echo Bld Roots $(BLD_ROOTS)
