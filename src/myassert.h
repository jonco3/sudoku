/*
 * Assertions
 */

#ifndef __MYASSERT_H__
#define __MYASSERT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#undef assert

void die(const char* message, ...)
	{
	char buffer[256];
	va_list args;
	va_start(args, message);
	vsprintf(buffer, message, args);
	va_end(args);
	fflush(stdout);
	fprintf(stderr, "%s\n", buffer);
	exit(1);
	}

#ifndef DEBUG

#define assert(a) ((void)0)
#define assert_equal(e, a) ((void)0)
#define assert_null(a) ((void)0)
#define assert_compile(e)

#else

#define assert(a) ((a) ? (void)0 : assert_fail(__FILE__, __LINE__, #a))

#define assert_equal(e, a) assert_equal_impl(__FILE__, __LINE__, e, a, #e, #a)

#define assert_null(a) assert_null_impl(__FILE__, __LINE__, a, #a)

void assert_fail(const char* file, int line, const char* as)
	{
	die("%s:%d: assertion failed: %s\n", file, line, as);
	}

void assert_equal_impl(const char* file, int line, int ev, int av, const char* es, const char* as)
	{
	if (av != ev)
		die("%s:%d: assertion failed: %s == %s, expected %d but got %d\n", file, line, as, es, ev, av);
	}

void assert_null_impl(const char* file, int line, void* av, const char* as)
	{
	if (av != NULL)
		die("%s:%d: assertion failed: %s == NULL, expected NULL but got %08x\n", file, line, as, av);
	}

// compile time assert macro from http://www.pixelbeat.org/programming/gcc/static_assert.html
#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
#define assert_compile(e) enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }

#endif

#endif
