# Standard Alpha GCC
ifndef CC_ALPHA
CC_ALPHA = alpha-linux-gnu-gcc -S
endif
CPP = gcc -E
CFLAGS = -O3 -W -Wall -fomit-frame-pointer -fno-exceptions -fno-unroll-loops \
	'-ffixed-$$9' '-ffixed-$$10' '-ffixed-$$11' '-ffixed-$$12' \
	'-ffixed-$$13' '-ffixed-$$14' '-ffixed-$$16' '-ffixed-$$17' \
	'-ffixed-$$18' '-ffixed-$$19' '-ffixed-$$20' '-ffixed-$$21' \
	'-fcall-used-$$26' -freorder-blocks \
	-msmall-text -msmall-data 
#	-mno-explicit-relocs
#	'-fcall-used-$$15' '-fcall-used-$$26' 

SOURCES = divide.c test-divide.c

DEPS = $(wildcard *.h)
.SUFFIXES: .c .o .s .us .xs .x .i

all: mtalpha-div.s 

clean:
	rm -f *.o *.s *.i *.us *.xs *.x

mtalpha-div.s: divide.s mtalpha-makediv.sh
	$(SHELL) mtalpha-makediv.sh $< >$@

%.i: %.c $(DEPS)
	$(CPP) -o $@ $<

%.xs: %.i
	$(CC_ALPHA) $(CFLAGS) -o $@ $<

%.xi: %.c $(DEPS)
	$(CPP) $< $@
	indent $@

%.us: %.xs
	$(SHELL) mtalpha-filter 0 <$< >$@

%.s: %.xs
	$(SHELL) mtalpha-filter 1 <$< >$@


