#Livermore loops for SVP, written in SLC
# Makefile and sources by M.A.Hicks

COMP = slc
COMP_OPTS = -b ppp
RUNNER = slr
SOURCE_EXT = .sl
BIN_EXT = .mg
OUT_DIR = bin
UTC_FLAG = -EE -P
ASM_FLAG = -S

#define all of the kernels to compile
KERNELS = kernel01 kernel02 kernel03 kernel04 kernel05 kernel06 kernel07 kernel08 kernel09 kernel10 kernel11 kernel12

# as per the design, each of the livermore
# kernels must be separately compiled.

all: $(KERNELS)
		echo Built all.

$(OUT_DIR): 
	mkdir $(OUT_DIR)

$(KERNELS): $(OUT_DIR) livermore.slh
		#make the MG-ALPHA executable
		$(COMP) $(COMP_OPTS) $(@)$(SOURCE_EXT) -o $(OUT_DIR)/$(@)$(BIN_EXT)
		#now make the asm
		$(COMP) $(COMP_OPTS) $(ASM_FLAG) $(@)$(SOURCE_EXT) -o $(OUT_DIR)/$(@).s
		#now make the utc
		$(COMP) $(COMP_OPTS) $(UTC_FLAG)  $(@)$(SOURCE_EXT) -o $(OUT_DIR)/$(@).utc
clean:
	rm -rf $(OUT_DIR)
