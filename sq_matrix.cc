
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

#include "sq_matrix.h"

SqMatrix::SqMatrix(int s) {
  size = s;
  coeff = new scalar_t *[size];
  coeff[0] = new scalar_t[size * size];
  for(int t = 1; t < size; t++) {
    coeff[t] = coeff[t-1] + s;
  }
}

void SqMatrix::free() {
  if(coeff) {
    delete[] coeff[0];
    delete[] coeff;
  }
}

SqMatrix::~SqMatrix() {
  free();
}

void SqMatrix::clear() {
  for(int i = 0; i < size; i++) {
    for(int j = 0; j < size; j++) {
      coeff[i][j] = 0.0;
    }
  }
}

int SqMatrix::invert() {
  SqMatrix result(size);
  result.clear();
  for(int i = 0 ; i < size; i++) {
    result.coeff[i][i] = 1.0;
  }

  for(int j = 0; j < size; j++) {
    scalar_t max = abs(coeff[j][j]);
    int jmax = j;
    for(int jj = j+1; jj < size; jj++) {
      if(abs(coeff[j][jj]) > max) {
        max = abs(coeff[j][jj]);
        jmax = jj;
      }
    }

    max = coeff[j][jmax];

    if(max == 0.0) {
      return 0;
    }

    switch_lines(j, jmax);
    result.switch_lines(j, jmax);

    multiply_line(j, 1 / max);
    result.multiply_line(j, 1 / max);

    for(int jj = j+1; jj < size; jj++) {
      scalar_t c = -coeff[j][jj];
      add_k_time_line(jj, j, c);
      result.add_k_time_line(jj, j, c);
    }
  }

  for(int j = size-1; j > 0; j--) {
    for(int jj = 0; jj < j; jj++) {
      scalar_t c = -coeff[j][jj];
      add_k_time_line(jj, j, c);
      result.add_k_time_line(jj, j, c);
    }
  }

  free();
  coeff = result.coeff;
  result.coeff = 0;

  return 1;
}
