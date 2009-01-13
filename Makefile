PYTHON_VERSION ?= 2.4

.PHONY: core python-install core-install

all: core 

install: core core-install python-install

core:
	$(MAKE) -C core

core-install:
	$(MAKE) -C core install

python-install:
	cd python && python$(PYTHON_VERSION) setup.py install
	
