HEADERS=$(wildcard *.h)
OBJECT_FILES = contour.o gridxtyp.o scatdata.o surfgrid.o xpand.o 
SHARED_LIB = libquikgrid.so
PREFIX=/usr/local

.PHONY: clean

all: lib demo

lib: $(OBJECT_FILES) qgcapi.o; 
	cc -shared -o $(SHARED_LIB) $(OBJECT_FILES) qgcapi.o -L. -Bsymbolic-functions -rdynamic -Wall -export-dynamic -lm -lstdc++
   
install: $(PREFIX)/include/quikgrid
	cp $(HEADERS) $(PREFIX)/include/quikgrid
	cp $(SHARED_LIB) $(PREFIX)/lib

$(PREFIX)/include/quikgrid:
	mkdir $(PREFIX)/include/quikgrid

demo: $(OBJECT_FILES) demo.o
	cc -lstdc++ -o demo $(OBJECT_FILES) demo.o 
 
%.o: %.cpp $(HEADERS)

clean:
	rm -f demo $(OBJECT_FILES) $(SHARED_LIB) qgcapi.o demo.o
