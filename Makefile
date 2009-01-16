PYTHON_VERSION ?= 2.4

.PHONY: core python python-install core-install

all: core 

install: core core-install python-install

core:
	cd core && cmake -DBUILD_SHARED_LIBS=OFF . && make

core-install:
	cd core && cmake -DBUILD_SHARED_LIBS=OFF . && make all install

python: python-install

python-install:
	cd core && cmake -DBUILD_SHARED_LIBS=ON . && make all install
	cd python && python$(PYTHON_VERSION) setup.py install
