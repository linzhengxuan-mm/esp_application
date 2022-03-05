#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <float.h>
#include <ctype.h>


#define FLOATING_POINT

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif
#define	MAXEXP		20
#define	MAXFRACT	10

#define	BUF	(MAXEXP+MAXFRACT+3)	/* 3 = sign + decimal point + NUL */
/* An upper bound for how long a long prints in decimal.  4 / 13 approximates
   log (2).  Add one char for roundoff compensation and one for the sign.  */
#define MAX_LONG_LEN ((CHAR_BIT * sizeof (long)  - 1) * 4 / 13 + 2)


/*
 * Flags used during conversion.
 */

#define	LONG		0x01	/* l: long or double */
#define	LONGDBL		0x02	/* L: long double or long long */
#define	SHORT		0x04	/* h: short */
#define	SUPPRESS	0x10	/* suppress assignment */
#define	POINTER		0x20	/* weird %p pointer (`fake hex') */
#define	NOSKIP		0x40	/* do not skip blanks */

/*
 * The following are used in numeric conversions only:
 * SIGNOK, NDIGITS, DPTOK, and EXPOK are for floating point;
 * SIGNOK, NDIGITS, PFXOK, and NZDIGITS are for integral.
 */

#define	SIGNOK		0x80	/* +/- is (still) legal */
#define	NDIGITS		0x100	/* no digits detected */

#define	DPTOK		0x200	/* (float) decimal point is still legal */
#define	EXPOK		0x400	/* (float) exponent (e+3, etc) still legal */

#define	PFXOK		0x200	/* 0x prefix is (still) legal */
#define	NZDIGITS	0x400	/* no zero digits detected */
#define	NNZDIGITS	0x800	/* no non-zero digits detected */

#define	VECTOR		0x2000	/* v: vector */
#define	FIXEDPOINT	0x4000	/* r/R: fixed-point */
#define	SIGNED  	0x8000	/* r: signed fixed-point */

/*
 * Conversion types.
 */

#define	CT_CHAR		0	/* %c conversion */
#define	CT_CCL		1	/* %[...] conversion */
#define	CT_STRING	2	/* %s conversion */
#define	CT_INT		3	/* integer, i.e., strtol or strtoul */
#define	CT_FLOAT	4	/* floating, i.e., strtod */

#if 0
#define u_char unsigned char
#endif
#ifndef u_char
#define u_char char
#endif
#ifndef u_long
#define u_long unsigned long
#endif

#ifndef _NO_LONGLONG
typedef unsigned long long u_long_long;
#endif

typedef union
{
  char c[16] __attribute__ ((__aligned__ (16)));
  short h[8];
  long l[4];
  int i[4];
  float f[4];
} vec_union;

#ifndef _LONG_DOUBLE
#define _LONG_DOUBLE long double
#endif

#define LONG_MAX	2147483647L
#define LONG_MIN	(-LONG_MAX - 1L)
#define ULONG_MAX 	(LONG_MAX * 2UL + 1)

#define LONG_LONG_MAX	9223372036854775807LL
#define LONG_LONG_MIN	(-LONG_LONG_MAX-1)

#define ULONG_LONG_MAX	(2ULL * LONG_LONG_MAX + 1)

/* ISO C9x macro names */
#define LLONG_MAX LONG_LONG_MAX
#define LLONG_MIN LONG_LONG_MIN
#define ULLONG_MAX ULONG_LONG_MAX


#define _CAST_VOID (void)

#define	EBADF 		9	/* Bad file number */
#define	ERANGE 		34	/* Math result not representable */

#define _CONST const
#define _NO_POS_ARGS
#define	_PTR		void *

#define dbg_prompt(fmt_, ...)	//ota_prompt(fmt_, ##__VA_ARGS__)
#define dbg_detail(fmt_, ...)	//ota_prompt(fmt_, ##__VA_ARGS__)
int _vsscanf(char _CONST *str, char _CONST *fmt0, va_list ap);

unsigned long _strtoul_(_CONST char *nptr, char **endptr, int base);
unsigned long long _strtoull_(_CONST char *nptr, char **endptr, int base);
long long _strtoll_(_CONST char *nptr, char **endptr, int base);
long _strtol_(_CONST char *nptr, char **endptr, int base);
double _strtod_(_CONST char *nptr, char **endptr);

#define _strtoul	_strtoul_
#define _strtoull	_strtoull_
#define _strtoll	_strtoll_
#define _strtol		_strtol_
#define _strtod		_strtod_

