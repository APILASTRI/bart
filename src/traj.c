/* Copyright 2014. The Regents of the University of California.
 * All rights reserved. Use of this source code is governed by
 * a BSD-style license which can be found in the LICENSE file.
 *
 * Authors:
 * 2012 Martin Uecker <uecker@eecs.berkeley.edu>
 */

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <complex.h>
#include <stdio.h>
#include <math.h>
#include <getopt.h>

#include "num/multind.h"

#include "misc/mmio.h"
#include "misc/misc.h"
#include "misc/mri.h"


static void usage(const char* name, FILE* fd)
{
	fprintf(fd, "Usage: %s <output>\n", name);
}


static void help(void)
{
	printf( "\n"
		"Computes k-space trajectories.\n"
		"\n"
		"-x x\treadout samples\n"
		"-y y\tphase encoding lines\n"
		"-a a\tacceleration\n"
		"-r\tradial\n"
		"-h\thelp\n");
}


int main(int argc, char* argv[])
{
	int X = 128;
	int Y = 128;
	int accel = 1;
	bool radial = false;
	int c;

	while (-1 != (c = getopt(argc, argv, "x:y:a:rh"))) {

		switch (c) {

		case 'x':
			X = atoi(optarg);
			break;

		case 'y':
			Y = atoi(optarg);
			break;

		case 'a':
			accel = atoi(optarg);
			break;

		case 'r':
			radial = true;
			break;

		case 'h':
			usage(argv[0], stdout);
			help();
			exit(0);

		default:
			usage(argv[0], stderr);
			exit(1);
		}
	}

	if (argc - optind != 1) {

		usage(argv[0], stderr);
		exit(1);
	}

	int N = X * Y / accel;
	//long dims[3] = { 3, X, Y / accel }; 
	long dims[3] = { 3, X * Y / accel, 1 };
	complex float* samples = create_cfl(argv[optind + 0], 3, dims);


	int p = 0;
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j += accel) {

			if (radial) {

				samples[p * 3 + 0] = ((float)i + 0.5 - (float)X / 2.) * sin(1. * M_PI * (float)j / (float)Y);
				samples[p * 3 + 1] = ((float)i + 0.5 - (float)X / 2.) * cos(1. * M_PI * (float)j / (float)Y);
				samples[p * 3 + 2] = 0.;

			} else {

				samples[p * 3 + 0] = (i - X / 2);
				samples[p * 3 + 1] = (j - Y / 2);
				samples[p * 3 + 2] = 0;
			}

			p++;
		}
	}
	assert(p == N - 0);

	unmap_cfl(3, dims, samples);
	exit(0);
}


