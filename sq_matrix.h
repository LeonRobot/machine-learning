
///////////////////////////////////////////////////////////////////////////
// This program is free software: you can redistribute it and/or modify  //
// it under the terms of the version 3 of the GNU General Public License //
// as published by the Free Software Foundation.                         //
//                                                                       //
// This program is distributed in the hope that it will be useful, but   //
// WITHOUT ANY WARRANTY; without even the implied warranty of            //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      //
// General Public License for more details.                              //
//                                                                       //
// You should have received a copy of the GNU General Public License     //
// along with this program. If not, see <http://www.gnu.org/licenses/>.  //
//                                                                       //
// (C) Idiap Research Institute                                          //
// Written by Francois Fleuret                                           //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports        //
///////////////////////////////////////////////////////////////////////////

#ifndef SQ_MATRIX_H
#define SQ_MATRIX_H

#include "misc.h"

class SqMatrix {

  void free();

  inline void switch_lines(int a, int b) {
    if(a != b) {
      scalar_t t;
      for(int i = 0; i < size; i++) {
	t = coeff[i][a];
	coeff[i][a] = coeff[i][b];
	coeff[i][b] = t;
      }
    }
  }

  inline void multiply_line(int a, scalar_t k) {
    for(int i = 0; i < size; i++) {
      coeff[i][a] *= k;
    }
  }

  inline void add_k_time_line(int a, int b, scalar_t k) {
    for(int i = 0; i < size; i++) {
      coeff[i][a] += k * coeff[i][b];
    }
  }

public:
  int size;
  scalar_t **coeff;

  SqMatrix(int s);
  ~SqMatrix();

  void clear();
  // Returns if the inversion succeeded
  int invert();
};

#endif
