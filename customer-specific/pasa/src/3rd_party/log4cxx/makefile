#
# Standard Module Makefile version 2.0
#
 
# Name of the componet (team/domain prefix '_' component name)
COMPONENT_NAME = log4cxx
 
ifndef PRJ_ROOT
export PRJ_ROOT = $(CURDIR)/../../../../
endif
include $(PRJ_ROOT)cfg/depends.mk
 
# Standard set of derived targets
library: 
	cp ./$(BUILD_CFG)/* $(SLIB_PATH)
	ln -sf $(SLIB_PATH)libexpat$(DEBUG_EXT).so.7 $(SLIB_PATH)libexpat$(DEBUG_EXT).so 
	ln -sf $(SLIB_PATH)libapr-1$(DEBUG_EXT).so $(SLIB_PATH)libapr-1$(DEBUG_EXT).so.5
	ln -sf $(SLIB_PATH)liblog4cxx$(DEBUG_EXT).so $(SLIB_PATH)liblog4cxx$(DEBUG_EXT).so.10
	ln -sf $(SLIB_PATH)libaprutil-1$(DEBUG_EXT).so $(SLIB_PATH)libaprutil-1$(DEBUG_EXT).so.5  
	@echo "***** `date` Done building library: $(COMPONENT_NAME) ******" 
               
clean:
	-rm -f $(BINARIES)
	-rm -f $(LIBRARIES)
	-rm -f $(ALL_OBJECTS)
	-rm -f $(SLIB_PATH)/*
	@echo "objs: --> $(OBJECTS)"
	@echo "sources: --> $(SOURCES)"
	@echo "headers: --> $(HEADERS)"
	@echo "includes: --> $(INCLUDES)"
	@echo "lib: --> $(LIBRARIES)" 
	@echo "bin: --> $(BINARIES)"
	@echo "libs: --> $(LD_LIBS)"
	@echo "lib paths: --> $(LIB_PATHS)" 
	
module_dirs: build_dirs