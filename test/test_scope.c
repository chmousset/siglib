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

#include <stdlib.h>
#include <stdio.h>
#include "sig.h"
#include "sigf.h"
#include "scope.h"

scope_t scope;

static char scope_data[1024];
char signals[] = "pid_out,inexistant,pid_feedback";

int test_scope(float **data, int data_l, float* output)
{	
	// setpoint buffer reader
	struct sig_buf_read_param_f pid_setpoint_p = {
		.buffer = data[0],
		.size = data_l,
		.delta = 0,
		.circular = 0,
		.check_buffer = 1,
		.n_last = 0
	};
	struct signal_float pid_setpoint = SIGN_FN("setpoint", sig_buf_read_f, &pid_setpoint_p);
	
	// setpoint buffer reader
	struct sig_buf_read_param_f pid_feedback_p = {
		.buffer = data[1],
		.size = data_l,
		.delta = 0,
		.circular = 0,
		.check_buffer = 1,
		.n_last = 0
	};
	struct signal_float pid_feedback = SIGN_FN("pid_feedback", sig_buf_read_f, &pid_feedback_p);
	
	// setpoint buffer reader
	struct sig_buf_read_param_f pid_ff0_p = {
		.buffer = data[2],
		.size = data_l,
		.delta = 0,
		.circular = 0,
		.check_buffer = 1,
		.n_last = 0
	};
	struct signal_float pid_ff0 = SIGN_FN("ff0", sig_buf_read_f, &pid_ff0_p);
	
	// setpoint buffer reader
	struct sig_buf_read_param_f pid_ff1_p = {
		.buffer = data[3],
		.size = data_l,
		.delta = 0,
		.circular = 0,
		.check_buffer = 1,
		.n_last = 0
	};
	struct signal_float pid_ff1 = SIGN_FN("ff1", sig_buf_read_f, &pid_ff1_p);
	
	// setpoint buffer reader
	struct sig_buf_read_param_f pid_ff2_p = {
		.buffer = data[4],
		.size = data_l,
		.delta = 0,
		.circular = 0,
		.check_buffer = 1,
		.n_last = 0
	};
	struct signal_float pid_ff2 = SIGN_FN("ff2", sig_buf_read_f, &pid_ff2_p);
	
	
	// PID controller
	struct sig_pid_param_f pid_p= {
		.p = 1.0,
		.i = 0.1,
		.d = 1.0,
		.k = {0.0, 0.0, 0.0},
		.max_output = 5.0,
		.integral = 0.0,
		.history = {0.0, 0.0, 0.0},
		.setpoint = &pid_setpoint,
		.feedback = &pid_feedback,
		.ff = {1.0, 2.0, 3.0},
		.sh = {0.0, 0.0},
		.ff0 = &pid_ff0,
		.ff1 = &pid_ff1,
		.ff2 = &pid_ff2
	};
	struct signal_float pid = SIGN_FN("pid_out", sig_pid_opt_f, &pid_p);

	sig_pid_compute_k_f(&pid);

	scope_init(&scope, scope_data, sizeof(scope_data));
	scope_enlist_sig_float(&scope, &pid);
	scope_enlist_sig_float(&scope, &pid_ff2);
	scope_enlist_sig_float(&scope, &pid_feedback);
	scope_setup(&scope, signals, 1);
	
	n_t n;
	for(n=0; n<data_l; n++)
	{
		output[n] = sig_get_value_f(&pid, n);
		scope_update(&scope, n);
	}

	printf("scope got %d samples\n", scope.samples);
	printf("scope state is %d (is SAMPLED:%d)\n", scope.state, (scope.state == SCOPE_SAMPLED ? 1 : 0));
	printf("scope signals count: %d\n", scope.signals_count_float);
	
	int i;
	for(i=0; i< scope.signals_count_float; i++)
		printf("%s%c", scope.signals_float[i]->name, (i == (scope.signals_count_float - 1) ? '\n': ','));

	for(i=0; i<(scope.samples * scope.signals_count_float); i++)
		printf("%f%c", ((float*)scope.buffer)[i], ( (i+1) % scope.signals_count_float ? ',' : '\n'));
	
	return 0;
}
