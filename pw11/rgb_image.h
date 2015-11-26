
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

/*

  A simple image class to either load color images, or produce
  materials.

 */

#ifndef RGB_IMAGE_H
#define RGB_IMAGE_H

#include "misc.h"

class RGBImage {
protected:
  int _width, _height;
  unsigned char ***_bit_plans, **_bit_lines, *_bit_map;

  void allocate();
  void deallocate();

public:

  static const int RED = 0;
  static const int GREEN = 1;
  static const int BLUE = 2;
  static const int RGB_DEPTH = 3;

  RGBImage();
  RGBImage(int width, int height);
  RGBImage(RGBImage *image);
  RGBImage(RGBImage *image, scalar_t scale);
  virtual ~RGBImage();

  void resize(int width, int height);

  inline int width() const { return _width; }
  inline int height() const { return _height; }

  inline void set_pixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    ASSERT(x >= 0 && x < _width && y >= 0 && y < _height);
    _bit_plans[RED][y][x] = r;
    _bit_plans[GREEN][y][x] = g;
    _bit_plans[BLUE][y][x] = b;
  }

  inline unsigned char pixel(int x, int y, int d) {
    ASSERT(x >= 0 && x < _width && y >= 0 && y < _height && d >= 0 && d < RGB_DEPTH);
    return _bit_plans[d][y][x];
  }

  virtual void read_jpg(const char *filename);
  virtual void write_jpg(const char *filename, int quality);

  virtual void fill(unsigned char r, unsigned char g, unsigned char b);

  virtual void draw_line(int thickness,
                         unsigned char r, unsigned char g, unsigned char b,
                         scalar_t x0, scalar_t y0, scalar_t x1, scalar_t y1);

  virtual void draw_ellipse(int thickness,
                            unsigned char r, unsigned char g, unsigned char b,
                            scalar_t xc, scalar_t yc,
                            scalar_t radius_1, scalar_t radius_2, scalar_t tilt);

  virtual void transform(RGBImage *src,
                         int over_sampling,
                         scalar_t x_src, scalar_t y_src,
                         scalar_t x_dst, scalar_t y_dst,
                         scalar_t scale,
                         scalar_t angle);
};

#endif
