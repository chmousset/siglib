/**
 * SigLib, simple Signals Library
 * 
 * Copyright (C) 2013 Charles-Henri Mousset
 * 
 * This file is part of SigLib.
 * 
 * SigLib is free software: you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * SigLib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU Lesser General Public License for more details. You should have received a copy of the GNU
 * General Public License along with SigLib. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Authors: Charles-Henri Mousset
 */

/** \file sigi.h
 * SigLib Header, integer signals
 */

#include "sig.h"

#ifndef SIG_LIBI_H__
#define SIG_LIBI_H__


/** @addtogroup config
 * @{

/** @} */


/** @addtogroup siglib
 * @{
 */

/** @} */

/** @ingroup int
 * @struct sig_interpolate_lin_param
 * @brief structure representing the parameters of a 'linear interpolation' signal
 * @details the 'linear interpolation' signal returns (a * (n - delay) + b) / div
 */
struct sig_interpolate_lin_param {
	int a;
	int b;
	n_t delay;
	int div;
};

/** @ingroup int
 * @brief linear interpolation (ax + b) form
 * @details if n = n_last, then the cached value (x_cst) is returned.
 * returns (a * (n - delay) + b) / div.
 * @see sig_interpolate_lin_param
 *
 * @param[in] self pointer to the signal structure
 * @param[in] n the value of n
 */
int sig_interpolate_lin(struct signal_int *self, n_t n);


int sig_interpolate_st(struct signal_int *self, n_t n);


#endif
