
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#include <fstream>

using namespace std;

#include "misc.h"

char *next_word(char *buffer, char *r, int buffer_size) {
  char *s;
  s = buffer;

  if(r != 0) {
    while((*r == ' ') || (*r == '\t') || (*r == ',')) r++;
    if(*r == '"') {
      r++;
      while((*r != '"') && (*r != '\0') &&
            (s<buffer+buffer_size-1))
        *s++ = *r++;
      if(*r == '"') r++;
    } else {
      while((*r != '\r') && (*r != '\n') && (*r != '\0') &&
            (*r != '\t') && (*r != ' ') && (*r != ',')) {
        if(s == buffer + buffer_size) {
          cerr << "Buffer overflow in next_word." << endl;
          abort();
        }
        *s++ = *r++;
      }
    }

    while((*r == ' ') || (*r == '\t') || (*r == ',')) r++;
    if((*r == '\0') || (*r=='\r') || (*r=='\n')) r = 0;
  }
  *s = '\0';

  return r;
}

scalar_t compute_correlation(int n1, int n2, int n12, int n) {
  // rho(u,v) = (p(u, v) - p(u)p(v))/sqrt(p(u)p(not(u))p(v)p(not(v)))

  scalar_t p1 = scalar_t(n1) / scalar_t(n);
  scalar_t p2 = scalar_t(n2) / scalar_t(n);
  scalar_t p12 = scalar_t(n12) / scalar_t(n);

  if(p1 == 0.0 || p1 == 1.0 || p2 == 0.0 || p2 == 1.0) {
    return 0.0;
  } else {
    return (p12 - p1 * p2) / sqrt(p1 * (1 - p1) * p2 * (1 - p2));
  }
}

void check(bool condition, const char *message) {
  if(!condition) {
    cerr << message << endl;
    exit(EXIT_FAILURE);
  }
}

void random_permutation(int nb, int *perm) {
  for(int k = 0; k < nb; k++) perm[k] = k;
  int i, t;
  for(int k = 0; k < nb - 1; k++) {
    i = int(drand48() * (nb - k)) + k;
    t = perm[i];
    perm[i] = perm[k];
    perm[k] = t;
  }
}
