/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"

static void dostrings(FILE *fp, const char *fname);

static void
usage(void)
{
	eprintf("usage: %s file...\n", argv0);
}

int
main(int argc, char *argv[])
{
	FILE *fp;

	ARGBEGIN {
	default:
		usage();
	} ARGEND;

	if (argc == 0) {
		dostrings(stdin, "<stdin>");
	} else {
		for (; argc > 0; argc--, argv++) {
			if (!(fp = fopen(argv[0], "r")))
				eprintf("open %s:", argv[0]);
			dostrings(fp, argv[0]);
			fclose(fp);
		}
	}

	return EXIT_SUCCESS;
}

static void
dostrings(FILE *fp, const char *fname)
{
	unsigned char buf[BUFSIZ];
	int c, i = 0;
	off_t offset = 0;

	do {
		offset++;
		if (isprint(c = getc(fp)))
			buf[i++] = c;
		if ((!isprint(c) && i >= 6) || i == sizeof(buf) - 1) {
			buf[i] = '\0';
			printf("%8ld: %s\n", (long)offset - i - 1, buf);
			i = 0;
		}
	} while (c != EOF);
	if (ferror(fp))
		eprintf("%s: read error:", fname);
}
