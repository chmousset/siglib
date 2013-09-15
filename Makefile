# sigLib, simple Signals Library
# 
# Copyright (C) 2013 Charles-Henri Mousset
# 
# This file is part of sigLib.
# 
# sigLib is free software: you can redistribute it and/or modify it under the terms of the
# GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# 
# sigLib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
# See the GNU Lesser General Public License for more details. You should have received a copy of the GNU
# General Public License along with sigLib. If not, see <http://www.gnu.org/licenses/>.
# 
# Authors: Charles-Henri Mousset


INCDIR = -I ./
INCDIR += -I ./test
CC=gcc
COPT=-Wall

test_sigf:
	$(CC) sigf.c sig.c test/testf.c test/csv.c test/test_pidf.c -o test/testf.out $(INCDIR) -lm $(COPT)

test:	test_sigf

clean:
	rm test/*.out