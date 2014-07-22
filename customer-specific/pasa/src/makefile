#
# Standard Product Makefile version 1.0
# 
#

ifndef PRJ_ROOT
export PRJ_ROOT = $(CURDIR)/../
endif

include $(PRJ_ROOT)cfg/depends.mk

COMPONENT_NAME = SmartDeviceLink

DEFS += \
	-Wc,-std=gnu++0x \
	-DEXTENDED_POLICY \
	-DENABLE_LOG \

all:
	@echo "***************************************************************************"
	@echo "Make $@ $(COMPONENT_NAME) beginning..."
	@echo "***************************************************************************"
	$(MAKE) bits
	@echo "***************************************************************************"
	@echo "Make $@ $(COMPONENT_NAME) finished."
	@echo "***************************************************************************"

bits: banner library binary

# All modules to be included, and local customizations
# Using $@ as a target to create a short and maintainable file
# In this context no variant features are needed from each
# spawn.  That may change in the future and $@ can be changed to build a
# specific feature. 
library: 
	@echo "***************************************************************************"
	@echo "Make $@ $(COMPONENT_NAME) beginning..."
	@echo "***************************************************************************"
	$(MAKE) -C 3rd_party-static/encryption/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C 3rd_party-static/jsoncpp/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C 3rd_party-static/MessageBroker/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C 3rd_party/log4cxx OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/interfaces OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/utils/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/media_manager/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/smart_objects/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/config_profile/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/request_watchdog/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/hmi_message_handler/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/formatters/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/connection_handler/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/protocol/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/protocol_handler/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/transport_manager/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/policy/src/policy/qdb_wrapper/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/policy/src/policy/policy_table/table_struct_ext OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/policy/src/policy/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/application_manager/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/resumption/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	@echo "***************************************************************************"
	@echo "Make $@ $(COMPONENT_NAME) finished."
	@echo "***************************************************************************"

.NOTPARALLEL:

binary:
	@echo "***************************************************************************"
	@echo "Make $@ $(COMPONENT_NAME) beginning..."
	@echo "***************************************************************************"
	$(MAKE) -C appMain OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	@echo "***************************************************************************"
	@echo "Make $@ $(COMPONENT_NAME) finished."
	@echo "***************************************************************************"

test: library

examples:

clean lint metrics: 
	@echo "***************************************************************************"
	@echo "Make $@ $(COMPONENT_NAME) beginning..."
	@echo "***************************************************************************"
	$(MAKE) -C 3rd_party-static/encryption/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C 3rd_party-static/jsoncpp/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C 3rd_party-static/MessageBroker/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C 3rd_party/log4cxx OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/interfaces OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/utils/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/smart_objects/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/config_profile/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/request_watchdog/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/hmi_message_handler/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/formatters/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/connection_handler/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/protocol_handler/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/protocol/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/transport_manager/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/policy/src/policy/qdb_wrapper/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/policy/src/policy/policy_table/table_struct_ext OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/policy/src/policy/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/application_manager/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/media_manager/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	$(MAKE) -C components/resumption/src OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@	
	$(MAKE) -C appMain OPTIMIZATION=NONE TARGET_FLG=-Vgcc_ntoarmv7le_gpp $@
	@echo "***************************************************************************"
	@echo "Make $@ $(COMPONENT_NAME) finished."
	@echo "***************************************************************************"

clobber:
	rm -fr $(BUILD_ROOTS)
