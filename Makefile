SOURCES_NAME = ompchat-1.0
SPEC = ompchat.spec
RPM_SOURCES_DIR = $(shell rpm --eval "%{_sourcedir}")
RPM_SPECS_DIR = $(shell rpm --eval "%{_specdir}")

all:
	+$(MAKE) -C src

pack:
	cp -r src $(SOURCES_NAME)
	tar -zcvf $(SOURCES_NAME).tar.gz $(SOURCES_NAME)
	rm -r $(SOURCES_NAME)

rpm: pack
	mv $(SOURCES_NAME).tar.gz $(RPM_SOURCES_DIR)
	mv rpm/$(SPEC) $(RPM_SPECS_DIR)
	rpmbuild -ba $(RPM_SPECS_DIR)/$(SPEC)

clean:
	+$(MAKE) -C src clean
