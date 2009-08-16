extern void t_main(struct sl_famdata*);

void lib_main(void) {
  struct { } args;
  struct sl_famdata root;
  root.ix = root.be = 0;
  root.li = root.st = 1;
  root.f = t_main;
  root.a = &args;
  root.ex = 0;
  t_main(&root);
}
