/**
 * sigLib, simple Signals Library
 * 
 * Copyright (C) 2013 Charles-Henri Mousset
 * 
 * This file is part of sigLib.
 * 
 * sigLib is free software: you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * sigLib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU Lesser General Public License for more details. You should have received a copy of the GNU
 * General Public License along with sigLib. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Authors: Charles-Henri Mousset
 */

/** \file scope.h
 * Scope Header
 */

#ifndef SIG_SCOPE_H__
#define SIG_SCOPE_H__

#include <stdlib.h>
#include "sig.h"

/** @addtogroup config
 * @{
 */

/** @ingroup scope
 * @brief If TRUE, the Scope can use signal_float
 */
#if !defined(SCOPE_USE_FLOAT) || defined(__DOXYGEN__)
	#define SCOPE_USE_FLOAT		TRUE
#endif

/** @ingroup scope
 * @brief If TRUE, the Scope can use signal_int
 */
#if !defined(SCOPE_USE_INT) || defined(__DOXYGEN__)
	#define SCOPE_USE_INT		TRUE
#endif

/** @ingroup scope
 * @brief Define max amount of signals sampled simulteanously
 */
#if !defined(SCOPE_MAX_SIGNALS) || defined(__DOXYGEN__)
	#define SCOPE_MAX_SIGNALS	16
#endif

/** @ingroup scope
 * @brief Amount of signals referenced (known).
 * Ignered if @SIG_DBG_NAME is FALSE
 */
#if !defined(SCOPE_MAX_SIGNALS_LIST) || defined(__DOXYGEN__)
	#define SCOPE_MAX_SIGNALS_LIST	64
#endif

/** @} */

/** @ingroup scope
 * @struct scope_type
 * @brief structure representing a signal Scope
 */
typedef struct scope_type
{
	#if defined(SCOPE_USE_INT) || defined(__DOXYGEN__)
		struct signal_int *signals_int[SCOPE_MAX_SIGNALS];				//!< List of signal_int* to be sempled
		int signals_count_int;											//!< number of signals in the list
	#endif
	#if defined(SCOPE_USE_FLOAT) || defined(__DOXYGEN__)
		struct signal_float *signals_float[SCOPE_MAX_SIGNALS];			//!< List of signals_float* to be sempled
		int signals_count_float;										//!< number of signals in the list
	#endif

	#if defined(SIG_DBG_NAME) || defined(__DOXYGEN__)
		char signals_names[SCOPE_MAX_SIGNALS * SIG_DBG_NAME_LENGHT];	//!< Names of the signals, ',' separeted

		#if defined(SCOPE_USE_INT) || defined(__DOXYGEN__)
			struct signal_int *signals_list_i[SCOPE_MAX_SIGNALS_LIST];	//!< Available (known) signal_int
			int signal_list_count_i;									//!< Number of known signal_int
		#endif
		#if defined(SCOPE_USE_FLOAT) || defined(__DOXYGEN__)
			struct signal_float *signals_list_f[SCOPE_MAX_SIGNALS_LIST];//!< Available (known) signals_float
			int signal_list_count_f;									//!< Number of known signal_int
		#endif
	#endif

	void *buffer;				//!< points to the memory dedicated to the buffer
	void *next_data;			//!< points to the next address where the signal's value must be saved
	int buffer_size_bytes;		//!< size (in bytes) of the buffer memory
	int buffer_size;			//!< buffer size (in samples)
	int samples;				//!< samples already collected
	enum scope_state_t {SCOPE_INIT, SCOPE_READY, SCOPE_SAMPLING, SCOPE_SAMPLED} state;	//!< state of the scope
	int prediv;					//!< Sampling predivisor
	int count;					//!< counter used by the predivisor

} scope_t;


/** @ingroup scope
 * scope scope init
 * @pre: none
 * @post: the internal variables are reset, and the state is set to @SCOPE_INIT.
 * @param[in] self : pointer to the scope_t struct
 * @param[in] *data : pointer to the memory used as buffer by the scope
 * @param[in] size : size of the buffer in bytes
 */
void scope_init(scope_t *self, void *data, int size);


/** @ingroup scope
 * setup the scope: signals,
 * @detils if @SIG_DBG_NAME is TRUE, the function searches for the signals names 
 * @param[in] self : pointer to the scope_t sctuct
 * @param[in] *signals : list of signal names to sample.
 *   these are ',' separated, and searched into @signals_list_int and @signals_list_float.
 *   ignored if @SIG_DBG_NAME is FALSE, you have to fill @signal_int and @signal_float yourself
 * @param[in] prediv predivisor for the sampling period
 */
void scope_setup(scope_t *self, char *signals, int prediv);

#if(SCOPE_USE_INT) || defined(__DOXYGEN__)
/** @ingroup scope
 * add a signal to the list of known signals
 * @param[in] self : pointer to the scope_t sctuct
 * @param[in] *signal pointer to a struct @signal_int
 * @return number of signals that can still be added (-1 if @sig was not added)
 */
	int scope_enlist_sig_int(scope_t *self, struct signal_int *sig);
#endif

#if(SCOPE_USE_FLOAT) || defined(__DOXYGEN__)
/** @ingroup scope
 * add a signal to the list of known signals
 * @param[in] self : pointer to the scope_t sctuct
 * @param[in] *signal pointer to a struct @signal_float
 * @return number of signals that can still be added (-1 if @sig was not added)
 */
	int scope_enlist_sig_float(scope_t *self, struct signal_float *sig);
#endif

/** @ingroup scope
 * returns the buffer depth in samples (how many samples the scope can hold)
 * @param[in] self : pointer to the scope_t sctuct
 */
int scope_max_samples(scope_t *self);

/** @ingroup scope
 * update the scope.
 * If the scope must sample, then it samples.
 * @param[in] self : pointer to the scope_t sctuct
 */
void scope_update(scope_t *self, n_t n);

#endif
