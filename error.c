#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "main.h"
#include "error.h"

void vwarning(char *fmt, va_list ap)
{
	fprintf(stderr, "%s: ", progname);
	vfprintf(stderr, fmt, ap);
	fputs("\n", stderr);
}

void warning(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vwarning(fmt, ap);
}

void vfatal(char *fmt, va_list ap)
{
	vwarning(fmt, ap);
	fail();
}

void fatal(char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfatal(fmt, ap);
}
