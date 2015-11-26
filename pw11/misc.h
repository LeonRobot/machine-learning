
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#ifndef MISC_H
#define MISC_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <cfloat>
#include <stdlib.h>
#include <string.h>

using namespace std;

typedef double scalar_t;

const int large_buffer_size = 65536;
const int buffer_size = 1024;

using namespace std;

#ifdef DEBUG
#define ASSERT(x) if(!(x)) { \
  std::cerr << "ASSERT FAILED IN " << __FILE__ << ":" << __LINE__ << endl; \
  abort(); \
}
#else
#define ASSERT(x)
#endif

inline scalar_t random_0_1() { return drand48(); }

template <class T>
void write_var(ostream *os, const T *x) { os->write((char *) x, sizeof(T)); }

template <class T>
void read_var(istream *is, T *x) { is->read((char *) x, sizeof(T)); }

char *next_word(char *buffer, char *r, int buffer_size);

scalar_t compute_correlation(int n1, int n2, int n12, int n);

template <class T>
inline T sq(T x) {
  return x * x;
}

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

void check(bool condition, const char *message);

inline scalar_t normal_sample() {
  scalar_t a = drand48();
  scalar_t b = drand48();
  return cos(2 * M_PI * a) * sqrt(-2 * log(b));
}

void random_permutation(int nb, int *perm);

#endif
