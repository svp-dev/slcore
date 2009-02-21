AS = mtalpha-linux-gnu-as
AR = mtalpha-linux-gnu-ar

all: libslos.a

clean:
	rm -f *.a *.o

libslos.a: div.o
	$(AR) -crus $@ $^

