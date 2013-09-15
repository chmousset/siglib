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

#ifndef CSV_H_
#define CSV_H_


#define CSV_BUFF_LEN	1024	//!< Lenght of the buffer used to read a single line from CSV file
#define FILE_PATH_LEN	1024	//!< max file path lenght


/**
 * @brief loads a CSV file, and reads its data into a table
 * @param[in] path path to the file
 * @param[in] data pointer to a float[][]. The application using this function must provide
 * a pointer to a float[][] element. After the operation completed, the variable pointer by this
 * variable contains the address of an array of pointers to array of floats.
 * @return 0 if no error
 */
int csv_load(char *path, int *size, float ***data);


/**
 * @brief displays the data read from a CSV file
 * @param[in] csv pointer to a float[][]
 * @param[in] csv_l number of lines to display
 * @return 0 if no error
 */
int csv_display(float **csv, int csv_l);


/**
 * @brief Free the data retreived from a CSV file
 * @param[in] csv pointer to a float[]
 */
void csv_free(float **csv);


#endif	// CSV_H_