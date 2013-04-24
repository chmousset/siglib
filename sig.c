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
#include "sig.h"

int sig_errno = 0;
void *sig_err_ptr = NULL;

#ifdef SIG_DBG_NAME
char sig_err_name[SIG_DBG_NAME_LENGHT] = "";
#endif


