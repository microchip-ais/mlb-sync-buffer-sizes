/*
 * MediaLB synchronous channel buffer calculator.
 *
 * Input:
 *     bytes per frame [1 .. 372]
 *
 * Output:
 *     all possibles combinations of fcntl and buffer size of DIM2
 *
 * Example output for the bytes per frame 4:
 *
 *     fcntl: 0, buffer size: 4 x [1 .. 2048] = [4 .. 8192]
 *     fcntl: 1, buffer size: 8 x [1 .. 1024] = [8 .. 8192]
 *     fcntl: 2, buffer size: 16 x [1 .. 512] = [16 .. 8192]
 *     fcntl: 3, buffer size: 32 x [1 .. 256] = [32 .. 8192]
 *     fcntl: 4, buffer size: 64 x [1 .. 128] = [64 .. 8192]
 *     fcntl: 5, buffer size: 128 x [1 .. 64] = [128 .. 8192]
 *     fcntl: 6, buffer size: 256 x [1 .. 32] = [256 .. 8192]
 *
 * Example output for the bytes per frame 26:
 *
 *     fcntl: 0, buffer size: 26 x [1 .. 315] = [26 .. 8190]
 *     fcntl: 1, buffer size: 52 x [1 .. 157] = [52 .. 8164]
 *     fcntl: 2, buffer size: 104 x [1 .. 78] = [104 .. 8112]
 *     fcntl: 3, buffer size: 208 x [1 .. 39] = [208 .. 8112]
 *     fcntl: 4, buffer size: 416 x [1 .. 19] = [416 .. 7904]
 *     fcntl: 5, buffer size: 832 x [1 .. 9] = [832 .. 7488]
 *
 */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned u32;

static u32 const fpsubb[] = { 1, 2, 4, 8, 16, 32, 64 };

enum { KiB = 1024 };
enum { CDT_BUF_SIZE = 4 * KiB };
enum { ADT_BUF_SIZE = 8 * KiB };


int main(int argc, char **argv)
{
	int fcntl;
	unsigned long val;
	char *rest;
	u32 bpf;

	if (argc < 2) {
		fprintf(stderr, "error: expected bytes per frame [1 .. 372]\n");
		return 1;
	}

	val = strtoul(argv[1], &rest, 0);
	if (*rest != 0 || val <= 0 || val > 372) {
		fprintf(stderr, "error: bad bytes per frame\n");
		return 1;
	}

	bpf = val;
	for (fcntl = 0; fcntl < sizeof(fpsubb) / sizeof(*fpsubb); fcntl++) {
		u32 u = fpsubb[fcntl] * bpf;

		if (4 * u <= CDT_BUF_SIZE) {
			u32 n = ADT_BUF_SIZE / u;

			printf("fcntl: %d, buffer size: %d x [1 .. %d] = [%d .. %d]\n",
				fcntl, u, n, u, u * n);
		}
	}

	return 0;
}
