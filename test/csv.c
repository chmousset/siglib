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


 int csv_load(char *path, int *size, float ***data)
 {
	FILE *fd;
	char *cell, line[CSV_BUFF_LEN], *ret;
	int i, ncols = 0, nlines = 0, curr_line = 0;
	float **output;
	fd = fopen(path, "r");
	if(fd == NULL)
	{
		printf("can't open file %s\n", path);
		return -1;
	}
	rewind(fd);
	
	// get first line to get the header
	ret = fgets(line, CSV_BUFF_LEN, fd);
	if(ferror(fd))
	{
		printf("can't read file\n");
		return -2;
	}
	cell = strtok(line, ",;\t");
	while (cell)
	{
		++ncols;
		cell = strtok(NULL,",;\t");
	}
	
	// create a table containing pointers to the data tables
	output = calloc(ncols + 1, sizeof(float *));
	*data = output;
	
	// count the lines in our file
	do
	{
		++nlines;
		ret = fgets(line, CSV_BUFF_LEN, fd);
	} while (ret);
	
	// get memory for that
	for(i=0; i<ncols; i++)
	{
		output[i] = malloc(nlines * sizeof(float));
		if(output[i] == NULL)
		{
			printf("cannot allocate memory for the data of colum %d (%d lines)\n", i, nlines);
			return -3;
		}
	}
	
	// got to the start of the file, and ignore first line
	rewind(fd);
	ret = fgets(line, CSV_BUFF_LEN, fd);
	
	// read the first line of data
	ret = fgets(line, CSV_BUFF_LEN, fd);
	while (feof(fd) == 0)
	{
		if(ferror(fd))
		{
			printf("can't read file at line %d (data index %d)\n", curr_line+1, curr_line);
			return -2;
		}
		
		i = 0;
		cell = strtok(line, ",;\t");
		while(cell)
		{
			if(i >= ncols)
			{	
				printf("line %d has %d columns, expecting %d\n", curr_line+1, i, ncols);
				return -3;
			}
			output[i][curr_line] = strtof(cell, NULL);
			cell = strtok(NULL, ",;\t");
			i++;
		}
		if(i < ncols)
		{
			
			printf("line %d has %d columns, expecting %d\n", curr_line+1, i, ncols);
			return -4;
		}
		
		curr_line++;
		// get a line of values
		ret = fgets(line, CSV_BUFF_LEN, fd);
	}
	*size = nlines-1;
	return 0;
}


int csv_display(float **csv, int csv_l)
{
	int csv_cols=0, i, j;
	
	if (csv == NULL)
		return -1;
	while (csv[csv_cols])
		++csv_cols;
	printf("size of CSV is %d columns, %d lines of data\n", csv_cols, csv_l);
	
	for(i=0; i < csv_l; i++)
	{
		for(j=0; j < csv_cols; j++)
		{
			printf("%f", csv[j][i]);
			if(j < csv_cols-1)
				printf(", ");
			else
				printf("\n");
		}
	}
	return 0;
}


void csv_free(float **csv)
{
	int i=0;
	
	if(csv == NULL)
		return;
	
	while(csv[i])
		free(csv[i++]);
	
	free(csv);
}
