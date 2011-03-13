from common import *

_filter_begin = [reader, lexer, splitsemi, parser]
_filter_stages = []
_filter_end = [printer]
                 


from ...common.asmproc import enhance

def filter(*args):
	return enhance.filter(_filter_begin, _filter_stages, _filter_end, *args)

