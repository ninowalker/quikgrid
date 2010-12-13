PYTHON_VERSION ?= 2.4

OS := $(shell uname)
CMAKE = $(shell which cmake) -DBUILD_SHARED_LIBS=OFF

ifeq ($(OS),Darwin)
CMAKE = $(shell which cmake) 
endif

.PHONY: core python python-install core-install

all: core 

install: core core-install python-install

core:
	cd core && $(CMAKE) . && make all

core-install:
	cd core && $(CMAKE) . && make all install

python: python-build python-install

python-build:
	cd core && $(CMAKE) -DBUILD_SHARED_LIBS=ON . && make

python-install:
	cd python && python$(PYTHON_VERSION) setup.py install
