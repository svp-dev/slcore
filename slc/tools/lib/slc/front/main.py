import opts
import sys
from ..msg import log
from ..input.cm4 import process
from ..input.parse import parse_program
from chains import *
from ..visitors import PrintVisitor

def main():
    opts.parse_args()
    #print opts.resolved
    if opts.resolved.verbose: log("massaging input...")
    p = process(opts.inputs)
    if opts.resolved.verbose: log("parsing...")
    p = parse_program(p)
    if opts.resolved.verbose: log("processing...")
    run_chain(p)
    if opts.resolved.verbose: log("dumping output...")
    
    if opts.resolved.output == '-':
        out = sys.stdout
    else:
        out = file(opts.resolved.output, 'w')
    p.accept(PrintVisitor(stream = out))

if __name__ == "__main__":
    main()





