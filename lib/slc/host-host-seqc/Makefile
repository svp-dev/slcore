SLC = ../../../bin/slc
CXXFLAGS=-I../t-seqc

all: crt.o libslos.a

clean:
	rm -f *.a *.o


libslos.a: io.o
	$(AR) -crus $@ $^

io.o: ../svplib/io.sl
	$(SLC) -b seqc -c -o $@ $< 
