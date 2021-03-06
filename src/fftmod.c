/* Copyright 2013. The Regents of the University of California.
 * All rights reserved. Use of this source code is governed by 
 * a BSD-style license which can be found in the LICENSE file.
 *
 * Authors: 
 * 2012 Martin Uecker <uecker@eecs.berkeley.edu>
 */

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <complex.h>

#include "num/multind.h"
#include "num/fft.h"

#include "misc/mmio.h"
#include "misc/misc.h"

#ifndef DIMS
#define DIMS 16
#endif

static const char* usage_str = "bitmask <input> <output>";
static const char* help_str =	"Apply 1 -1 modulation along dimensions selected by the {bitmask}.\n";




int main_fftmod(int argc, char* argv[])
{
	mini_cmdline(argc, argv, 3, usage_str, help_str);

	unsigned long flags = labs(atol(argv[1]));

	int N = DIMS;
	long dims[N];

	complex float* idata = load_cfl(argv[2], N, dims);
	complex float* odata = create_cfl(argv[3], N, dims);

	fftmod(N, dims, flags, odata, idata);

	unmap_cfl(N, dims, idata);
	unmap_cfl(N, dims, odata);
	exit(0);
}