int _vsscanf_test(char* str, char* fmt, ...)
{
	int ret;
	va_list ap;
	va_start (ap, fmt);
	ret = _vsscanf (str, fmt, ap);
	va_end (ap);
	return ret;
}


u_char * sccl(register char *tab, register u_char *fmt);

#define CYGNUS_NEC
//#define _NO_LONGLONG

int _vsscanf(char _CONST *str, char _CONST *fmt0, va_list ap)
{
	register u_char *fmt = (u_char *) fmt0;
	register int c;		/* character from format, or conversion */
	register int type;		/* conversion type */
	register size_t width;	/* field width, or 0 */
	register char *p;		/* points into all kinds of strings */
	register int n;		/* handy integer */
	register int flags;		/* flags as defined above */
	register char *p0;		/* saves original value of p when necessary */
	int orig_flags=0;               /* saved flags used when processing vector */
	int int_width;                /* tmp area to store width when processing int */
	int nassigned;		/* number of fields assigned */
	int nread;			/* number of characters consumed from fp */
	int base = 0;			/* base argument to strtol/strtoul */
	int nbytes = 1;               /* number of bytes read from fmt string */
	unsigned short wc;                   /* wchar to use to read format string */
	char vec_sep;                 /* vector separator char */
	char last_space_char;         /* last white-space char eaten - needed for vec support */
	int vec_read_count;           /* number of vector items to read separately */
	int looped;                   /* has vector processing looped */
	u_long (*ccfn) () = 0;	/* conversion function (strtol/strtoul) */
	char ccltab[256];		/* character class table for %[...] */
	char buf[BUF];		/* buffer for numeric conversions */
	vec_union vec_buf;
	char *lptr;           /* literal pointer */

	char *ch_dest=NULL;
	short *sp=NULL;
	int *ip;
	float *flp=NULL;
	_LONG_DOUBLE *ldp;
	double *dp;
	long *lp=NULL;
#ifndef _NO_LONGLONG
	long long *llp;
#else
	u_long _uquad;
#endif
	char* cur = (char*)str;
	int len = strlen(str);

	dbg_prompt("str = %s, len = %d, fmt = %s", str, len, fmt0);

	/* `basefix' is used to avoid `if' tests in the integer scanner */
	static _CONST short basefix[17] =
	{10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

	nassigned = 0;
	nread = 0;
	for (;;)
	{
		wc = *fmt;
		fmt += nbytes;

		dbg_detail("------------------------\nwc = %c, nbytes = %d", wc, nbytes);
		if (wc == 0)
		{
			return nassigned;
		}
		if (nbytes == 1 && isspace (wc))
		{
			for (;;)
			{
				dbg_detail("");
				if (!len)
				{
					return nassigned;
				}
				if (!isspace (*cur))
				{
					break;
				}
				nread++, len--,cur++;
			}
			continue;
		}
		if (wc != '%')
		{
			dbg_detail("");
			goto literal;
		}
		width = 0;
		flags = 0;
		vec_sep = ' ';
		vec_read_count = 0;
		looped = 0;

		/*
		* switch on the format.  continue if done; break once format
		* type is derived.
		*/

	again:
		c = *fmt++;
		dbg_detail("c = %c, %s", c,fmt);
		switch (c)
		{
			case '%':
			literal:
				lptr = fmt - nbytes;
				dbg_detail("n=%d, nbytes = %d, len = %d, *lptr = %s", n, nbytes, len, lptr);
				for (n = 0; n < nbytes; ++n)
				{
					if (len == 0)
					{
						goto input_failure;
					}
					
					dbg_prompt("*cur = %c, *lptr = %c", *cur, *lptr);

					if (*cur != *lptr)
					{
						dbg_prompt("*cur = [%c], *lptr = [%c]", *cur, *lptr);
						goto match_failure;
					}
					len--,cur++;
					nread++;
					++lptr;
				}
				continue;

			case '*':
				flags |= SUPPRESS;
				goto again;
			case ',':
			case ';':
			case ':':
			case '_':
				if (flags == SUPPRESS || flags == 0)
				{
					vec_sep = c;
				}
				goto again;
			case 'l':
				if (flags & SHORT)
				{
					continue; /* invalid format, don't process any further */
				}
				if (flags & LONG)
				{
					flags &= ~LONG;
					flags &= ~VECTOR;
					flags |= LONGDBL;
				}
				else
				{
					flags |= LONG;
					if (flags & VECTOR)
					vec_read_count = 4;
				}
				goto again;
			case 'L':
				flags |= LONGDBL;
				flags &= ~VECTOR;
				goto again;
			case 'h':
				flags |= SHORT;
				if (flags & LONG)
				{
					continue;  /* invalid format, don't process any further */
				}
				
				if (flags & VECTOR)
				{
					vec_read_count = 8;
				}
				goto again;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				width = width * 10 + c - '0';
				goto again;

				/*
				* Conversions. Those marked `compat' are for
				* 4.[123]BSD compatibility.
				*
				* (According to ANSI, E and X formats are supposed to
				* the same as e and x.  Sorry about that.)
				*/

			case 'D':		/* compat */
				flags |= LONG;
				/* FALLTHROUGH */
			case 'd':
				type = CT_INT;
				ccfn = (u_long (*)())_strtol;
				base = 10;
				break;

			case 'i':
				type = CT_INT;
				ccfn = (u_long (*)())_strtol;
				base = 0;
				break;

			case 'O':		/* compat */
				flags |= LONG;
				/* FALLTHROUGH */
			case 'o':
				type = CT_INT;
				ccfn = _strtoul;
				base = 8;
				break;

			case 'u':
				type = CT_INT;
				ccfn = _strtoul;
				base = 10;
				break;

			case 'X':		/* compat   XXX */
			case 'x':
				flags |= PFXOK;	/* enable 0x prefixing */
				type = CT_INT;
				ccfn = _strtoul;
				base = 16;
				break;

#ifdef FLOATING_POINT
			case 'E':		/* compat   XXX */
			case 'G':		/* compat   XXX */
			/* ANSI says that E,G and X behave the same way as e,g,x */
			/* FALLTHROUGH */
			case 'e':
			case 'f':
			case 'g':
				type = CT_FLOAT;
				if (flags & VECTOR)
				vec_read_count = 4;
				break;
#endif

			case 's':
				flags &= ~VECTOR;
				type = CT_STRING;
				break;

			case '[':
				fmt = sccl (ccltab, fmt);
				flags |= NOSKIP;
				flags &= ~VECTOR;
				type = CT_CCL;
				break;

			case 'c':
				flags |= NOSKIP;
				type = CT_CHAR;
				if (flags & VECTOR)
				{
					/* not allowed to have h or l with c specifier */
					if (flags & (LONG | SHORT))
					{
						continue;  /* invalid format don't process any further */
					}
					width = 0;
					vec_read_count = 16;
				}
				break;

			case 'p':		/* pointer format is like hex */
				flags |= POINTER | PFXOK;
				type = CT_INT;
				ccfn = _strtoul;
				base = 16;
				break;

			case 'n':
				if (flags & SUPPRESS)	/* ??? */
				{
					continue;
				}
				flags &= ~VECTOR;
				if (flags & SHORT)
				{
					sp = va_arg (ap, short *);
					*sp = nread;
				}
				else if (flags & LONG)
				{
					lp = va_arg (ap, long *);
					*lp = nread;
				}
#ifndef _NO_LONGLONG
				else if (flags & LONGDBL)
				{
					llp = va_arg (ap, long long*);
					*llp = nread;
				}
#endif
				else
				{
					ip = va_arg (ap, int *);
					*ip = nread;
				}
				continue;

			/*
			* Disgusting backwards compatibility hacks.	XXX
			*/
			case '\0':		/* compat */
				return EOF;

			default:		/* compat */
				if (isupper (c))
				{
					flags |= LONG;
				}
				type = CT_INT;
				ccfn = (u_long (*)())_strtol;
				base = 10;
				break;
		}

		dbg_prompt("len = %d, flags = %x",len, flags);

		process:
		/*
		* We have a conversion that requires input.
		*/
		if (!len)
		{
			goto input_failure;
		}

		/*
		* Consume leading white space, except for formats that
		* suppress this.
		*/
		last_space_char = '\0';

		if ((flags & NOSKIP) == 0)
		{
			while (isspace (*cur))
			{
				last_space_char = *cur;
				nread++;
				if (--len > 0)
				{
					cur++;
				}
				else
#ifndef CYGNUS_NEC
				if (__srefill (fp))
#endif
				goto input_failure;
			}
			/*
			* Note that there is at least one character in the
			* buffer, so conversions that do not set NOSKIP ca
			* no longer result in an input failure.
			*/
		}
		
		dbg_prompt("looped = %d, flags = %x",looped, flags);

		/* for vector formats process separator characters after first loop */
		if (looped && (flags & VECTOR))
		{
			flags = orig_flags; 
			/* all formats other than default char have a separator char */
			if (vec_sep != ' ' || type != CT_CHAR)
			{
				if ((vec_sep == ' ' && last_space_char != ' ') || (vec_sep != ' ' && *cur != vec_sep))
				{
					dbg_prompt("type = %d",type);
					goto match_failure;
				}
				if (vec_sep != ' ')
				{
					nread++;
					if (--len > 0)
					{
						cur++;
					}
					else
#ifndef CYGNUS_NEC
					if (__srefill (fp))
#endif
					goto input_failure;
				}
			}
			/* after eating the separator char, we must eat any white-space
			after the separator char that precedes the data to convert */
			if ((flags & NOSKIP) == 0)
			{
				while (isspace (*cur))
				{
					last_space_char = *cur;
					nread++;
					if (--len > 0)
					{
						cur++;
					}
					else
#ifndef CYGNUS_NEC
					if (__srefill (fp))
#endif
					goto input_failure;
				}
			}

		}
		else /* save to counter-act changes made to flags when processing */
		{
			orig_flags = flags;
		}
		
		dbg_prompt("type = %d",type);

		/*
		* Do the conversion.
		*/
		switch (type)
		{

			case CT_CHAR:
				/* scan arbitrary characters (sets NOSKIP) */
				if (width == 0)
				{
					width = 1;
				}
				
				if (flags & SUPPRESS)
				{
					size_t sum = 0;

					for (;;)
					{
						if ((n = len) < (int)width)
						{
							sum += n;
							width -= n;
							cur += n;
#ifndef CYGNUS_NEC
							if (__srefill (fp))
							{
#endif
								if (sum == 0)
								{
									goto input_failure;
								}
								break;
#ifndef CYGNUS_NEC
							}
#endif
						}
						else
						{
							sum += width;
							len -= width;
							cur += width;
							break;
						}
					}
					nread += sum;
				}
				else
				{
					int n = width;
					if (!looped)
					{
						if (flags & VECTOR)
						{
							ch_dest = vec_buf.c;
						}
						else
						{
							ch_dest = va_arg (ap, char *);
						}
					}

					size_t r = 0;//fread (ch_dest, 1, width, fp);
					if(len >= width)
					{
						r = width;
						memcpy(ch_dest, cur, width);
						cur += width;
						len -= width;
					}
					else if(len)
					{
						r = len;
						memcpy(ch_dest, cur, len);
						cur += len;
						len = 0;
					}
					else
					{
						r = 0;
					}

					if (r == 0)
					{
						goto input_failure;
					}
					nread += r;
					ch_dest += r;

					if (!(flags & VECTOR))
					{
						nassigned++;
					}
				}
				break;

			case CT_CCL:
				/* scan a (nonempty) character class (sets NOSKIP) */
				if (width == 0)
				{
					width = ~0;		/* `infinity' */
				}
				/* take only those things in the class */
				if (flags & SUPPRESS)
				{
					n = 0;
					//while (ccltab[*cur])
					while(*(ccltab+*cur))
					{
						n++, len--, cur++;
						if (--width == 0)
						{
							break;
						}
						if (!len)
						{
							if (n == 0)
							{
								goto input_failure;
							}
							break;
						}
					}
					
					if (n == 0)
					{
						dbg_prompt("");
						nassigned++;
						continue;						
						//goto match_failure;
					}
				}
				else
				{
					p0 = p = va_arg (ap, char *);
					//while (ccltab[*cur])
					while(*(ccltab+*cur))
					{
						len--;
						*p++ = *cur++;
						if (--width == 0)
						{
							break;
						}
						
						if (!len)
						{
							if (p == p0)
							{
								goto input_failure;
							}
							break;
						}
					}
					n = p - p0;
					if (n == 0)
					{
						dbg_prompt("");
						nassigned++;
						continue;						
						//goto match_failure;
					}
					*p = 0;
					nassigned++;
				}
				nread += n;
				break;

			case CT_STRING:
				/* like CCL, but zero-length string OK, & no NOSKIP */
				if (width == 0)
				{
					width = ~0;
				}
				
				if (flags & SUPPRESS)
				{
					n = 0;
					while (!isspace (*cur))
					{
						n++, len--, cur++;
						if (--width == 0)
						{
							break;
						}
						if (!len)
						{
							break;
						}
					}
					nread += n;
				}
				else
				{
					p0 = p = va_arg (ap, char *);
					while (!isspace (*cur))
					{
						len--;
						*p++ = *cur++;
						if (--width == 0)
						{
							break;
						}
						if (!len)
						{
							break;
						}
					}
					*p = 0;
					nread += p - p0;
					nassigned++;
				}
				continue;

			case CT_INT:
			{
				unsigned int_width_left = 0;
				int skips = 0;
				int_width = width;
#ifdef hardway
				if (int_width == 0 || int_width > sizeof (buf) - 1)
#else
				/* size_t is unsigned, hence this optimisation */
				if (int_width - 1 > sizeof (buf) - 2)
#endif
				{
					int_width_left = width - (sizeof (buf) - 1);
					int_width = sizeof (buf) - 1;
				}
				flags |= SIGNOK | NDIGITS | NZDIGITS | NNZDIGITS;
				for (p = buf; int_width; int_width--)
				{
					c = *cur;
					/*
					* Switch on the character; `goto ok' if we
					* accept it as a part of number.
					*/
					dbg_prompt("c = %c, int_width = %d", c, int_width);
					switch (c)
					{
						/*
						* The digit 0 is always legal, but is special.
						* For %i conversions, if no digits (zero or nonzero)
						* have been scanned (only signs), we will have base==0.
						* In that case, we should set it to 8 and enable 0x
						* prefixing. Also, if we have not scanned zero digits
						* before this, do not turn off prefixing (someone else
						* will turn it off if we have scanned any nonzero digits).
						*/
						case '0':
							if (! (flags & NNZDIGITS))
							{
								goto ok;
							}
							if (base == 0)
							{
								base = 8;
								flags |= PFXOK;
							}
							if (flags & NZDIGITS)
							{
								flags &= ~(SIGNOK | NZDIGITS | NDIGITS);
								goto ok;
							}
							flags &= ~(SIGNOK | PFXOK | NDIGITS);
							if (int_width_left)
							{
								int_width_left--;
								int_width++;
							}
							++skips;
							goto skip;

						/* 1 through 7 always legal */
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
							base = basefix[base];
							flags &= ~(SIGNOK | PFXOK | NDIGITS | NNZDIGITS);
							goto ok;

						/* digits 8 and 9 ok iff decimal or hex */
						case '8':
						case '9':
							base = basefix[base];
							if (base <= 8)
							{
								break;	/* not legal here */
							}
							flags &= ~(SIGNOK | PFXOK | NDIGITS | NNZDIGITS);
							goto ok;

						/* letters ok iff hex */
						case 'A':
						case 'B':
						case 'C':
						case 'D':
						case 'E':
						case 'F':
						case 'a':
						case 'b':
						case 'c':
						case 'd':
						case 'e':
						case 'f':
							/* no need to fix base here */
							if (base <= 10)
							{
								break;	/* not legal here */
							}
							flags &= ~(SIGNOK | PFXOK | NDIGITS | NNZDIGITS);
							goto ok;

						/* sign ok only as first character */
						case '+':
						case '-':
							if (flags & SIGNOK)
							{
								flags &= ~SIGNOK;
								goto ok;
							}
							break;

						/* x ok iff flag still set & 2nd char */
						case 'x':
						case 'X':
							if (flags & PFXOK && p == buf + 1)
							{
								base = 16;/* if %i */
								flags &= ~PFXOK;
								/* We must reset the NZDIGITS and NDIGITS
								flags that would have been unset by seeing
								the zero that preceded the X or x.  */
								flags |= NZDIGITS | NDIGITS;
								goto ok;
							}
							break;
						default:
							dbg_detail("default");
							break;
					}

					/*
					* If we got here, c is not a legal character
					* for a number.  Stop accumulating digits.
					*/
					break;
					ok:
						/*
						* c is legal: store it and look at the next.
						*/
						*p++ = c;
					skip:
						if (--len > 0)
						{
							cur++;
						}
						else
#ifndef CYGNUS_NEC
						if (__srefill (fp))
#endif
						break;		/* EOF */
				}
				/*
				* If we had only a sign, it is no good; push back the sign.
				* If the number ends in `x', it was [sign] '0' 'x', so push back
				* the x and treat it as [sign] '0'.
				*/
				if (flags & NDIGITS)
				{
					if (p > buf)
					{
						//ungetc (*(u_char *)-- p, fp);
						*cur = *(u_char *)-- p;
					}
					dbg_prompt("cur = %s",cur);
					goto match_failure;
				}
				c = ((u_char *) p)[-1];
				if (c == 'x' || c == 'X')
				{
					--p;
					*cur = c;
					//ungetc (c, fp);
				}
				if ((flags & SUPPRESS) == 0)
				{
					u_long res;

					*p = 0;
					dbg_detail("ccfn base = %d", base);
					res = (*ccfn) (buf, (char **) NULL, base);
					dbg_detail("res = %d", res);
					if ((flags & POINTER) && !(flags & VECTOR))
					{
						*(va_arg (ap, void **)) = (void*) (unsigned int) res;
					}
					else if (flags & SHORT)
					{
						if (!(flags & VECTOR))
						{
							sp = va_arg (ap, short *);
						}
						else if (!looped)
						{
							sp = vec_buf.h;
						}
						*sp++ = res;
					}
					else if (flags & LONG)
					{
						if (!(flags & VECTOR))
						{
							lp = va_arg (ap, long *);
						}
						else if (!looped)
						{
							lp = vec_buf.l;
						}
						*lp++ = res;
					}
#ifndef _NO_LONGLONG
					else if (flags & LONGDBL)
					{
						u_long_long resll;
						if (ccfn == _strtoul)
						{
							resll = _strtoull (buf, (char **) NULL, base);
						}
						else
						{
							resll = _strtoll (buf, (char **) NULL, base);
						}
						llp = va_arg (ap, long long*);
						*llp = resll;
					}
#endif
					else
					{
						if (!(flags & VECTOR))
						{
							ip = va_arg (ap, int *);
							*ip++ = res;
						}
						else
						{
							if (!looped)
							{
								ch_dest = vec_buf.c;
							}
							*ch_dest++ = (char)res;
						}
					}
					if (!(flags & VECTOR))
					{
						nassigned++;
					}
				}
				nread += p - buf + skips;
				break;
			}

#ifdef FLOATING_POINT
			case CT_FLOAT:
			{
				/* scan a floating point number as if by strtod */
				/* This code used to assume that the number of digits is reasonable.
				However, ANSI / ISO C makes no such stipulation; we have to get
				exact results even when there is an unreasonable amount of
				leading zeroes.  */
				long leading_zeroes = 0;
				long zeroes, exp_adjust;
				char *exp_start = NULL;
				unsigned fl_width = width;
				unsigned width_left = 0;
#ifdef hardway
				if (fl_width == 0 || fl_width > sizeof (buf) - 1)
#else
				/* size_t is unsigned, hence this optimisation */
				if (fl_width - 1 > sizeof (buf) - 2)
#endif
				{
					width_left = fl_width - (sizeof (buf) - 1);
					fl_width = sizeof (buf) - 1;
				}
				flags |= SIGNOK | NDIGITS | DPTOK | EXPOK;
				zeroes = 0;
				exp_adjust = 0;
				dbg_prompt("buf = %s, width = %d, flags = %x", buf, fl_width, flags);
				for (p = buf; fl_width; )
				{
					c = *cur;
					dbg_prompt("c = %c", c);
					/*
					* This code mimicks the integer conversion
					* code, but is much simpler.
					*/
					switch (c)
					{

						case '0':
							if (flags & NDIGITS)
							{
								flags &= ~SIGNOK;
								zeroes++;
								if (width_left)
								{
									width_left--;
									fl_width++;
								}
								goto fskip;
							}
							/* Fall through.  */
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							flags &= ~(SIGNOK | NDIGITS);
							goto fok;

						case '+':
						case '-':
							if (flags & SIGNOK)
							{
								flags &= ~SIGNOK;
								goto fok;
							}
							break;
						case '.':
							dbg_prompt("flags = %x", flags);
							if (flags & DPTOK)
							{
								flags &= ~(SIGNOK | DPTOK);
								leading_zeroes = zeroes;
								goto fok;
							}
							break;
						case 'e':
						case 'E':
							/* no exponent without some digits */
							if ((flags & (NDIGITS | EXPOK)) == EXPOK
							|| ((flags & EXPOK) && zeroes))
							{
								if (! (flags & DPTOK))
								{
									exp_adjust = zeroes - leading_zeroes;
									exp_start = p;
								}
								flags = (flags & ~(EXPOK | DPTOK)) | SIGNOK | NDIGITS;
								zeroes = 0;
								goto fok;
							}
							break;
					}
					break;
					fok:
						*p++ = c;
					fskip:
						fl_width--;
						++nread;
						if (--len > 0)
						{
							cur++;
						}
						else if(strlen(str))
						{
							cur = (char*)str;
							len = strlen(str);
						}
						else
						{
							break;
						}
						dbg_prompt("refill");
#ifndef CYGNUS_NEC
						if (__srefill (fp))
						break;		/* EOF */	      
#endif

				}
				
				if (zeroes)
				{
					flags &= ~NDIGITS;
				}
				/*
				* If no digits, might be missing exponent digits
				* (just give back the exponent) or might be missing
				* regular digits, but had sign and/or decimal point.
				*/
				if (flags & NDIGITS)
				{
					dbg_prompt("[%d] flags = %d, %s", flags, exp_start);
					if (flags & EXPOK)
					{
						/* no digits at all */
						while (p > buf)
						{
							*cur = *(u_char *)-- p;
							--nread;
						}
						dbg_prompt("");
						goto match_failure;
					}
					
					/* just a bad exponent (e and maybe sign) */
					c = *(u_char *)-- p;
					--nread;
					if (c != 'e' && c != 'E')
					{
						//ungetc (c, fp);	/* sign */
						*cur = c;
						c = *(u_char *)-- p;
						--nread;
					}
					*cur = c;
					//ungetc (c, fp);
				}
				if ((flags & SUPPRESS) == 0)
				{
					double res;
					long new_exp = 0;
					dbg_prompt("flags = %x\n", flags);

					*p = 0;
					if ((flags & (DPTOK | EXPOK)) == EXPOK)
					{
						exp_adjust = zeroes - leading_zeroes;
						new_exp = -exp_adjust;
						exp_start = p;
						dbg_prompt("new_exp = %d, %s", new_exp, exp_start);
					}
					else if (exp_adjust)
					{
						new_exp = _strtol ((exp_start + 1), NULL, 10) - exp_adjust;
					}
					
					if (exp_adjust)
					{

						/* If there might not be enough space for the new exponent,
						truncate some trailing digits to make room.  */
						if (exp_start >= buf + sizeof (buf) - MAX_LONG_LEN)
						exp_start = buf + sizeof (buf) - MAX_LONG_LEN - 1;
						sprintf (exp_start, "e%ld", new_exp);

					}

					
					dbg_prompt("%s", buf);

					res = _strtod(buf, NULL);
					dbg_prompt("res 33 = %f", res);
					//res = strtol(&errno, buf, NULL, 10);
					if (flags & LONG)
					{
						dp = va_arg (ap, double *);
						*dp = res;
						dbg_prompt("*dp = %f", *dp);
					}
					else if (flags & LONGDBL)
					{
						ldp = va_arg (ap, _LONG_DOUBLE *);
						*ldp = res;
						dbg_prompt("*ldp = %f", *ldp);
					}
					else
					{
						if (!(flags & VECTOR))
						{
							flp = va_arg (ap, float *);
							dbg_prompt("flp = %x", flp);
						}
						else if (!looped)
						{
							flp = vec_buf.f;
							dbg_prompt("flp = %X", flp);
						}
						*flp = res;
						dbg_prompt("*flp = %f", *flp);
						flp++;
					}
					if (!(flags & VECTOR))
					{
						nassigned++;
					}
				}
				break;
			}
#endif /* FLOATING_POINT */
		}
		
		if (vec_read_count-- > 1)
		{
			looped = 1;
			goto process;
		}
		if (flags & VECTOR)
		{
			int i;
			unsigned long *vp = va_arg (ap, unsigned long *);
			for (i = 0; i < 4; ++i)
			{
				*vp++ = vec_buf.l[i];
			}
			nassigned++;
		}
	}
input_failure:
	printf("INPUT FAILURE");
	return nassigned ? nassigned : -1;
match_failure:
	printf("MATCH FAILURE");
	return nassigned;
}

u_char * sccl(register char *tab, register u_char *fmt)
{
	register int c, n, v;

	/* first `clear' the whole table */
	c = *fmt++;			/* first char hat => negated scanset */
	if (c == '^')
	{
		v = 1;			/* default => accept */
		c = *fmt++;		/* get new first char */
	}
	else
	{
		v = 0;			/* default => reject */
	}
	/* should probably use memset here */
	for (n = 0; n < 256; n++)
	{
		tab[n] = v;
	}
	if (c == 0)
	{
		return fmt - 1;		/* format ended before closing ] */
	}

	/*
	* Now set the entries corresponding to the actual scanset to the
	* opposite of the above.
	*
	* The first character may be ']' (or '-') without being special; the
	* last character may be '-'.
	*/

	v = 1 - v;
	for (;;)
	{
		tab[c] = v;		/* take character c */
		doswitch:
		n = *fmt++;		/* and examine the next */
		switch (n)
		{
		case 0:		/* format ended too soon */
			return fmt - 1;
		case '-':
			/*
			* A scanset of the form [01+-] is defined as `the digit 0, the
			* digit 1, the character +, the character -', but the effect of a
			* scanset such as [a-zA-Z0-9] is implementation defined.  The V7
			* Unix scanf treats `a-z' as `the letters a through z', but treats
			* `a-a' as `the letter a, the character -, and the letter a'.
			*
			* For compatibility, the `-' is not considerd to define a range if
			* the character following it is either a close bracket (required by
			* ANSI) or is not numerically greater than the character we just
			* stored in the table (c).
			*/
			n = *fmt;
			if (n == ']' || n < c)
			{
				c = '-';
				break;		/* resume the for(;;) */
			}
			fmt++;
			do
			{			/* fill in the range */
				tab[++c] = v;
			}
			while (c < n);
#if 1			/* XXX another disgusting compatibility hack */
			/*
			* Alas, the V7 Unix scanf also treats formats such
			* as [a-c-e] as `the letters a through e'. This too
			* is permitted by the standard....
			*/
			goto doswitch;
#else
			c = *fmt++;
			if (c == 0)
				return fmt - 1;
			if (c == ']')
				return fmt;
#endif

		break;
		case ']':		/* end of scanset */
			return fmt;

		default:		/* just another character */
			c = n;
			break;
		}
	}
	/* NOTREACHED */
}
long _strtol_(_CONST char *nptr, char **endptr, int base)
{
	register const unsigned char *s = (const unsigned char *)nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
               if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? LONG_MIN : LONG_MAX;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? (char *)s - 1 : nptr);
	return (acc);
}


