extern void t_main(void);

int main(void) {
  register void *pv __asm__("$1") = &t_main;
  long ret;
  __asm__ ("allocate %0, 1, 0, 0, 0\n\tcrei %0, 0(%1)" 
	   : "=r"(ret), "=r"(pv) : "1"(pv));
  return ret;
}
