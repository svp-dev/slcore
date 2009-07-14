#Livermore loops for SVP, written in SLC
# Makefile and sources by M.A.Hicks

COMP = slc
COMP_OPTS = -b ppp
RUNNER = slr
SOURCE_EXT = .sl
BIN_EXT = .mg
OUT_DIR = kernels
UTC_FLAG = -b utc0 -E -P
ASM_FLAG = -S
EXP_FILE = exp.tar.bz2
BENCHER = bash ./benchmarker.sh

#define all of the kernels to compile
KERNELS = kernel01 kernel02 kernel03 kernel04 kernel05 kernel06 kernel07 kernel08 kernel09 kernel10 kernel11 kernel12 kernel13

# as per the design, each of the livermore
# kernels must be separately compiled.

all: $(KERNELS)
		echo Built all.

$(OUT_DIR): 
	mkdir $(OUT_DIR)

$(KERNELS): $(OUT_DIR) livermore.slh
		$(COMP) $(COMP_OPTS) $(@)$(SOURCE_EXT) -o $(OUT_DIR)/$(@)$(BIN_EXT)
		$(COMP) $(COMP_OPTS) $(ASM_FLAG) $(@)$(SOURCE_EXT) -o $(OUT_DIR)/$(@).s
		$(COMP) $(UTC_FLAG)  $(@)$(SOURCE_EXT) -o $(OUT_DIR)/$(@).utc

.PHONY : clean		
clean:
	rm -rf $(OUT_DIR)

.PHONY : exp
exp: $(OUT_DIR)
	tar xfj $(EXP_FILE) -C $(OUT_DIR)

.PHONY : exprun
exprun: exp
	$(BENCHER) $(OUT_DIR)
