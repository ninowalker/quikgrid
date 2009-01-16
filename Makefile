PYTHON_VERSION ?= 2.4

OS := $(shell uname)
CMAKE = $(shell which cmake)
 
ifeq ($(OS),Darwin)
ifeq ($(CMAKE),)
	CMAKE = /sw/bin/cmake
endif
endif

.PHONY: core python python-install core-install

all: core 

install: core core-install python-install

core:
	cd core && $(CMAKE) -DBUILD_SHARED_LIBS=OFF . && make all

core-install:
	cd core && $(CMAKE) -DBUILD_SHARED_LIBS=OFF . && make all install

python: python-install

python-install:
	cd core && $(CMAKE) -DBUILD_SHARED_LIBS=ON . && make all install
	cd python && python$(PYTHON_VERSION) setup.py install