unsigned long _strtoul_(_CONST char *nptr, char **endptr, int base)
{
	register const unsigned char *s = (const unsigned char *)nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * See strtol for comments as to the logic used.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
	cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
               if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULONG_MAX;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? (char *)s - 1 : nptr);
	return (acc);
}

unsigned long long _strtoull_(_CONST char *nptr, char **endptr, int base)
{
	register const unsigned char *s = (const unsigned char *)nptr;
	register unsigned long long acc;
	register int c;
	register unsigned long long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * See strtol for comments as to the logic used.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = (unsigned long long)ULONG_LONG_MAX / (unsigned long long)base;
	cutlim = (unsigned long long)ULONG_LONG_MAX % (unsigned long long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
               if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = ULONG_LONG_MAX;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? (char *)s - 1 : nptr);
	return (acc);
}

long long _strtoll_(_CONST char *nptr, char **endptr, int base)
{
	register const unsigned char *s = (const unsigned char *)nptr;
	register unsigned long long acc;
	register int c;
	register unsigned long long cutoff;
	register int neg = 0, any, cutlim;

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	do {
		c = *s++;
	} while (isspace(c));
	if (c == '-') {
		neg = 1;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
	    c == '0' && (*s == 'x' || *s == 'X')) {
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	cutoff = neg ? -(unsigned long long)LONG_LONG_MIN : LONG_LONG_MAX;
	cutlim = cutoff % (unsigned long long)base;
	cutoff /= (unsigned long long)base;
	for (acc = 0, any = 0;; c = *s++) {
		if (isdigit(c))
			c -= '0';
		else if (isalpha(c))
			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
               if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0) {
		acc = neg ? LONG_LONG_MIN : LONG_LONG_MAX;
	} else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *) (any ? (char *)s - 1 : nptr);
	return (acc);
}

double _strtod_(_CONST char *nptr, char **endptr)
{
	char *ptr = nptr;
	double fra_divider = 10;
	double value = 0;
	int dot_found = 0;
	int neg = 0;

	if(*ptr == '-')
	{
		neg = 1;
		ptr++;
	}
	else if(*ptr == '+')
	{
		ptr++;
	}
	
	while(*ptr)
	{
		if(*ptr >= '0' && *ptr <= '9')
		{
			if(dot_found)
			{
				value += (*ptr - '0') / fra_divider;
				fra_divider *= 10;
			}
			else
			{
				value = value * 10 + *ptr - '0';
			}
		}
		else if(*ptr == '.')
		{
			if(dot_found)
			{
				break;
			}
			else
			{
				dot_found = 1;
			}
		}
		else
		{
			break;
		}


		ptr++;

	}

	if(endptr)
	{
		*endptr = ptr;
	}

	if(neg)
	{
		value = -value;
	}


	return value;
}

int sscanf_(char* buffer, char* format, ...)
{
    int count = 0;

    if(!strlen(buffer)) return count;

    va_list sp;
    va_start(sp, format);
    count = _vsscanf((char *)(buffer), (char *)format, sp);
    va_end(sp);

    return count;
}
