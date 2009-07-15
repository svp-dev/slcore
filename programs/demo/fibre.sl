m4_include(fibre.slh)
m4_include(svp/iomacros.slh)
m4_include(cstddef.slh)

// SLT_RUN: 
// SLT_RUN: -f TEST.d1
// SLT_RUN: -f TEST.d2
// SLT_RUN: -f TEST.d3

sl_def(t_main, void)
{
  size_t i, j, nitems;
  int tag;

  for (i = 0; (tag = fibre_tag(i)) != -1; ++i) {
    printf("%d: tag = %d rank = %d\n", i, tag, fibre_rank(i));
    printf("%d: shape = { ", i);
    for (j = 0; j < fibre_rank(i); ++j)
      printf("%d ", fibre_shape(i)[j]);
    printf("}\n");

    nitems = 1;
    for (j = 0; j < fibre_rank(i); ++j)
      nitems *= fibre_shape(i)[j];
    printf("%d: values = { ", i);
    for (j = 0; j < nitems; ++j) {
      switch(tag) {
      case 0: printf("%u ", *((unsigned long*)fibre_data(i)+j)); break;
      case 1: printf("%d ", *((long*)fibre_data(i)+j)); break;
      case 2: printf("%f ", *((double*)fibre_data(i)+j)); break;
      default: printf("? "); break;
      }
    }
    printf("}\n");
  }
  printf("number of Fibre items: %d\n", i);

}
sl_enddef
