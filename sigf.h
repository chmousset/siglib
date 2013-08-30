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


/** \file sigf.h
 * SigLib Header, floating point implementation
 */

#include "sig.h"

#ifndef SIG_LIBF_H__
#define SIG_LIBF_H__


/***************************************************************************************/
/*                                    Macros                                           */
/***************************************************************************************/
#define sig_get_lastvalue_f(sig)	sig->x_cst;


/***************************************************************************************/
/*                                 Configuration                                       */
/***************************************************************************************/


/** @addtogroup config
 * @{
 */

/**
 * @brief Enables PID feed-forard
 */
#if !defined(SIG_PID_FF) || defined(__DOXYGEN__)
#define SIG_PID_FF                  TRUE
#endif

/** @} */


/** @addtogroup siglib
 * @{
 */

/** @} */

/***************************************************************************************/
/*                              Parameter Structures                                   */
/***************************************************************************************/

/** @ingroup float
 * @struct sig_add_param_f
 * @brief structure representing the parameters of a 'adder' signal
 * @details the adder returns (a + b)
 * a and b can be (in order of priority)
 * -# a pointer to another signal
 * -# a pointer to a variable
 * -# a constant value
 */
struct sig_add_param_f {
	struct signal_float *a;								//!< signal source for a
	struct signal_float *b;								//!< signal source for b
	float *a_var;										//!< variable source for a
	float *b_var;										//!< variable source for a
	float a_cst;										//!< constant value of a
	float b_cst;										//!< constant value of b
	n_t n_last;											//!< the sample was taken at n = n_last
};


/** @ingroup float
 * @struct sig_sampler_param_f
 * @brief structure representing the parameters of a sampler signal
 * @details returns the sampled value
 */
struct sig_sampler_param_f {
	n_t n_last;											//!< the sample was taken at n = n_last
};

/** @ingroup float
 * @struct sig_interpolate_lin_param_f
 * @brief structure representing the parameters of a 'linear interpolation' signal
 * @details the 'linear interpolation' signal returns (a * (n - delay) + b)
 */
struct sig_interpolate_lin_param_f {
	float a;
	float b;
	n_t delay;
};

/** @ingroup float
 * @struct sig_iirlp1_param_f
 * @brief structure representing the parameters of a IIR Low Pass 1st order filter
 */
struct sig_iirlp1_param_f {
	float a;											//!< damping factor 0 <= n <= 1
	float oma;											//!< 1 - a
	n_t n_last;											//!< the evaluation was done at n = n_last
	struct signal_float *source;
};

/** @ingroup float
 * @struct sig_step_param_f
 * @brief structure representing the parameters of a step function
 */
struct sig_step_param_f {
	n_t n_min;											//!< lower boundary of the n-Window
	n_t n_max;											//!< upped boundary of the n-Window
	float x_active;										//!< output value when active
	float x_inact;										//!< output value when inactive
};

/** @ingroup float
 * @struct sig_fir_n_param_f
 * @brief structure representing the parameters of a generic n-tap FIR filter
 * you can design your filter with http://t-filter.appspot.com/fir/index.html
 */
struct sig_fir_n_param_f {
	int tap_count;										//!< how many taps are present
	int index_last;										//!< index is where the next input should be saved in the samples array
	float *taps;										//!< points to the taps array
	float *samples;										//!< points to the x[n-i] history of the source (samples of the source)
	struct signal_float *source;						//!< source signal for the filter
	n_t n_last;											//!< the evaluation was done at n = n_last
};

/** @ingroup float
 * @struct sig_pid_param_f
 * @brief structure representing the parameters of a PID controller
 * @details Since signals are not aware of computation period, I and D coefficients don't consider sampling rate. Thus, I and D gains should be corrected with sampling time (dt being the sample period):
 *   - The real D gain is d*dt
 *   - The real I gain is i/dt
 *   - P is the real gain
 * 
 * This is one of the reasons why care should be taken to compute PID algorithm at precise time.
 */
struct sig_pid_param_f {
	n_t n_last;											//!< the evaluation was done at n = n_last
	float p;											//!< Proportional gain
	float i;											//!< Integral gain
	float d;											//!< Derivative gain
	float k[3];											//!< K-params calculated from P, I and D terms. Used by optimized form. Computed from p, i and d by sig_pid_compute_k_f()
	float max_output;									//!< output limit. Also used for anti-windup
	float integral;										//!< integral term (for naive form)
	float history[3];									//!< history of the erro input (to compute d-term)
	struct signal_float *setpoint;						//!< setpoint (target). Error input if feedback is NULL
	struct signal_float *feedback;						//!< feedback (measured output). Set to NULL to use setpoint as an error input
#if (SIG_PID_FF) || defined(__DOXYGEN__)
	float ff[3];										//!< Feed-Forward parameters
	float sh[2];										//!< setpoint history for Feed-Forward computation
#endif
};

/** @ingroup float
 * @struct sig_buf_read_param_f
 * @brief structure representing the parameters of a buffer reader
 */
