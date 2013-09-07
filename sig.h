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


/** \file sig.h
 * SigLib Header
 */

#ifndef SIG_LIB_H__
#define SIG_LIB_H__

#include "sigconf.h"


#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE	1
#endif


#if !defined(FALSE) || defined(__DOXYGEN__)
#define FLASE	0
#endif

#define max(a,b) (a>b? a : b)
#define min(a,b) (a<b? a : b)
#define abs(a) (a>0? a : -a)


/** @addtogroup config
 * @{
 */
#if !defined(SIG_DBG_NAME) || defined(__DOXYGEN__)
#define SIG_DBG_NAME	TRUE						//!< if TRUE, signals are named. Set to FALSE to save data space
#endif


#if !defined(SIG_DBG_NAME_LENGHT) || defined(__DOXYGEN__)
#define SIG_DBG_NAME_LENGHT		64					//!< Length (in chars) of the maximum signal name. Default is 64, tweak for memory optimization
#endif

/** Specify the type of 'n'. @warning default is @c unsigned @c int . Changing this for any other thing should be done carefully and checking all used sig-func is recommended! */
typedef unsigned int n_t;

/** @} */


/** @addtogroup siglib
 * @{
 */
extern int sig_errno;								//!< last Error number. If non-zero, all signals exit returning 0
extern void *sig_err_ptr;							//!< points to the signal struct that had error

#if defined(SIG_DBG_NAME) || defined(__DOXYGEN__)
extern char sig_err_name[SIG_DBG_NAME_LENGHT];		//!< name of the signal that had an error
#endif

/** @brief returns 1 if 'n' is valid in the signal *sig 's N-Window
 * @see n-Window
 */
#define SIG_NWINDOW_VALID(n, params) \
	(params->n_min > params->n_max ? \
			( (n >=  params->n_min) || (n <= params->n_max ) ? 1 : 0)\
			: ( (n >= params->n_min) && (n <= params->n_max) ? 1 : 0))

#define SIG_ERR_NO_SELF			-1					//!< the signal pointer was not passed correctly to the function
#define SIG_ERR_NO_CONFIG		-2					//!< the signal should contain parameters, but it's pointer is NULL
#define SIG_ERR_NWINDOW			-3					//!< 'n' was out of the signal's N-validity windows

/**
 * @def SIG_ERRNO_FAIL
 * @brief Checks if an error occurred
 * @details It is used at each start of signal evaluation ((*x) function).
 * It causes the function return 0 immediately if an error has previously occurred.
 * @pre should be called from (*x) function
 */
#define SIG_ERRNO_FAIL if(sig_errno) \
	return 0;

/**
 * @def SIG_ERRNO(a)
 * @brief end function and fill errno
 * @details Called by a signal evaluation function (*x) on error.
 * Fills the sig_errno variable and copies signal's name in sig_err_name (if SIG_DBG_NAME is defined).
 * Returns 0
 * @pre should be called from (*x) function
 */
#if SIG_DBG_NAME || defined(__DOXYGEN__)
	#define SIG_ERRNO(a) {sig_errno = a; \
	strcpy(sig_err_name, self->name); \
	sig_err_ptr = self; \
	return 0;}
#else
	#define SIG_ERRNO(a) {sig_errno = a; \
	sig_err_ptr = self; \
	return 0;}
#endif

/**
 * generic macro to get the value of a signal. It's advantage is that it's type independent and inline so this should help with speed.
 */
#define sig_value(s,n) (sig_errno !=0 ? 0 : ( (s)->x != NULL ? (s)->x((s), n) : ( (s)->x_var ? *((s)->x_var) : (s)->x_cst ) ) )


/** @} */

/** @ingroup float
 * @struct signal_float
 * @brief structure representing a floating-point signal
 */
struct signal_float {
#if SIG_DBG_NAME || defined(__DOXYGEN__)
	char name[SIG_DBG_NAME_LENGHT];						//!< name of the signal
#endif
	float (*x)(struct signal_float *self, int n);		//!< pointer to the evaluation function (*x)
	float *x_var;										//!< points to a variable. used if x == NULL
	float x_cst;										//!< constant value. used if x == NULL && x_var == NULL
	void *params;										//!< points to the signal parameter(s), if any.
};
typedef float (*sig_func_f)(struct signal_float *self, int n);

/** @ingroup int
 * @struct signal_int
 * @brief structure representing an integer signal
 */
struct signal_int {
#if SIG_DBG_NAME || defined(__DOXYGEN__)
	char name[SIG_DBG_NAME_LENGHT];						//!< name of the signal
#endif
	int (*x)(struct signal_int *self, int n);			//!< pointer to the evaluation function (*x)
	int *x_var;											//!< points to a variable. used if x == NULL
	int x_cst;											//!< constant value. used if x == NULL && x_var == NULL
	void *params;										//!< points to the signal parameter(s), if any.
};

#endif
