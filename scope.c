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
			memcpy(self->signals_names, signals, min(SCOPE_MAX_SIGNALS * SIG_DBG_NAME_LENGHT, strlen(signals)));
			signame = strtok(self->signals_names, &delimiter);
			self->signals_count_int = 0;
			while(signame)
			{
				for(i=0; (i<SCOPE_MAX_SIGNALS_LIST) && (self->signals_count_int < SCOPE_MAX_SIGNALS); i++)
				{
					if(self->signals_list_i[i] == 0)
						break;
					if(strcmp(self->signals_list_i[i]->name, signame) == 0)
					{
						self->signals_int[self->signals_count_int] = self->signals_list_i[i];
						self->signals_count_int++;
						break;
					}
				}
				signame = strtok(NULL, &delimiter);
			}
		#endif
		#if defined(SCOPE_USE_FLOAT)
			memcpy(self->signals_names, signals, min(SCOPE_MAX_SIGNALS * SIG_DBG_NAME_LENGHT, strlen(signals)));
			signame = strtok(self->signals_names, &delimiter);
			self->signals_count_float = 0;
			while(signame)
			{
				for(i=0; (i<SCOPE_MAX_SIGNALS_LIST) && (self->signals_count_float < SCOPE_MAX_SIGNALS); i++)
				{
					if(self->signals_list_f[i] == 0)
					{
						break;
					}
					if(strcmp(self->signals_list_f[i]->name, signame) == 0)
					{
						self->signals_float[self->signals_count_float] = self->signals_list_f[i];
						self->signals_count_float++;
						break;
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
	if(self->signal_list_count_i < SCOPE_MAX_SIGNALS)
	{
		self->signals_list_i[self->signal_list_count_i] = sig;
		self->signal_list_count_i++;
		return (SCOPE_MAX_SIGNALS - self->signal_list_count_i);
	}
	else
		return -1;
}
#endif

#if(SCOPE_USE_FLOAT)
int scope_enlist_sig_float(scope_t *self, struct signal_float *sig)
{
	if((self->signal_list_count_f < SCOPE_MAX_SIGNALS) && *sig->name)
	{
		self->signals_list_f[self->signal_list_count_f] = sig;
		self->signal_list_count_f++;
		return (SCOPE_MAX_SIGNALS - self->signal_list_count_f);
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
			self->samples++;
			self->count++;
			self->count %= self->prediv;
	};
}
