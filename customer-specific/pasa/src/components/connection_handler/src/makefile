#
# Standard Module Makefile version 2.0
#

# Name of the componet (team/domain prefix '_' component name)
COMPONENT_NAME = connection_handler

ifndef PRJ_ROOT
export PRJ_ROOT = $(CURDIR)/../../../../
endif
include $(PRJ_ROOT)cfg/depends.mk

# Additive Compile Flags (Flags from initiating make process will still apply)
DEFS += \
	-Wc,-Werror=return-type \
	-Wc,-Wuninitialized

# Set local includes and then the reference includes (priority order determines search path)
# Default pattern are any configuration includes (which would be things like qnx), local (Team) component directories,
# dependencies includes (other teams)
# Local (current component references should be in the form of 
#  $(CC_IFLAG)$(TEAM_ROOT)$(COMPONENT_NAME)/directory
#  Example your public include directory would be
#  $(CC_IFLAG)$(TEAM_ROOT)$(COMPONENT_NAME)/inc
# Team references should only be to other's public includes such as
#  $(CC_IFLAG)$(TEAM_ROOT)NS_MessageCenter/inc
# Global (non-team) references should be only to other's public includes such 
#  these are found in the depends include file and captured in the (DEPENDS_INCLUDES) variable
INCLUDES = \
	$(CFG_INCS) \
	$(CC_IFLAG)./ \
	$(CC_IFLAG)../include \
	$(CC_IFLAG)../../include \
	$(CC_IFLAG)../../transport_manager/include \
	$(CC_IFLAG)../../protocol_handler/include \
	$(CC_IFLAG)../../config_profile/include \
	$(CC_IFLAG)../../utils/include \
	$(CC_IFLAG)../../ \
	$(CC_IFLAG)../../../3rd_party-static/encryption/include \
	$(CC_IFLAG)../../../3rd_party/log4cxx/include \
	$(DEPENDS_INCLUDES)


# Do the same if you need to include library paths as well
#  Do an incremental in case additional library paths are defined
#  at the top-level make.  Use similar guidelines as for includes 
#  for example to include a team component library it would be 
#  $(TEAM_ROOT)NS_MessageCenter/lib/NS_MessageCenter/
LIB_PATHS += \
  $(DEPENDS_LIB_PATHS)
  

# Define binary outputs.  These can be libraries or executables.
#  Name a variable for each deliverable.  Suffixes should be
#  EXEC - For Executables -> output to the bin directory
#TIME_EXEC = $(BIN_PATH)time
#  LIB - For Static Libraries -> output to lib directory with specific naming
#MATH_LIB = $(LIB_PATH)$(LIB_PREFIX)math.$(LIB_EXT)
#  SLIB - For Shared Objects
#FRMWRK_SLIB = $(SLIB_PATH)frmwrk.$(SO_EXT)
#  LIB - Define the static library for Message Queue
#
#
COMPONENT_LIB = $(LIB_PATH)$(LIB_PREFIX)$(COMPONENT_NAME)$(DEBUG_EXT).$(LIB_EXT)

## Sources Section

# Define Library & Executable Sources (on a per deliverable basis)
#  This includes sources located in subdirectories.

# Define generic line that pulls all c, cc, cpp files 
# since your in the src folder is pull only files from there
COMPONENT_SRCS = \
	$(wildcard *.c*)

# Define sources that my not be local to your component
# here, you can define indivial files or wildcard from 
# a different folder.
NON_LOCAL_SRCS = \
 

# List of all sources to be built.  Can be assembled from the other defintitions.
#  This only defines sources for the current directory, so if there are subdirectories
#  those are not included.  (Those are found in simple subdirectory makefiles that only
#  direct the building of sources, but no linking into a binary)
SOURCES = \
	$(COMPONENT_SRCS) \
	$(NON_LOCAL_SRCS)



#
# Convert the source files to object files with correct folder location.
#
#
C_LANG_OBJECTS = $(addprefix $(BLD_PATH),$(addsuffix .$(OBJ_EXT),$(basename $(filter %.c ,$(SOURCES) ) ) ) )
CPP_LANG_OBJECTS = $(addprefix $(BLD_PATH),$(addsuffix .$(OBJ_EXT),$(basename $(filter %.cpp %.cxx,$(SOURCES) ) ) ) )
CC_LANG_OBJECTS = $(addprefix $(BLD_PATH),$(addsuffix .$(OBJ_EXT),$(basename $(filter %.cc,$(SOURCES) ) ) ) )


# List of all sources to be generated.  Can be assembled from the other defintitions.
OBJECTS = \
	$(C_LANG_OBJECTS) \
	$(CPP_LANG_OBJECTS) \
	$(CC_LANG_OBJECTS)


	
# All headers that are dependencies.  Wildcard is easy to pickup local headers.
#  This is only to automate the rebuilding, all builds on the servers are cleans
#  So this is not a huge deal when building on a component level.
HEADERS = \
	$(wildcard ../include/$(COMPONENT_NAME)/*.h*) \
	$(wildcard ../../transport_manager/include/transport_manager/*.h*) \
	$(wildcard ../../transport_manager/include/transport_manager/pasa_bt/*.h*) \
	$(wildcard ../../transport_manager/include/transport_manager/device_adapter/*.h*) \
	$(wildcard ../../transport_manager/include/transport_manager/tcp/*.h*) \
	$(wildcard ../../protocol_manager/include/protocol_handler/*.h) \
	$(wildcard ../../utils/include/utils/*.h*) \
	$(wildcard ../../utils/include/utils/threads/*.h*) \

	
LIBRARIES = \
	$(COMPONENT_LIB)


# Make targets
#  Standard
all: banner module_dirs subdirs local library binary

debug: 
	$(MAKE) TARGET=arm DEBUG=TRUE all

base: banner module_dirs subdirs local

# Standard Building of Source Files (Default builds for all objects defined above)
$(C_LANG_OBJECTS): $(SOURCES) $(HEADERS)
	$(CC_CMD)

$(CPP_LANG_OBJECTS): $(SOURCES) $(HEADERS)
	$(CPP_CMD)

$(CC_LANG_OBJECTS): $(SOURCES) $(HEADERS)
	$(CCC_CMD)

local: $(OBJECTS)

# Defines specific for each deliverable

# For a static library
$(COMPONENT_LIB): $(OBJECTS)
	$(AR_CMD)

# Standard set of derived targets
library: base \
	$(LIBRARIES) 
	@echo "***** `date` Done building library: $(COMPONENT_NAME) ******" 

binary: base \
	$(BINARIES)

# Subdirs should be to jump to subdirectories
#  standard form is of
#  $(MAKE) -C subdirectory_name $(MAKECMDGOALS) 
subdirs:
	
clean:
	-rm -f $(BINARIES)
	-rm -f $(LIBRARIES)
	-rm -f $(OBJECTS)
	@echo "objs: --> $(OBJECTS)"
	@echo "sources: --> $(SOURCES)"
	@echo "headers: --> $(HEADERS)"
	@echo "includes: --> $(INCLUDES)"
	@echo "lib: --> $(LIBRARIES)" 
	@echo "bin: --> $(BINARIES)"


module_dirs: build_dirs
