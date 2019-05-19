SOURCE_TAR = ompchat-1.0.tar.gz
SPEC = ompchat.spec
RPM_SOURCES_DIR = $(shell rpm --eval "%{_sourcedir}")
RPM_SPECS_DIR = $(shell rpm --eval "%{_specdir}")

all:
	+$(MAKE) -C src

pack:
	tar -zcvf $(SOURCE_TAR) src

rpm: pack
	mv $(SOURCE_TAR) $(RPM_SOURCES_DIR)
	mv rpm/$(SPEC) $(RPM_SPECS_DIR)
	rpmbuild -ba $(RPM_SPECS_DIR)/$(SPEC)

clean:
	+$(MAKE) -C src clean
