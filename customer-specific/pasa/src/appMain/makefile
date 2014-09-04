#
# Standard Module Makefile version 2.0
#

# Name of the componet (team/domain prefix '_' component name)
COMPONENT_NAME = SmartDeviceLink

ifndef PRJ_ROOT
export PRJ_ROOT = $(CURDIR)/../../
endif
include $(PRJ_ROOT)cfg/depends.mk



# Additive Compile Flags (Flags from initiating make process will still apply)
DEFS +=

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
	$(CC_IFLAG). \
	$(CC_IFLAG)../components/application_manager/include \
	$(CC_IFLAG)../components/media_manager/include \
	$(CC_IFLAG)../components/connection_handler/include \
	$(CC_IFLAG)../components/protocol_handler/include \
	$(CC_IFLAG)../components/transport_manager/include \
	$(CC_IFLAG)../components/smart_objects/include \
	$(CC_IFLAG)../components/formatters/include \
	$(CC_IFLAG)../components/request_watchdog/include \
	$(CC_IFLAG)../components/config_profile/include \
	$(CC_IFLAG)../components/hmi_message_handler/include \
	$(CC_IFLAG)../components/utils/include \
	$(CC_IFLAG)../components/policies/include \
	$(CC_IFLAG)../components/resumption/include \
	$(CC_IFLAG)../components \
	$(CC_IFLAG)../components/include \
	$(CC_IFLAG)../3rd_party-static/MessageBroker/include \
	$(CC_IFLAG)../3rd_party-static/jsoncpp/include \
	$(CC_IFLAG)../3rd_party-static/encryption/include \
	$(CC_IFLAG)../3rd_party/log4cxx/include \
	$(DEPENDS_INCLUDES)	

# Do the same if you need to include library paths as well
#  Do an incremental in case additional library paths are defined
#  at the top-level make.  Use similar guidelines as for includes 
#  for example to include a team component library it would be 
#  $(TEAM_ROOT)NS_MessageCenter/lib/NS_MessageCenter/
LIB_PATHS += \
	$(SLIB_PATH) \
	$(DEPENDS_LIB_PATHS)

# Define your libs here addtional to the DEPENDS_DYNAMIC_LIBS or DEPENDS_STATIC_LIBS
# - see depends.mk  
# This only defines the static libs
STATIC_LIBS += \
	encryption$(DEBUG_EXT) \
	jsoncpp$(DEBUG_EXT) \
	MessageBroker$(DEBUG_EXT) \
	MessageBrokerClient$(DEBUG_EXT) \
	MessageBrokerServer$(DEBUG_EXT) \
	utils$(DEBUG_EXT) \
	smart_objects$(DEBUG_EXT) \
	config_profile$(DEBUG_EXT) \
	formatters$(DEBUG_EXT) \
	interfaces$(DEBUG_EXT) \
	request_watchdog$(DEBUG_EXT) \
	hmi_message_handler$(DEBUG_EXT) \
	connection_handler$(DEBUG_EXT) \
	protocol_handler$(DEBUG_EXT) \
	protocol$(DEBUG_EXT) \
	transport_manager$(DEBUG_EXT) \
	resumption$(DEBUG_EXT) \
	usage_statistics$(DEBUG_EXT) \
	application_manager$(DEBUG_EXT) \
	media_manager$(DEBUG_EXT) \
	crypto


# This only defines the dynamic libs
DYNAMIC_LIBS += \
	socket \
	ipod \
	iap2client \
	qdb \
	apr-1$(DEBUG_EXT) \
	expat$(DEBUG_EXT) \
	aprutil-1$(DEBUG_EXT) \
	log4cxx$(DEBUG_EXT)

ifdef BUILD_AOA_SUPPORT
DYNAMIC_LIBS += pps aoa$(DEBUG_EXT)
endif

#  LIB - For Static Libraries -> output to lib directory with specific naming
#MATH_LIB = $(LIB_PATH)$(LIB_PREFIX)math.$(LIB_EXT)
#  SLIB - For Shared Objects
#FRMWRK_SLIB = $(SLIB_PATH)frmwrk.$(SO_EXT)
#  LIB - Define the static library for Message Queue
#COMPONENT_LIB = $(LIB_PATH)$(LIB_PREFIX)$(COMPONENT_NAME).$(LIB_EXT)
#
#
# Define binary outputs.  These can be libraries or executables.
#  Name a variable for each deliverable.  Suffixes should be
#  EXEC - For Executables -> output to the bin directory
#
#
COMPONENT_EXEC = $(BIN_PATH)$(COMPONENT_NAME)$(DEBUG_EXT)

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
	$(COMPONENT_SRCS)

