#
# Standard Dependency Makefile Version 2.0
#
# Dependency file dictates not only what other modules the project is dependent on, but 
# also where to get that dependent element which logically includes versioning or
# baselining information


# Begin User Build Location Definition
#  This is the prefixing information which dictates the versioning of dependant projects
#  Typically path statements will include some set of versioning information.

# The Repository Root is the base location of where a build is put in context
#  By default this is up a directory from the project root.  This is based on
#  the default local build where a component where a project is started to build is 
#  on the same level as other components in the repository.  Otherwise a value can
#  be forced by setting the value before operating on the configuration file.
#
#  Example would be to setting the Repository Root to a baseline label name
#  where the contents of the baseline lable are exported to the filesystem.
ifndef REPOSITORY_ROOT
export REPOSITORY_ROOT = $(PRJ_ROOT)../
endif

# The Team Root is akin to the Repository Root.  It sets the location of
#  the root for other components in the same stream.  It is very rare to 
#  modify the value of the team root.  However it is entirely possible that
#  the repository root and the team root are pointed to the same location.
# By default the Team Root is set to the workspace location of your project
ifndef TEAM_ROOT
export TEAM_ROOT = $(PRJ_ROOT)../
endif

# Setup the include paths for project functional dependencies by component.
# This is a list of include paths that should be done with relative locations
# that are relative to the repository root for all elements external to the 
# component.  All Dependency includes should be based on the $(REPOSITORY_ROOT)
# all local (team/local) dependencies should be handled in the main makefile.
# Note: Internal include locations should be simply in the makefile directly using
# the './' as the notation for referencing location.
# EXCEPTION:  The first include is a TEAM_ROOT for the locally "globally" visible
# module(s) of the Domain.  (When doing the product release this is overridden)
DEPENDS_INCLUDES = \
	$(CC_IFLAG)$(REPOSITORY_ROOT)NativeServices/inc \
	$(CC_IFLAG)$(REPOSITORY_ROOT)NativeServices/inc/ip1 \
	$(CC_IFLAG)$(REPOSITORY_ROOT)NativeServices/inc/framework \
	$(CC_IFLAG)$(REPOSITORY_ROOT)NativeServices/inc/services \
	$(CC_IFLAG)$(REPOSITORY_ROOT)NativeServices/inc/utility \
	$(CC_IFLAG)$(REPOSITORY_ROOT)NativeServices/inc/native \
	$(CC_IFLAG)$(TEAM_ROOT)AppLink/inc \
	

# Setup Library paths.  Same rules as above
DEPENDS_LIB_PATHS = \
	$(TEAM_ROOT)SmartDeviceLink/lib/$(TARGET)$(BUILD_CFG) \
	
# Setup library definintions.  Same rules
include $(TEAM_ROOT)SmartDeviceLink/cfg/smartdevicelinklibraries.mk

# Setup the include location for the tools configuration.  This sets the 
# standard tool definition locations.  By default these are local, but
# long term will be a standard component for a project to include.  This
# definition is inherited in cases that this is a recursive call.

# Set tools location
ifndef TOOLS_CFG
TOOLS_CFG = $(TEAM_ROOT)SmartDeviceLink/cfg/
endif

# Include the configuration
include $(TOOLS_CFG)config.mk
