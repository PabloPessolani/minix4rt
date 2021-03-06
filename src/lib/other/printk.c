/*	printk() - kernel printf()			Author: Kees J. Bot
 *								15 Jan 1994
 */
#define nil 0
#include <stdarg.h>
#include <limits.h>

#define isdigit(c)	((unsigned) ((c) - '0') <  (unsigned) 10)

#if !__STDC__
/* Classic C stuff, ignore. */
void putk();
void printk(fmt) char *fmt;
#else

/* Printk() uses putk() to print characters. */
void putk(int c);

void printk(const char *fmt, ...)
#endif
{
	int c;
	enum { LEFT, RIGHT } adjust;
	enum { LONG, INT } intsize;
	int fill;
	int width, max, len, base;
	static char X2C_tab[]= "0123456789ABCDEF";
	static char x2c_tab[]= "0123456789abcdef";
	char *x2c;
	char *p;
	long i;
	unsigned long u;
	char temp[8 * sizeof(long) / 3 + 2];

	va_list argp;

	va_start(argp, fmt);

	while ((c= *fmt++) != 0) {
		if (c != '%') {
			/* Ordinary character. */
			putk(c);
			continue;
		}

		/* Format specifier of the form:
		 *	%[adjust][fill][width][.max]keys
		 */
		c= *fmt++;

		adjust= RIGHT;
		if (c == '-') {
			adjust= LEFT;
			c= *fmt++;
		}

		fill= ' ';
		if (c == '0') {
			fill= '0';
			c= *fmt++;
		}

		width= 0;
		if (c == '*') {
			/* Width is specified as an argument, e.g. %*d. */
			width= va_arg(argp, int);
			c= *fmt++;
		} else
		if (isdigit(c)) {
			/* A number tells the width, e.g. %10d. */
			do {
				width= width * 10 + (c - '0');
			} while (isdigit(c= *fmt++));
		}

		max= INT_MAX;
		if (c == '.') {
			/* Max field length coming up. */
			if ((c= *fmt++) == '*') {
				max= va_arg(argp, int);
				c= *fmt++;
			} else
			if (isdigit(c)) {
				max= 0;
				do {
					max= max * 10 + (c - '0');
				} while (isdigit(c= *fmt++));
			}
		}

		/* Set a few flags to the default. */
		x2c= x2c_tab;
		i= 0;
		base= 10;
		intsize= INT;
		if (c == 'l' || c == 'L') {
			/* "Long" key, e.g. %ld. */
			intsize= LONG;
			c= *fmt++;
		}
		if (c == 0) break;

		switch (c) {
			/* Decimal.  Note that %D is treated as %ld. */
		case 'D':
			intsize= LONG;
		case 'd':
			i= intsize == LONG ? va_arg(argp, long)
						: va_arg(argp, int);
			u= i < 0 ? -i : i;
			goto int2ascii;

			/* Octal. */
		case 'O':
			intsize= LONG;
		case 'o':
			base= 010;
			goto getint;

			/* Hexadecimal.  %X prints upper case A-F, not %lx. */
		case 'X':
			x2c= X2C_tab;
		case 'x':
			base= 0x10;
			goto getint;

			/* Unsigned decimal. */
		case 'U':
			intsize= LONG;
		case 'u':
		getint:
			u= intsize == LONG ? va_arg(argp, unsigned long)
						: va_arg(argp, unsigned int);
		int2ascii:
			p= temp + sizeof(temp)-1;
			*p= 0;
			do {
				*--p= x2c[u % base];
			} while ((u /= base) > 0);
			goto string_length;

			/* A character. */
		case 'c':
			p= temp;
			*p= va_arg(argp, int);
			len= 1;
			goto string_print;

			/* Simply a percent. */
		case '%':
			p= temp;
			*p= '%';
			len= 1;
			goto string_print;

			/* A string.  The other cases will join in here. */
		case 's':
			p= va_arg(argp, char *);

		string_length:
			for (len= 0; p[len] != 0 && len < max; len++) {}

		string_print:
			width -= len;
			if (i < 0) width--;
			if (fill == '0' && i < 0) putk('-');
			if (adjust == RIGHT) {
				while (width > 0) { putk(fill); width--; }
			}
			if (fill == ' ' && i < 0) putk('-');
			while (len > 0) { putk((unsigned char) *p++); len--; }
			while (width > 0) { putk(fill); width--; }
			break;

			/* Unrecognized format key, echo it back. */
		default:
			putk('%');
			putk(c);
		}
	}

	/* Mark the end with a null (should be something else, like -1). */
	putk(0);
	va_end(argp);
}


/*
 * $PchHeader: /mount/hd2/minix/lib/minix/other/RCS/printk.c,v 1.2 1994/09/07 18:45:05 philip Exp $
 */
