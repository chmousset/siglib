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
#include "sig.h"
#include "sigf.h"

int test_pidf(float **data, int data_l, float* output)
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
	struct signal_float pid_setpoint = {.x = (sig_func_f)sig_buf_read_f, .params = &pid_setpoint_p};
	
	// setpoint buffer reader
	struct sig_buf_read_param_f pid_feedback_p = {
		.buffer = data[1],
		.size = data_l,
		.delta = 0,
		.circular = 0,
		.check_buffer = 1,
		.n_last = 0
	};
	struct signal_float pid_feedback = {.x = (sig_func_f)sig_buf_read_f, .params = &pid_feedback_p};
	
	// setpoint buffer reader
	struct sig_buf_read_param_f pid_ff0_p = {
		.buffer = data[2],
		.size = data_l,
		.delta = 0,
		.circular = 0,
		.check_buffer = 1,
		.n_last = 0
	};
	struct signal_float pid_ff0 = {.x = (sig_func_f)sig_buf_read_f, .params = &pid_ff0_p};
	
	// setpoint buffer reader
	struct sig_buf_read_param_f pid_ff1_p = {
		.buffer = data[3],
		.size = data_l,
		.delta = 0,
		.circular = 0,
		.check_buffer = 1,
		.n_last = 0
	};
	struct signal_float pid_ff1 = {.x = (sig_func_f)sig_buf_read_f, .params = &pid_ff1_p};
	
	// setpoint buffer reader
	struct sig_buf_read_param_f pid_ff2_p = {
		.buffer = data[4],
		.size = data_l,
		.delta = 0,
		.circular = 0,
		.check_buffer = 1,
		.n_last = 0
	};
	struct signal_float pid_ff2 = {.x = (sig_func_f)sig_buf_read_f, .params = &pid_ff2_p};
	
	
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
	struct signal_float pid = {
		.x = (sig_func_f)sig_pid_opt_f,
		.x_var = NULL,
		.x_cst = 0.0,
		.params = (void*)&pid_p
	};
	sig_pid_compute_k_f(&pid);
	
	n_t n;
	for(n=0; n<data_l; n++)
		output[n] = sig_get_value_f(&pid, n);
	
	return 0;
}
