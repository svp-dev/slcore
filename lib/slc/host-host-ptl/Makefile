SLC = ../../../bin/slc
CXXFLAGS=-I../t-ptl

all: crt.o libslos.a

clean:
	rm -f *.a *.o


libslos.a: ptl_debug.o ptl_vars.o ptl_svp.o io.o
	$(AR) -crus $@ $^

io.o: ../svplib/io.sl
	$(SLC) -b ptl -c -o $@ $< 
