
#########################################################################
# This program is free software: you can redistribute it and/or modify  #
# it under the terms of the version 3 of the GNU General Public License #
# as published by the Free Software Foundation.			        #
# 								        #
# This program is distributed in the hope that it will be useful, but   #
# WITHOUT ANY WARRANTY; without even the implied warranty of	        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      #
# General Public License for more details.			        #
# 								        #
# You should have received a copy of the GNU General Public License     #
# along with this program. If not, see <http://www.gnu.org/licenses/>.  #
# 								        #
# (C) Idiap Research Institute                                          #
# Written by Francois Fleuret                                           #
# Contact <francois.fleuret@idiap.ch> for comments & bug reports        #
#########################################################################

ifeq ($(DEBUG),yes)
  OPTIMIZE_FLAG = -ggdb3 -DDEBUG
else
  OPTIMIZE_FLAG = -ggdb3 -O3
endif

CXXFLAGS = -Wall $(OPTIMIZE_FLAG)

all: pw1

TAGS: *.cc *.h
	etags --members -l c++ *.cc *.h

pw1: \
	sq_matrix.o \
	functional_regressor.o \
	polynomial_regressor.o \
	hinge_regressor.o\
	pw1.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

Makefile.depend: *.h *.cc Makefile
	$(CC) -M *.cc > Makefile.depend

clean:
	\rm -f pw1 *.o Makefile.depend TAGS

-include Makefile.depend
