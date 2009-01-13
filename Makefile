objects = contour.o gridxtyp.o scatdata.o surfgrid.o xpand.o 
sharedlib = libquikgrid.so

all: lib demo

lib: $(objects) qgcapi.o; cc -shared -o $(sharedlib) qgcapi.o $(objects) -L. -Bsymbolic-functions -rdynamic -Wall -export-dynamic -lm -lstdc++
   
demo : $(objects) demo.o; cc -lstdc++ -o demo $(objects) demo.o 
 
demo.o :  demo.cpp scatdata.h surfgrid.h xpand.h contour.h 
contour.o : contour.cpp  surfgrid.h
gridxtyp.o : gridxtyp.cpp gridxtyp.h 
scatdata.o : scatdata.cpp scatdata.h 
surfgrid.o : surfgrid.cpp surfgrid.h
xpand.o    : xpand.cpp scatdata.h surfgrid.h xpand.h gridxtyp.h 
qgcapi.o   : qgcapi.cpp

.PHONY : clean
clean   : ; rm -f demo $(objects) $(sharedlib) qgcapi.o demo.o
