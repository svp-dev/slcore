#include <stdio.h>
#include <stdarg.h>

int snprintf(char *restrict str, size_t sz, char const* restrict fmt, ...)
{
	int ret;
	va_list ap;

	va_start(ap, fmt);
	ret = vsnprintf(str, sz, fmt, ap);
	va_end(ap);
	return (ret);
}
