/* Copyright 2013. The Regents of the University of California.
 * All rights reserved. Use of this source code is governed by 
 * a BSD-style license which can be found in the LICENSE file.
 *
 * Authors:
 * 2012 Martin Uecker <uecker@eecs.berkeley.edu>
 *
 *
 * McKenzie CA, Yeh EN, Ohliger MA, Price MD, Sodickson DK. Self-calibrating parallel
 * imaging with automatic coil sensitivity extraction. Magn Reson Med 2002; 47:529–538.
 */


#include <complex.h>
#include <math.h>
#include <assert.h>

#ifdef USE_GSL
#include <gsl/gsl_specfunc.h>
#endif


#include "num/multind.h"
#include "num/flpmath.h"
#include "num/fft.h"

#include "misc/mri.h"
#include "misc/misc.h"

#include "direct.h"




static double bessel_I0(double x)
{
#ifdef USE_GSL
	return gsl_sf_bessel_I0(x);
#else
	assert(0);
#endif
}

static double kaiser(double beta, int M, int n)
{
	if (M == 1)
		return 1.;

	if (fabs((double)n / (double)M - 0.5) >= 0.5)
		return 0.;

	return bessel_I0(beta * sqrt(1. - pow(2. * (double)n / (double)M - 1., 2.)))
		/ bessel_I0(beta);
}	

void direct_calib(const long dims[5], complex float* sens, const long caldims[5], const complex float* data)
{
	complex float* tmp = md_alloc(5, caldims, CFL_SIZE);

	assert(1 == caldims[4]);
	assert(1 == dims[4]);

	md_copy(5, caldims, tmp, data, CFL_SIZE);

	// apply Kaiser-Bessel Window beta=4
	for (int z = 0; z < caldims[2]; z++)
		for (int y = 0; y < caldims[1]; y++)
			for (int x = 0; x < caldims[0]; x++)
				for (int c = 0; c < caldims[3]; c++)
					tmp[((c * caldims[2] + z) * caldims[1] + y) * caldims[0] + x]
						*= kaiser(4., caldims[2], z)
						* kaiser(4., caldims[1], y)
						* kaiser(4., caldims[0], x);

	md_resizec(5, dims, sens, caldims, tmp, CFL_SIZE);

	ifftc(5, dims, 7, sens, sens);

	long dims1[5];
	md_select_dims(5, ~MD_BIT(COIL_DIM), dims1, dims);

	complex float* img = md_alloc(5, dims1, CFL_SIZE);

	md_zrss(5, dims, COIL_FLAG, img, sens);
#if 1
	long T = md_calc_size(5, dims1);
	for (int i = 0; i < T; i++)
		for (int j = 0; j < dims[COIL_DIM]; j++)
			sens[j * T + i] *= (cabs(img[i]) == 0.) ? 0. : (1. / cabs(img[i]));
#endif
	free(img);
}



