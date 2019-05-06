export OUTDIR=$(shell pwd)/build
export OBJ_LIBS = obj-libs
export INCDIR=$(shell pwd)/include
#----------------------------------------------------------------------------
# Export following paths to customize
#----------------------------------------------------------------------------
ifndef OPT_FUJITSU_INC_DIR
OPT_FUJITSU_INC_DIR = /opt/morpho/include/printer
endif

ifndef OPT_FUJITSU_LIB_DIR
OPT_FUJITSU_LIB_DIR = /opt/morpho/lib/printer
endif

ifndef OPT_ETC_LD_DIR
OPT_ETC_LD_DIR = /opt/morpho/etc/ld.so.conf.d
endif
#----------------------------------------------------------------------------
# Library Details
#----------------------------------------------------------------------------

LIB_NAME := fujitsu_library
LIB_MAJOR := 1
LIB_MINOR := 0
LIB_RELEASE := 0
VERSION := $(LIB_MAJOR).$(LIB_MINOR).$(LIB_RELEASE)

LIB_DIR = $(OUTDIR)/lib
SRC = $(wildcard src/*.cpp)
OBJ_FILES = $(OUTDIR)/$(OBJ_LIBS)/
OBJ = $(addprefix  $(OBJ_FILES),$(notdir $(SRC:.cpp=.o)))
#----------------------------------------------------------------------------
# RPM INSTALLATION PACKAGE
#----------------------------------------------------------------------------
RPM_VERSION_NO	= $(VERSION)

RPM_FUJITSU_LIB_SPEC_FILE = $(OUTDIR)/fujitsu-library.spec
FUJITSU_LIB_RPM_NAME 	= fujitsu-library
FUJITSU_LIB_RPM_TAR	= fujitsu-library.tar.gz
FUJITSU_LIB_CONF_FILE	= fujitsu.conf
#----------------------------------------------------------------------------
# Include Path
#----------------------------------------------------------------------------
INCLUDE_PATH +=-I ./include
INCLUDE_PATH +=-I /usr/include/libusb-1.0

#----------------------------------------------------------------------------
# Library Path
#----------------------------------------------------------------------------
LIBS = -ludev
LIBS += -lusb-1.0

#----------------------------------------------------------------------------
# Macros/Defines
#----------------------------------------------------------------------------
#MACRO+=-DDEBUG
MACRO+=-DBARCODE_FUJITSU_LIB_VERSION=\"$(VERSION)\"

#----------------------------------------------------------------------------
# CFLAGS
#----------------------------------------------------------------------------
CFLAGS +=-c -DHAVE_NETINET_IN_H -g -fPIC -std=c++0x -O0
LDFLAGS := -shared

QUIET_COMPILE  =@echo Compiling $< ...
QUIET_PREFIX   =@

all: buildrepo lib$(LIB_NAME).so.$(VERSION) rpm-build

lib$(LIB_NAME).so.$(VERSION): $(OBJ)
	mkdir -p $(LIB_DIR)
	$(QUIET_PREFIX) $(CXX) $(MACRO) $(LDFLAGS) $(OBJ) -o $(LIB_DIR)/lib$(LIB_NAME).so.$(VERSION) $(LIBS)
	$(QUIET_PREFIX) cd $(LIB_DIR) && ln -fs lib$(LIB_NAME).so.$(VERSION) lib$(LIB_NAME).so

$(OBJ_FILES)%.o:	src/%.cpp
	$(QUIET_COMPILE)
	$(QUIET_PREFIX) $(CXX) $(MACRO) $(CFLAGS) $(INCLUDE_PATH) $< -o $@ -Wall -Wno-unused-result -Wno-sign-compare -Wno-unused-but-set-variable -O3

buildrepo:
	$(call make-repo)
	# Create obj directory structure
define make-repo
        mkdir -p $(OBJ_FILES)
endef

rpm-build:
	@echo "==================================================================================="
	@echo "Preparing RPM Packages for Fujitsu Library ..."
	@echo "==================================================================================="

        # ********************************************************************
        # Make a conf file for library.
        # ********************************************************************
	echo	"$(OPT_FUJITSU_LIB_DIR)" > $(OUTDIR)/$(FUJITSU_LIB_CONF_FILE)

        #Make tar file for installation package of Fujitsu Library
	cd $(OUTDIR) && tar -zcvf $(FUJITSU_LIB_RPM_TAR) $(INCDIR) $(LIB_DIR) $(OUTDIR)/$(FUJITSU_LIB_CONF_FILE)

        # Set up RPM directory structure
        
	rpmdev-setuptree

        # Prepare .spec file
	echo  	"Name:  $(FUJITSU_LIB_RPM_NAME)" > $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo  	"Version:  $(RPM_VERSION_NO)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo  	"Release:   1%{?dist}" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo -e "Summary:    Creating rpm packages for Fujitsu Library\n" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo  	"License:    Morpho" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo 	"AutoReqProv:    no" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo 	"requires: libusbx systemd-devel" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo -e "Source0:    $(FUJITSU_LIB_RPM_TAR)\n" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo  	"BuildArch:    noarch" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo -e "%define _binaries_in_noarch_packages_terminate_build   0\n" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo  	"%description" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo 	"Rpm package contains library files of Fujitsu Library module." >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo	"%package devel" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo 	"requires: $(FUJITSU_LIB_RPM_NAME)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo -e "summary: Developement package for Fujitsu Library." >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo  	"%description devel" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo 	"Rpm package contains library and header files of Fujitsu Library." >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo 	"%prep" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo -e "%setup -q -c\n" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo  	"%install" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo 	"mkdir -p -m 777 %{buildroot}$(OPT_FUJITSU_INC_DIR)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo 	"mkdir -p -m 777 %{buildroot}$(OPT_ETC_LD_DIR)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo -e "mkdir -p -m 777 %{buildroot}$(OPT_FUJITSU_LIB_DIR)\n" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo    "cp -r $(INCDIR)/*.h %{buildroot}$(OPT_FUJITSU_INC_DIR)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo    "cp $(OUTDIR)/$(FUJITSU_LIB_CONF_FILE) %{buildroot}$(OPT_ETC_LD_DIR)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo    "cp -r $(LIB_DIR)/* %{buildroot}$(OPT_FUJITSU_LIB_DIR)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo 	"%files" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo    "$(OPT_ETC_LD_DIR)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo    "$(OPT_FUJITSU_LIB_DIR)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo 	"%files devel" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo -e "$(OPT_FUJITSU_INC_DIR)\n" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo 	"%post" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

# 	Remove files/directory on .rpm uninstallation
	echo 	"%postun" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo 	"rm -rf $(OPT_FUJITSU_LIB_DIR)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo 	"if [ `ls -1 $(OPT_ETC_LD_DIR)/$(FUJITSU_LIB_CONF_FILE) 2>/dev/null | wc -l ` -gt 0 ]; then rm $(OPT_ETC_LD_DIR)/$(FUJITSU_LIB_CONF_FILE); fi;" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo 	"%postun devel" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo 	"rm -rf $(OPT_FUJITSU_INC_DIR)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	echo 	"%changelog" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo 	"* Tue Nov 14 2017 The Fujitsu Library RPM Package $(RPM_VERSION_NO)" >> $(RPM_FUJITSU_LIB_SPEC_FILE)
	echo 	"- Initial version of the package" >> $(RPM_FUJITSU_LIB_SPEC_FILE)

	cp $(OUTDIR)/$(FUJITSU_LIB_RPM_TAR) ~/rpmbuild/SOURCES
	cp $(RPM_FUJITSU_LIB_SPEC_FILE) ~/rpmbuild/SPECS
	cd ~/rpmbuild/SPECS && rpmbuild -ba $(RPM_FUJITSU_LIB_SPEC_FILE)
	cp ~/rpmbuild/RPMS/noarch/$(FUJITSU_LIB_RPM_NAME)*.rpm $(OUTDIR)
	if [ -a $(OUTDIR)/$(FUJITSU_LIB_RPM_TAR) ]; then rm $(OUTDIR)/$(FUJITSU_LIB_RPM_TAR); fi;

	if [ -a $(RPM_FUJITSU_LIB_SPEC_FILE) ]; then rm $(RPM_FUJITSU_LIB_SPEC_FILE); fi;

	if [ -a $(OUTDIR)/$(FUJITSU_LIB_CONF_FILE) ]; then rm $(OUTDIR)/$(FUJITSU_LIB_CONF_FILE); fi;

rpm-build-clean:
	@echo "==================================================================================="
	@echo "Cleaning fujitsu-library package..."
	@echo "==================================================================================="
	if [ `ls -1 $(OUTDIR)/$(FUJITSU_LIB_RPM_NAME)*.rpm 2>/dev/null | wc -l ` -gt 0 ]; then rm $(OUTDIR)/$(FUJITSU_LIB_RPM_NAME)*.rpm; fi;
	if [ `ls -1 ~/rpmbuild/RPMS/noarch/$(FUJITSU_LIB_RPM_NAME)*.rpm 2>/dev/null | wc -l ` -gt 0 ]; then rm ~/rpmbuild/RPMS/noarch/$(FUJITSU_LIB_RPM_NAME)*.rpm; fi;

clean:
	make rpm-build-clean
	rm -rf build

.PHONY: clean rpm-build rpm-build-clean
