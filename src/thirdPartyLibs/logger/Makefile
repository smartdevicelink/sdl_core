
PKGNAME     :=log4cplus
PKGVER      :=1.0.4

SOURCE_DIR  :=$(PKGNAME)-$(PKGVER)
INSTALL_DIR :=  $(PWD)/bin

LIB_NAME := liblog4cplus.a

LIB_INST_PATH := $(INSTALL_DIR)/$(LIB_NAME)

LIBS = $(LIB_INST_PATH)

###############################################################################
# Rules

debug: all

release: all

all: install

$(SOURCE_DIR)/Makefile:
	cd $(SOURCE_DIR); automake --add-missing; ./configure --prefix=$(INSTALL_DIR) --enable-static

# Target will build package and create file 'build' to remember the time of build -
# this prevents the build of package every time we run make
build: $(SOURCE_DIR)/Makefile 
	make -C $(SOURCE_DIR)
	touch build

$(INSTALL_DIR)/liblog4cplus.a: build
	mkdir -p $(INSTALL_DIR)
	cp $(SOURCE_DIR)/src/.libs/liblog4cplus.a $(INSTALL_DIR)/
#	$(MAKE) -C $(SOURCE_DIR) install

# Target will install package and create file 'install' to remember the time of install -
# this prevents the isntall of package every time we run make
install: $(LIBS)
	touch install

clean:

.PHONY: all debug release download check_download clean 3rdp_clean
