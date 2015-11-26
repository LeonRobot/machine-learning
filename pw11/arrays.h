
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#ifndef ARRAYS_H
#define ARRAYS_H

template<class T>
T inline sqdist(int dim, T *x, T *y) {
  T result = 0;
  for(int k = 0; k < dim; k++) result += sq(x[k] - y[k]);
  return result;
}

template<class T>
T inline sum(int dim, T *x) {
  T result = 0;
  for(int k = 0; k < dim; k++) result += x[k];
  return result;
}

template<class T>
T inline dotprod(int dim, T *x, T *y) {
  T result = 0;
  for(int k = 0; k < dim; k++) result += x[k] * y[k];
  return result;
}

template<class T>
void fill_vector(int dim, T *dst, T v) {
  for(int k = 0; k < dim; k++) {
    *(dst++) = v;
  }
}

template<class T>
void copy_vector(int dim, T *dst, T *src) {
  for(int k = 0; k < dim; k++) {
    dst[k] = src[k];
  }
}

template<class T>
T *flat_array(T **array) {
  return array[0];
}

template<class T>
T **allocate_array(int a, int b) {
  if(a > 0 && b > 0) {
    T *whole = new T[a * b];
    T **array = new T *[a];
    for(int k = 0; k < a; k++) {
      array[k] = whole;
      whole += b;
    }
    return array;
  } else {
    return 0;
  }
}

template<class T>
void deallocate_array(T **array) {
  if(array) {
    delete[] array[0];
    delete[] array;
  }
}

template<class T>
void fill_array(int a, int b, T **dst, T v) {
  T *p_dst = dst[0];
  for(int k = 0; k < a * b; k++) {
    *(p_dst++) = v;
  }
}

template<class T>
void copy_array(int a, int b, T **dst, T **src) {
  T *p_dst = dst[0], *p_src = src[0];
  for(int k = 0; k < a * b; k++) {
    *(p_dst++) = *(p_src++);
  }
}

//////////////////////////////////////////////////////////////////////

template<class T>
T *flat_volume(T ***volume) {
  return volume[0][0];
}

template<class T>
T ***allocate_volume(int a, int b, int c) {
  T *whole = new T[a * b * c];
  T **column = new T *[a * b];
  T ***volume = new T **[a];

  for(int k = 0; k < a; k++) {
    volume[k] = column;
    for(int l = 0; l < b; l++) {
      column[l] = whole;
      whole += c;
    }
    column += b;
  }

  return volume;
}

template<class T>
void deallocate_volume(T ***volume) {
  if(volume) {
    delete[] volume[0][0];
    delete[] volume[0];
    delete[] volume;
  }
}

template<class T>
void fill_volume(int a, int b, int c, T ***dst, T v) {
  T *p_dst = dst[0][0];
  for(int k = 0; k < a * b * c; k++) {
    *(p_dst++) = v;
  }
}

template<class T>
void copy_volume(int a, int b, int c, T ***dst, T ***src) {
  T *p_dst = dst[0][0], *p_src = src[0][0];
  for(int k = 0; k < a * b * c; k++) {
    *(p_dst++) = *(p_src++);
  }
}

#endif
