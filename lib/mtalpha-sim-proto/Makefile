AS = mtalpha-linux-gnu-as
AR = mtalpha-linux-gnu-ar

all: libslos.a crt.o

clean:
	rm -f *.a *.o

libslos.a: div.o
	$(AR) -crus $@ $^

