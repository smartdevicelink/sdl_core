INCLUDE_DIRS = -I./include -I../../thirdPartyLibs/jsoncpp/include
LIB_INCLUDE_DIRS = ../../thirdPartyLibs/jsoncpp/bin/
CC=g++
CFLAGS=$(INCLUDE_DIRS) -L$(LIB_INCLUDE_DIRS)
SRC_DIR = ./src
compile_flags      := -Wall -MD -pipe
link_flags            := -s -pipe
libraries               := -ljsonCpp
build_flags := -O2
#relative_include_dirs   := $(addprefix ../, $(root_include_dir))
#
objects := $(patsubst ../../%, %, $(wildcard $(addsuffix /*.c*, $(SRC_DIR))))
objects := $(objects:.cpp=.o)

OUTPUT := json_handler

all: $(OUTPUT)

$(OUTPUT) : $(OBJ_DIR) $(objects)
	$(CC) -o $@ $(objects) 
OBJ_DIR :
	mkdir -p obj

VPATH := ../
 
%.o : %.cpp
	g++ -o $@ -c $< $(compile_flags) $(build_flags) $(CFLAGS)

.PHONY : clean

clean : 
	rm -rf obj

include $(wildcard $(addsuffix /*.d, $(OBJ_DIR)))
