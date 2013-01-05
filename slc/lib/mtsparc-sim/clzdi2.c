int __clzdi2(unsigned long long i)
{
    int b;
    if (i == 0)
        return 0;
    for (b = 0; !(i & (1ULL << (sizeof(i)*8 - 1))); ++b)
        i = (unsigned long long)i << 1;
    return b;
}

