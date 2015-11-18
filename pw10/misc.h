
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
// Written by and Copyright (C) Francois Fleuret                         //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports        //
///////////////////////////////////////////////////////////////////////////

#ifndef MISC_H
#define MISC_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <cfloat>
#include <stdlib.h>
#include <string.h>

using namespace std;

//typedef double scalar_t;
typedef float scalar_t;
const scalar_t SCALAR_MAX = FLT_MAX;
const scalar_t SCALAR_MIN = FLT_MIN;

const int buffer_size = 1024;
const int large_buffer_size = 65536;

using namespace std;

#ifdef DEBUG
#define ASSERT(x) if(!(x)) { \
  std::cerr << "ASSERT FAILED IN " << __FILE__ << ":" << __LINE__ << endl; \
  abort(); \
}
#else
#define ASSERT(x)
#endif

template<class T>
T smooth_min(T x, T y) {
  T z = exp(x - y);
  return 0.5 * (x + y - (x - y)/(1 + 1/z) - (y - x)/(1 + z));
}

template <class T>
void write_var(ostream *os, const T *x) { os->write((char *) x, sizeof(T)); }

template <class T>
void read_var(istream *is, T *x) { is->read((char *) x, sizeof(T)); }

template <class T>
void grow(int *nb_max, int nb, T** current, int factor) {
  ASSERT(*nb_max > 0);
  if(nb == *nb_max) {
    T *tmp = new T[*nb_max * factor];
    memcpy(tmp, *current, *nb_max * sizeof(T));
    delete[] *current;
    *current = tmp;
    *nb_max *= factor;
  }
}

template <class T>
inline T sq(T x) {
  return x * x;
}

inline scalar_t log2(scalar_t x) {
  return log(x)/log(2.0);
}

inline scalar_t xi(scalar_t x) {
  if(x <= 0.0) return 0.0;
  else         return - x * log(x)/log(2.0);
}

scalar_t discrete_entropy(int *n, int nb);

char *next_word(char *buffer, char *r, int buffer_size);

void random_permutation(int *val, int nb);
void tag_subset(bool *val, int nb_total, int nb_to_tag);

struct Couple {
  int index;
  scalar_t value;
};

int compare_couple(const void *a, const void *b);

#endif
