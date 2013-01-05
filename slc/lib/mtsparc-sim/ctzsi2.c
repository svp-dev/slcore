int __ctzsi2(unsigned long i)
{
    int b;
    if (i == 0)
        return 0;
    for (b = 0; !(i & 1); ++b)
        i = (unsigned long)i >> 1;
    return b;
}