#
# Convert the source files to object files with correct folder location.
#
#
C_LANG_OBJECTS = $(addprefix $(BLD_PATH),$(addsuffix .$(OBJ_EXT),$(basename $(filter %.c ,$(SOURCES) ) ) ) )
CPP_LANG_OBJECTS = $(addprefix $(BLD_PATH),$(addsuffix .$(OBJ_EXT),$(basename $(filter %.cpp,$(SOURCES) ) ) ) )
CC_LANG_OBJECTS = $(addprefix $(abspath $(BLD_PATH))/,$(addsuffix .$(OBJ_EXT),$(basename $(filter %.cc,$(SOURCES) ) ) ) )


# List of all sources to be generated.  Can be assembled from the other defintitions.
OBJECTS = \
	$(C_LANG_OBJECTS) \
	$(CPP_LANG_OBJECTS) \
	$(CC_LANG_OBJECTS)

# assuming all objects should be included in the executable.
ALL_OBJECTS = $(addprefix $(BLD_PATH),$(addsuffix .$(OBJ_EXT),$(notdir $(basename $(NON_LOCAL_SRCS) )) ) ) \
				$(OBJECTS)

# All headers that are dependencies.  Wildcard is easy to pickup local headers.
#  This is only to automate the rebuilding, all builds on the servers are cleans
#  So this is not a huge deal when building on a component level.
HEADERS = \
	$(wildcard *.h*)

# Binaries are any executables.  Creating the list faciliates the default build target
BINARIES = \
	$(COMPONENT_EXEC)
	
LIBRARIES = \
	$(COMPONENT_LIB)

# Make targets
#  Standard
all: banner module_dirs subdirs local binary

base: banner module_dirs subdirs local

# Standard Building of Source Files (Default builds for all objects defined above)
$(C_LANG_OBJECTS): $(SOURCES) $(HEADERS)
	$(CC_CMD)

$(CPP_LANG_OBJECTS): $(SOURCES) $(HEADERS)
	$(CPP_CMD)

$(CC_LANG_OBJECTS): $(SOURCES) $(HEADERS)
	$(CCC_CMD)

local: $(OBJECTS)

# For an executable 
#  General form is:
#	$(LD_CMD) [additional flags] -L[Library Paths] -l[libraries] $(EXEC_OBJS) $(LD_OFLAG) $(@)
#  Since the -L and -l are in front of a list it can uses as follows
#   $(LD_CMD) $(addprefix -L, $(LIB_PATHS)) {-Bstatic|-Bshared} $(addprefix -l, lib1 lib2) $(EXEC_OBJS) $(LD_OFLAG) $(@)
#  Use -Bstatic and -Bdynamic before libraries to indicate whether they should be static or shared.  (applies for all following libs)

$(COMPONENT_EXEC): $(OBJECTS) subdirs
	$(LD_CMD) $(LD_OFLAG) $(@) $(ALL_OBJECTS) \
	$(addprefix -L , $(LIB_PATHS)) \
	-Bstatic -Wl,"-start-group" $(addprefix -l , $(STATIC_LIBS)) -Wl,"-end-group" -Bdynamic $(addprefix -l , $(DYNAMIC_LIBS))



# Standard set of derived targets
library: base \
	$(LIBRARIES) 
	@echo "***** `date` Done building library: $(COMPONENT_NAME) ******" 


binary: base \
	$(BINARIES)


# Subdirs should be to jump to subdirectories
#  standard form is of
#  $(MAKE) -C subdirectory_name $(MAKECMDGOALS) 
# If you have libraries that this component is dependent on 
# list them here! They will be build them first! 
subdirs:
	
clean:
	-rm -f $(BINARIES)
	-rm -f $(LIBRARIES)
	-rm -f $(ALL_OBJECTS)
	@echo "objs: --> $(OBJECTS)"
	@echo "sources: --> $(SOURCES)"
	@echo "headers: --> $(HEADERS)"
	@echo "includes: --> $(INCLUDES)"
	@echo "lib: --> $(LIBRARIES)" 
	@echo "bin: --> $(BINARIES)"
	@echo "libs: --> $(LD_LIBS)"
	@echo "lib paths: --> $(LIB_PATHS)"


module_dirs: build_dirs
