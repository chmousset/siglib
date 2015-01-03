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

#ifndef TEST_SCOPE_H_
#define TEST_SCOPE_H_


/**
 * @brief test the PID regulator, floating-point version
 * @param[in] data array of array of float
 * @param[in] data_l number of elements in the arrays
 * @param[in] output array the test will write the PID output from
 * @return 0 on success
 */
int test_scope(float **data, int data_l, float* output);


#endif	// TEST_PIDF_H_
