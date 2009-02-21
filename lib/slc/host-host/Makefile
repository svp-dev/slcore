

all: libslos.a libutc-ptl.a libseqc.a

clean:
	rm -f *.a *.o

CXXFLAGS=-I../t-ptl

libslos.a: entry.o
	$(AR) -crus $@ $^

libseqc.a: seqc-main.o
	$(AR) -crus $@ $^

libutc-ptl.a: ptl_debug.o ptl_vars.o ptl_svp.o
	$(AR) -crus $@ $^
