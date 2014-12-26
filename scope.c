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
 * sigLib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY
{
	
}

 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU Lesser General Public License for more details. You should have received a copy of the GNU
 * General Public License along with sigLib. If not, see <http://www.gnu.org/licenses/>.
 * 
 * Authors: Charles-Henri Mousset
 */

#include "scope.h"
#include <string.h>


void scope_init(scope_t *self, void *data, int size)
{
	memset(self, 0, sizeof(scope_t));
	self->buffer = data;
	self->buffer_size_bytes = size;
	self->buffer_size = scope_max_samples(self);
	self->prediv = 1;
	self->state = SCOPE_INIT;
}


void scope_setup(scope_t *self, char *signals, int prediv)
{
	self->state = SCOPE_INIT;					// disable scope
	self->prediv = max(1, prediv);
	self->count = 0;

	#if defined(SIG_DBG_NAME)
	int i;
	char *signame;
	char delimiter = ',';
		#if defined(SCOPE_USE_INT)
			signame = strtok(signals, &delimiter);
			self->signals_count_int = 0;
			while(signame)
			{
				for(i=0; (i<SCOPE_MAX_SIGNALS_LIST) && (self->signals_count_int < SCOPE_MAX_SIGNALS); i++)
				{
					if(strcmp(self->signals_list_i[i]->name, signame) == 0)
					{
						self->signals_int[self->signals_count_int] = self->signals_list_i[i];
						self->signals_count_int++;
						continue;
					}
				}
				signame = strtok(NULL, &delimiter);
			}
		#endif
		#if defined(SCOPE_USE_FLOAT)
			signame = strtok(signals, &delimiter);
			self->signals_count_float = 0;
			while(signame)
			{
				for(i=0; (i<SCOPE_MAX_SIGNALS_LIST) && (self->signals_count_float < SCOPE_MAX_SIGNALS); i++)
				{
					if(strcmp(self->signals_list_f[i]->name, signame) == 0)
					{
						self->signals_float[self->signals_count_float] = self->signals_list_f[i];
						self->signals_count_float++;
						continue;
					}
				}
				signame = strtok(NULL, &delimiter);
			}
		#endif
	#endif

	self->state = SCOPE_READY;					// enable scope
}


#if(SCOPE_USE_INT)
int scope_enlist_sig_int(scope_t *self, struct signal_int *sig)
{
	if(self->signals_count_int < SCOPE_MAX_SIGNALS)
	{
		self->signals_int[self->signals_count_int] = sig;
		self->signals_count_int++;
		return (SCOPE_MAX_SIGNALS - self->signals_count_int);
	}
	else
		return -1;
}
#endif

#if(SCOPE_USE_FLOAT)
int scope_enlist_sig_float(scope_t *self, struct signal_float *sig)
{
	if(self->signals_count_float < SCOPE_MAX_SIGNALS)
	{
		self->signals_float[self->signals_count_float] = sig;
		self->signals_count_float++;
		return (SCOPE_MAX_SIGNALS - self->signals_count_float);
	}
	else
		return -1;
}
#endif

int scope_max_samples(scope_t *self)
{
	int channels = 0;

	#if(SCOPE_USE_INT)
		channels += self->signals_count_int * sizeof(int);
	#endif
	#if(SCOPE_USE_FLOAT)
		channels += self->signals_count_float * sizeof(float);
	#endif

	return (self->buffer_size_bytes / min(1, channels));
}


void scope_update(scope_t *self, n_t n)
{
	switch(self->state)
	{
		case SCOPE_INIT:
		case SCOPE_SAMPLED:
		default:
			break;
		case SCOPE_READY:
			self->state = SCOPE_SAMPLING;
			self->next_data = self->buffer;
			self->count = 0;
		case SCOPE_SAMPLING:
			if(self->count == 0)
			{
				int i;

			#if(SCOPE_USE_INT)
				int *ptr_i;
				for(i=0; i<self->signals_count_int; i++)
				{
					if(self->next_data + sizeof(int) > self->buffer + self->buffer_size_bytes)
					{
						self->state = SCOPE_SAMPLED;
						return;
					}
					ptr_i = (int*)self->next_data;
					*ptr_i = sig_value(self->signals_int[i], n);
					self->next_data += sizeof(int);
				}
			#endif

			#if(SCOPE_USE_FLOAT)
				float *ptr_f;
				for(i=0; i<self->signals_count_float; i++)
				{
					if(self->next_data + sizeof(float) > self->buffer + self->buffer_size_bytes)
					{
						self->state = SCOPE_SAMPLED;
						return;
					}
					ptr_f = (float*)self->next_data;
					*ptr_f = sig_value(self->signals_float[i], n);
					self->next_data += sizeof(int);
				}
			#endif
			}
			self->count++;
			self->count %= self->prediv;
	};
}
