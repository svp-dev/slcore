
int ffs(int v)
{
#ifdef __mt_freestanding__
    return __builtin_ffs(v);
#else
    int ret;
    unsigned x = v;

    if (!x) return 0;

    for (ret = 1; !(x & 1); ++ret)
        x >>= 1;
    return ret;
#endif
}
