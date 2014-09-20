/* Copyright 2014. The Regents of the University of California.
 * All rights reserved. Use of this source code is governed by 
 * a BSD-style license which can be found in the LICENSE file.
 *
 * Authors:
 * 2012, 2014 Martin Uecker <uecker@eecs.berkeley.edu>
 */
 
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <complex.h>
#include <stdio.h>

#include "num/multind.h"
#include "num/flpmath.h"

#include "misc/misc.h"
#include "misc/mmio.h"
#include "misc/utils.h"


#ifndef DIMS
#define DIMS 16
#endif





const char* usage_str = "flags <input> <output>";
const char* help_str = "Normalize along selected dimensions.\n";



int main(int argc, char* argv[])
{
	mini_cmdline(argc, argv, 3, usage_str, help_str);

	int N = DIMS;
	long dims[N];
	complex float* data = load_cfl(argv[2], N, dims);

	int flags = atoi(argv[1]);

	assert(flags >= 0);

	complex float* out = create_cfl(argv[3], N, dims);
	md_copy(N, dims, out, data, CFL_SIZE);
	normalize(N, flags, dims, out);

	unmap_cfl(N, dims, out);
	exit(0);
}

