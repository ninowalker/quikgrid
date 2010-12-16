
.PHONY: core python

all: core python

install: all core-install python-install

core:
	cd core && scons

core-install:
	cd core && scons install

python-install:
	cd python && python setup.py install
