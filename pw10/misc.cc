
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
          exit(1);
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

scalar_t discrete_entropy(int *n, int nb) {
  scalar_t s = 0, t = 0;
  for(int k = 0; k < nb; k++) {
    if(n[k] > 0) {
      s += n[k] * log(scalar_t(n[k]));
      t += n[k];
    }
  }
  return (log(t) - s/scalar_t(t))/log(2.0);
}

void random_permutation(int *val, int nb) {
  for(int k = 0; k < nb; k++) val[k] = k;
  int i, t;
  for(int k = 0; k < nb - 1; k++) {
    i = int(drand48() * (nb - k)) + k;
    t = val[i];
    val[i] = val[k];
    val[k] = t;
  }
}

void tag_subset(bool *val, int nb_total, int nb_to_tag) {
  ASSERT(nb_to_tag <= nb_total);
  int index[nb_total];
  random_permutation(index, nb_total);
  for(int n = 0; n < nb_total; n++) val[n] = false;
  for(int n = 0; n < nb_to_tag; n++) val[index[n]] = true;
}

int compare_couple(const void *a, const void *b) {
  if(((Couple *) a)->value < ((Couple *) b)->value) return -1;
  else if(((Couple *) a)->value > ((Couple *) b)->value) return 1;
  else return 0;
}