struct sig_buf_read_param_f {
	float *buffer;										//!< points to the data buffer
	int size;											//!< Size of the buffer. If the buffer is circular, the index will be modulo size
	int delta;											//!< delta between data index and n. index = n + delta
	unsigned circular		: 1;						//!< if 1, buffer is circular (index = (n + delta) % size). If not, the output will stick when size is exceeded (index = min ((size-1), n + delta))
	unsigned check_buffer	: 1;						//!< if 1, throw an error if the buffer is NULL<; If 0, return x_cst until buffer is not NULL
	unsigned dummy			: 6;						//!< unused. Reserved for future use
	n_t n_last;											//!< the evaluation was done at n = n_last
};


/***************************************************************************************/
/*                              Function definitions                                   */
/***************************************************************************************/


/** @ingroup float
 * @brief evaluate the signal an return it's value
 * @details returns (in order of priority)
 * -# result of the @b (*x) function, if x != NULL
 * -# content of x_var pointer (if x_var != NULL)
 * -# x_cst
 * 
 * @param[in] self pointer to the signal structure
 * @param[in] n the value of n
 */
float sig_get_value_f(struct signal_float *self, n_t n);


/** @ingroup float
 *
 * @brief return sampled value of a variable
 * @details if n = n_last, then the cached value (x_cst) is returned.
 * else, the value pointed by (x_ptr) is copied in (x_cst) and then returned
 * 
 * @param[in] self pointer to the signal structure
 * @param[in] n the value of n
 */
float sig_sampler_f(struct signal_float *self, n_t n);



/** @ingroup float
 * @brief adds two parameters
 * @details if n = n_last, then the cached value (x_cst) is returned.
 * returns (a + b).
 * @see sig_add_param_f
 *
 * @param[in] self pointer to the signal structure
 * @param[in] n the value of n
 */
float sig_add_f(struct signal_float *self, n_t n);


/** @ingroup float
 * @brief linear interpolation (ax + b) form
 * @details if n = n_last, then the cached value (x_cst) is returned.
 * returns (a * (n - delay) + b).
 * @see sig_interpolate_lin_param_f
 *
 * @param[in] self pointer to the signal structure
 * @param[in] n the value of n
 */
float sig_interpolate_lin_f(struct signal_float *self, n_t n);


/** @ingroup float
 * @ingroup sig-func
 * @brief Infinite Impulse Response low-pass filter, first order
 * @details if n = n_last, then the cached value (x_cst) is returned.
 * returns y[n] = y[n-1](1 - a) + a*x[n].
 * @see sig_iirlp1_param_f
 *
 * @param[in] self pointer to the signal structure
 * @param[in] n the value of n
 */
float sig_iirlp1_f(struct signal_float *self, n_t n);


/** @ingroup float
 * @ingroup sig-func
 * @brief Finite Impulse Response filter
 * @details if n = n_last, then the cached value (x_cst) is returned.
 * returns y[n] = sum(from i=n to -tap_count, tap[i] * sample[i]).
 * @see sig_fir_n_param_f
 *
 * @param[in] self pointer to the signal structure
 * @param[in] n the value of n
 */
float sig_fir_n_f(struct signal_float *self, n_t n);

float sig_step_f(struct signal_float *self, n_t n);


/** @ingroup float
 * @ingroup sig-func
 * @brief PID computation, naive (trivial) version.
 * @details This version of the PID computation is not optimized for speed, but does not require
 * the K-params, so the P, I and D parameters can be changed on the fly without calling sig_pid_compute_k_f()
 * @see sig_pid_param_f
 *
 * @param[in] self pointer to the signal structure
 * @param[in] n the value of n
 */
float sig_pid_naive_f (struct signal_float *self, n_t n);


/** @ingroup float
 * @ingroup sig-func
 * @brief PID computation, optimized version
 * @details Used the K-params to save multiple sycles or computation.
 * the function sig_pid_compute_k_f() must be called before using the optimized form.
 * @see sig_pid_param_f
 *
 * @param[in] self pointer to the signal structure
 * @param[in] n the value of n
 */
float sig_pid_opt_f (struct signal_float *self, n_t n);


/** @ingroup float
 * @ingroup sig-func
 * @brief PID K-params computation. Must be used each time the P,I or D parameters are modified
 * @details k[0] = P + I + D
 * k[1] = -P - 2 * D
 * k[2] = D
 * @see sig_pid_param_f
 *
 * @param[in] self pointer to the signal structure
 * @param[in] n the value of n
 */
void sig_pid_compute_k_f (struct signal_float *self);


/** @ingroup float
 * @ingroup sig-func
 * @brief buffer reader. Outputs the content of a buffer
 * @details The buffer reader has two behaviours depending if circular is set or not.
 * @see sig_buf_read_param_f
 *
 * @param[in] self pointer to the signal structure
 * @param[in] n the value of n
 */
float sig_buf_read_f (struct signal_float *self, n_t n);


#endif
