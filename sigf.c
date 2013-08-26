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


/** \file sig.c
 * SigLib Code
 */

#include <string.h>
#include "sigf.h"


float sig_get_value_f(struct signal_float *self, n_t n)
{
	SIG_ERRNO_FAIL
	
	if(self->x)
		return self->x(self, n);
	if(self->x_var)
		return *self->x_var;
	return self->x_cst;
}


float sig_sampler_f(struct signal_float *self, n_t n)
{
	SIG_ERRNO_FAIL
	
	if(self == NULL)
		SIG_ERRNO(-1);
	
	if(self->params == NULL)
		SIG_ERRNO(-2);
	
	if (((struct sig_sampler_param_f*)self->params)->n_last == n)
		return self->x_cst;
	self->x_cst = *self->x_var;
	((struct sig_sampler_param_f*)self->params)->n_last = n;
	return self->x_cst;
}

float sig_add_f(struct signal_float *self, n_t n)
{
	float a, b;
	struct sig_add_param_f *ptr;
	
	SIG_ERRNO_FAIL
	
	if(self == NULL)
		SIG_ERRNO(-1);
	
	if(self->params == NULL)
		SIG_ERRNO(-2);
	
	ptr = (struct sig_add_param_f*)self->params;

	if (ptr->n_last == n)
		return self->x_cst;

	if(ptr->a)
		a = sig_value(ptr->a, n);		//a = sig_get_value_f(ptr->a, n);
	else if(ptr->a_var)
		a = *ptr->a_var;
	else
		a = ptr->a_cst;
	
	SIG_ERRNO_FAIL
		
	if(ptr->b)
		b = sig_value(ptr->b, n);		//b = sig_get_value_f(ptr->b, n);
	else if(ptr->b_var)
		b = *ptr->b_var;
	else
		b = ptr->b_cst;
	self->x_cst = a+b;
	return self->x_cst;
}

float sig_interpolate_lin_f(struct signal_float *self, n_t n)
{
	
	return 0.0;
}

float sig_iirlp1_f(struct signal_float *self, n_t n)
{
	struct sig_iirlp1_param_f *ptr = (struct sig_iirlp1_param_f *) self->params;
	float source_value;
	SIG_ERRNO_FAIL

	if(self == NULL)
		SIG_ERRNO(-1);

	if(self->params == NULL)
		SIG_ERRNO(-2);

	if (n == ptr->n_last)
		return self->x_cst;
	
	source_value = sig_value(ptr->source, n);		//source_value = sig_get_value_f(ptr->source, n);

	self->x_cst = (self->x_cst * ptr->oma) +  (source_value * ptr->a);
	ptr->n_last = n;
	return self->x_cst;
}


float sig_step_f(struct signal_float *self, n_t n)
{
	struct sig_step_param_f *ptr = (struct sig_step_param_f *)self->params;
	SIG_ERRNO_FAIL

	if(self == NULL)
		SIG_ERRNO(-1);

	if(self->params == NULL)
		SIG_ERRNO(-2);

	if (SIG_NWINDOW_VALID(n, ptr))
		return ptr->x_active;
	else
		return ptr->x_inact;
}

float sig_fir_n_f(struct signal_float *self, n_t n)
{
	struct sig_fir_n_param_f *ptr = (struct sig_fir_n_param_f *) self->params;
	int i;
	int index;
	SIG_ERRNO_FAIL

	if(self == NULL)
		SIG_ERRNO(-1);

	if(self->params == NULL)
		SIG_ERRNO(-2);

	if (n == ptr->n_last)
		return self->x_cst;

	ptr->samples[ptr->index_last++] = sig_value(ptr->source, n);		// store the input into the buffer
	ptr->index_last %= ptr->tap_count;										// make sure the index rollback
	index = ptr->index_last;

	self->x_cst = 0;														// this is the output of the filter, initialize it at 0
	for (i=0; i< ptr->tap_count; i++)
	{
		index = index != 0 ? index - 1 : ptr->tap_count-1;
		self->x_cst += ptr->samples[i] * ptr->taps[index];					// MAC the samples by the taps
	}
	return self->x_cst;
}


float sig_pid_opt_f (struct signal_float *self, n_t n)
{
	struct sig_pid_param_f *ptr = (struct sig_pid_param_f *) self->params;
	float output, error;
	int i;
	if (ptr == NULL)
		SIG_ERRNO(-1);
	
	if (n == ptr->n_last)
		return self->x_cst;
	
	if (n == ptr->n_last)
		return self->x_cst;
	
	// get the current error
	error = sig_get_value_f(ptr->setpoint, n);
	if (ptr->feedback)
		error -= sig_get_value_f(ptr->feedback, n);
	
	// push new error sample into the history
	ptr->history[2] = ptr->history[1];
	ptr->history[1] = ptr->history[0];
	ptr->history[0] = error;
	
	// compute the PID output
	self->x_cst += ptr->history[0] * ptr->k[0];
	self->x_cst += ptr->history[1] * ptr->k[1];
	self->x_cst += ptr->history[2] * ptr->k[2];
	
	// limit the output to max_output
	if (self->x_cst > ptr->max_output)
		self->x_cst = ptr->max_output;
	else if (self->x_cst < (-1 * ptr->max_output))
		self->x_cst = -1 * ptr->max_output;
	
	return self->x_cst;
}


float sig_pid_naive_f (struct signal_float *self, n_t n)
{
	struct sig_pid_param_f *ptr = (struct sig_pid_param_f *) self->params;
	float output, error;
	int i;
	if (ptr == NULL)
		SIG_ERRNO(-1);
	
	if (n == ptr->n_last)
		return self->x_cst;
	
	if (n == ptr->n_last)
		return self->x_cst;
	
	// get the current error
	error = sig_get_value_f(ptr->setpoint, n);
	if (ptr->feedback)
		error -= sig_get_value_f(ptr->feedback, n);
	
	// push new error sample into the history
	ptr->history[2] = ptr->history[1];
	ptr->history[1] = ptr->history[0];
	ptr->history[0] = error;
	
	// compute the PID output
	self->x_cst = ptr->history[0] * ptr->p;
	ptr->integral += ptr->history[0] * ptr->i;
	self->x_cst += ptr->integral;
	self->x_cst += (ptr->history[0] - ptr->history[1]) * ptr->d;
	
	
	// limit the integral part to max_output
	if (ptr->integral > ptr->max_output)
		ptr->integral = ptr->max_output;
	else if (ptr->integral < (-1 * ptr->max_output))
		ptr->integral = -1 * ptr->max_output;
	
	// limit the output to max_output
	if (self->x_cst > ptr->max_output)
		self->x_cst = ptr->max_output;
	else if (self->x_cst < (-1 * ptr->max_output))
		self->x_cst = -1 * ptr->max_output;
	
	return self->x_cst;
}


void sig_pid_compute_k_f (struct signal_float *self)
{
	struct sig_pid_param_f *ptr = (struct sig_pid_param_f *) self->params;
	if (ptr == NULL)
		SIG_ERRNO(-1);
	ptr->k[0] = ptr->p + ptr->i + ptr->d;
	ptr->k[1] = -1 * ptr->p - 2 * ptr->i;
	ptr->k[2] = ptr->d;
}
