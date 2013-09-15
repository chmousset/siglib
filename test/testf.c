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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "test_pidf.h"


int main ( int argc, char *argv[])
{
	float *data_out, **data;
	int data_l;
	char filename[1024];
	
	if(argc < 2)
	{
		printf("please input a file name prefix for the output\n");
		return -1;
	}
	
	// Read CSV for input
	sprintf(filename, "%s_pidf.csv", argv[1]);
	if(csv_load(filename, &data_l, &data))
	{
		printf("cannot load %s\n", filename);
		return -1;
	}
	data_out = malloc(sizeof(float) * data_l);
	test_pidf(data, data_l, data_out);
	csv_free(data);
	free(data_out);
	
	return 0;
}
