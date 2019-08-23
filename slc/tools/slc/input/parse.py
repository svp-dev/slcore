from ..msg import die
from ..ast import *

def unexpected(item):
      die("unexpected construct '%s'" % item.get('type','unknown'), item)


def parse_varuse(varuse, item):
      #print "parse varuse %x: item %x: %r" % (id(varuse), id(item), item)
      varuse.loc = item['loc']
      varuse.name = item['name'].strip()
      if item.has_key('rhs'):
            varuse.rhs = parse_block(item['rhs'])
      return varuse

def parse_create(item):
      c = Create(loc = item['loc'],
                 loc_end = item['loc_end'],
                 label = item['lbl'],
                 place = parse_block(item['place']),
                 start = parse_block(item['start']),
                 limit = parse_block(item['limit']),
                 step = parse_block(item['step']),
                 block = parse_block(item['block']),
                 extras = parse_extras(item['extras']),
                 sync_type = item['sync'],
                 fun = parse_block(item['fun']),
                 body = parse_block(item['body']))
                 
      for p in item['args']:
            c.args.append(parse_argparm(CreateArg(), 'arg', p))

      if 'fid' in item and item['fid']:
            c.fid_lvalue = parse_block(item['fid'])
      if 'result' in item and item['result']:
            c.result_lvalue = parse_block(item['result'])

      return c

def parse_indexdecl(item):
      return IndexDecl(loc = item['loc'],
                       indexname = item['name'].strip(),
                       extraarg = item['extra'])

def parse_spawndecl(item):
      return SpawnDecl(loc = item['loc'],
                       spawnname = item['name'].strip())

def parse_spawnsync(item):
      return SpawnSync(loc = item['loc'],
                       rhs = parse_block(item['rhs']))

def parse_scope(item):
      s = Scope(loc = item['loc'],
                loc_end = item['loc_end'])
      s += parse_block(item['body'])
      return s
  
def parse_attr(item):
      n = item['name'].strip()
      del item['type']
      del item['name']

      for k in item:
            item[k] = item[k].strip()

      return Attr(name = n, 
                  payload = item)

def parse_extras(items):
      if len(items) == 0:
            return None
      b = Extras()
      for item in items:
            if isinstance(item, dict):
                  t = item['type']
                  if t == 'attr': b += parse_attr(item)
                  else: unexpected(item)
            else:
                  assert isinstance(item, str)
                  # ignore strings
      if len(b) > 0:
            return b
      return None

def parse_block(items):
      if len(items) == 0:
            return None
      b = Block()
      #print "new block %x (len %d)" % (id(b), len(b))
      for item in items:
            #print "parse block %x (len %d): item %x: %r" % (id(b), len(b), id(item), item)
            if isinstance(item, dict):
                  t = item['type']
                  if t == 'indexdecl': b += parse_indexdecl(item)
                  elif t == 'getp': b += parse_varuse(GetP(), item)
                  elif t == 'setp': b += parse_varuse(SetP(), item)
                  elif t == 'geta': b += parse_varuse(GetA(), item)
                  elif t == 'seta': b += parse_varuse(SetA(), item)
                  elif t == 'create': b += parse_create(item)
                  elif t == 'break': b += parse_break(item)
                  elif t == 'end_thread': b += parse_end_thread(item)
                  elif t == 'decl_funptr': b += parse_funptrdecl(item)
                  elif t == 'scope': b += parse_scope(item)
                  elif t == 'spawndecl': b += parse_spawndecl(item)
                  elif t == 'spawnsync': b += parse_spawnsync(item)
                  else: unexpected(item)
            else: 
                  assert isinstance(item, str)
                  csp = item.strip(' \t')
                  if len(csp) > 0:
                        b += Opaque(item)
            #print "parse block %x: item %x -- END (len %d)" % (id(b), id(item), len(b))
      if len(b) > 0:
            return b
      return None

def parse_argparm(p, cat, item):
      #print "parse argparm %x: item %x: %r" % (id(p), id(item), item)
      t = item['type'].replace('_mutable','')
      if not t.endswith(cat):
            unexpected(item)
      p.loc = item['loc']
      p.type = item['type']
      p.ctype = CType(items = item['ctype'])
      p.name = item['name'].strip()
      if item.has_key('init'):
         p.init = parse_block(item['init'])
      return p            

def parse_break(item):
      return Break(loc = item['loc'])

def parse_end_thread(item):
      return EndThread(loc = item['loc'])

def parse_funptrdecl(item):
      d = FunDeclPtr(loc = item['loc'],
                     loc_end = item['loc_end'],
                     name = item['name'].strip(),
                     extras = parse_extras(item['extras']))
      for p in item['params']:
            d += parse_argparm(FunParm(), 'parm', p)
      return d

def parse_fundecl(item):
      d = FunDecl(loc = item['loc'], 
                  loc_end = item['loc_end'],
                  name = item['name'].strip(),
                  extras = parse_extras(item['extras']))
      for p in item['params']:
            d += parse_argparm(FunParm(), 'parm', p)
      return d

def parse_fundef(item):
      d = FunDef(loc = item['loc'], 
                 loc_end = item['loc_end'], 
                 name = item['name'].strip(),
                 extras = parse_extras(item['extras']),
                 body = parse_block(item['body']))
      for p in item['params']:
            d += parse_argparm(FunParm(), 'parm', p)
      return d

def parse_program(source):

      source = eval(source)

      p = Program()
      for item in source:
            if type(item) == type({}):
                  t = item['type']
                  if t == 'decl': p += parse_fundecl(item)
                  elif t == 'decl_funptr': p += parse_funptrdecl(item)
                  elif t == 'fundef': p += parse_fundef(item)
                  elif t == 'scope': p += parse_scope(item)
                  else: unexpected(item)
            else: p += Opaque(item)
      return p


__all__ = ['parse_program']
