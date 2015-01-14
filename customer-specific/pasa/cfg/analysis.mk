# QCC arm COMPILER SETTINGS
#  Standard form of tool.mk file configured for qnx vmware

#Add qnx binary tools to path
PATH := $(QNX_HOME)/usr/bin/;$(PATH)

# include the compiler definitions in which we are linting against
include $(PRJ_ROOT)/cfg/qnx_armv7.mk

# Tools and Options
CODE_COUNT = surveyor
LINT = lint-nt
CC_IFLAG = -i

CC_DEFS = -D__LANGUAGE_C -D_LANGUAGE_C -D__QNX__ -D__QNXNTO__ \
	-D__GNUC__=4 -D__GNUC_MINOR__=4 -D__GNUC_PATCHLEVEL__=2 \
	-D__unix__ -D__unix -D__ELF__ -D__ARM__ -D__arm__ \
	-D__ARM_ARCH_4__ -D__APCS_32__ -D__LITTLEENDIAN__ -D__ARMEL__ -D_MUDFLAPTH

CPP_DEFS = -D__cplusplus -D__QNX__ -D__QNXNTO__ \
	-D__GNUC__=4 -D__GNUC_MINOR__=4 -D__GNUC_PATCHLEVEL__=2 \
	-D__NO_INLINE__ -D__DEPRECATED -D__EXCEPTIONS \
	-D__unix__ -D__unix -D__ELF__ -D__ARM__ -D__arm__ \
	-D__ARM_ARCH_4__ -D__APCS_32__ -D__LITTLEENDIAN__ -D__ARMEL__ -D_MUDFLAPTH 

LINT_C_INCLUDES = \
	$(CC_IFLAG)C:/lint.qnx \
	$(CC_IFLAG)$(QNX_TARGET)/usr/include \

LINT_CPP_INCLUDES = \
	$(CC_IFLAG)C:/lint.qnx \
	$(CC_IFLAG)$(QNX_TARGET)/usr/include \
	$(CC_IFLAG)$(QNX_HOST)/usr/lib/gcc/arm-unknown-nto-qnx6.5.0/4.4.2/include \
	$(CC_IFLAG)$(QNX_TARGET)/usr/include/cpp/c \
	$(CC_IFLAG)$(QNX_TARGET)/usr/include/cpp \

LINT_SOURCES := $(shell find .. -iname "*.c" -or -iname "*.cc" -or -iname "*.cpp")
LINT_IMPLIED_DIRS := $(addprefix $(CC_IFLAG), $(sort $(dir $(shell find .. -iname "*.c" -or -iname "*.cc" -or -iname "*.cpp"))))

# Metric Rule 
metrics:
	@echo --------------------
	@echo Start metrics on $(COMPONENT_NAME)
	@echo --------------------
	-@mkdir -p $(PRJ_ROOT)/ana/metrics
	-@$(CODE_COUNT) -detailed -progress -out $(PRJ_ROOT)/ana/metrics/metrics_$(COMPONENT_NAME).csv ..

# Static Analysis Rule
lint:
	@echo --------------------
	@echo Performing Lint on $(COMPONENT_NAME)
	@echo --------------------
	@echo  includes the following files:
	@echo $(LINT_SOURCES)
	@echo --------------------
	-@c:/lint/$(LINT) $(PRJ_ROOT)cfg/options.lnt C:/lint.qnx/co-gcc.lnt $(CC_DEFS) $(LINT_C_INCLUDES) $(INCLUDES) $(LINT_IMPLIED_DIRS) $(filter %.c ,$(LINT_SOURCES))
	-@c:/lint/$(LINT) $(PRJ_ROOT)cfg/options.lnt C:/lint.qnx/co-gcc.lnt $(CPP_DEFS) $(LINT_CPP_INCLUDES) $(INCLUDES) $(LINT_IMPLIED_DIRS) $(filter %.cpp %.cc %.cxx,$(LINT_SOURCES) )