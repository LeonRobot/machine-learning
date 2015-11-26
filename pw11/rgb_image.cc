
////////////////////////////////////////////////////////////////////
// START_IP_HEADER                                                //
//                                                                //
// Written by Francois Fleuret                                    //
// Contact <francois.fleuret@idiap.ch> for comments & bug reports //
//                                                                //
// END_IP_HEADER                                                  //
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdio.h>

#include "jpeg_misc.h"

#include "rgb_image.h"

void RGBImage::allocate() {
  _bit_plans = new unsigned char **[RGB_DEPTH];
  _bit_lines = new unsigned char *[RGB_DEPTH * _height];
  _bit_map = new unsigned char [_width * _height * RGB_DEPTH];
  for(int k = 0; k < RGB_DEPTH; k++) _bit_plans[k] = _bit_lines + k * _height;
  for(int k = 0; k < RGB_DEPTH * _height; k++) _bit_lines[k] = _bit_map + k * _width;
}

void RGBImage::deallocate() {
  delete[] _bit_plans;
  delete[] _bit_lines;
  delete[] _bit_map;
}

RGBImage::RGBImage() : _bit_plans(0), _bit_lines(0), _bit_map(0) { }

RGBImage::RGBImage(int width, int height) : _width(width), _height(height) {
  allocate();
  memset(_bit_map, 0, _width * _height * RGB_DEPTH * sizeof(unsigned char));
}

void RGBImage::resize(int width, int height) {
  deallocate();
  _width = width;
  _height = height;
  allocate();
  memset(_bit_map, 0, _width * _height * RGB_DEPTH * sizeof(unsigned char));
}

RGBImage::RGBImage(RGBImage *image) {
  _width = int(image->_width);
  _height = int(image->_height);

  allocate();

  for(int y = 0; y < _height; y++) {
    for(int x = 0; x < _width; x++) {
      _bit_plans[RED][y][x] = image->_bit_plans[RED][y][x];
      _bit_plans[GREEN][y][x] = image->_bit_plans[GREEN][y][x];
      _bit_plans[BLUE][y][x] = image->_bit_plans[BLUE][y][x];
    }
  }
}

RGBImage::RGBImage(RGBImage *image, scalar_t scale) {
  _width = int(scale * image->_width);
  _height = int(scale * image->_height);

  allocate();

  for(int y = 0; y < _height; y++) {
    for(int x = 0; x < _width; x++) {

      const int delta = 10;
      int sr = 0, sg = 0, sb = 0, t = 0;
      int xo, yo;

      for(int yy = y * delta; yy < (y + 1) * delta; yy++) {
        for(int xx = x * delta; xx < (x + 1) * delta; xx++) {
          xo = (image->_width * xx)/(_width * delta);
          yo = (image->_height * yy)/(_height * delta);
          if(xo >= 0 && xo < image->_width && yo >= 0 && yo < image->_height) {
            sr += image->_bit_plans[RED][yo][xo];
            sg += image->_bit_plans[GREEN][yo][xo];
            sb += image->_bit_plans[BLUE][yo][xo];
            t++;
          }
        }
      }

      if(t > 0) {
        _bit_plans[RED][y][x] = sr / t;
        _bit_plans[GREEN][y][x] = sg / t;
        _bit_plans[BLUE][y][x] = sb / t;
      } else {
        _bit_plans[RED][y][x] = 0;
        _bit_plans[GREEN][y][x] = 0;
        _bit_plans[BLUE][y][x] = 0;
      }

    }
  }
}

RGBImage::~RGBImage() {
  deallocate();
}

void RGBImage::write_jpg(const char *filename, int quality) {
  struct jpeg_compress_struct cinfo;
  struct my_error_mgr jerr;
  FILE *outfile;                /* target file */
  JSAMPARRAY buffer;            /* Output row buffer */

  jpeg_create_compress (&cinfo);

  if ((outfile = fopen (filename, "wb")) == 0) {
    fprintf (stderr, "Can't open %s\n", filename);
    exit(EXIT_FAILURE);
  }

  cinfo.err = jpeg_std_error (&jerr.pub);
  jerr.pub.error_exit = my_error_exit;

  if (setjmp (jerr.setjmp_buffer)) {
    jpeg_destroy_compress (&cinfo);
    fclose (outfile);
    exit(EXIT_FAILURE);
  }

  jpeg_stdio_dest (&cinfo, outfile);

  cinfo.image_width = _width;
  cinfo.image_height = _height;
  cinfo.input_components = RGB_DEPTH;

  cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults (&cinfo);
  jpeg_set_quality (&cinfo, quality, TRUE);
  jpeg_start_compress (&cinfo, TRUE);
  int y = 0;
  buffer =
    (*cinfo.mem->alloc_sarray) ((j_common_ptr) & cinfo, JPOOL_IMAGE,
                                _width * RGB_DEPTH, 1);
  while (int(cinfo.next_scanline) < _height) {
    for(int d = 0; d < RGB_DEPTH; d++)
      for(int x = 0; x < _width; x++)
        buffer[0][x * RGB_DEPTH + d] =
          (JSAMPLE) ((_bit_map[x + _width * (y + _height * d)] * (MAXJSAMPLE + 1)) / 255);
    jpeg_write_scanlines (&cinfo, buffer, 1);
    y++;
  }

  jpeg_finish_compress (&cinfo);
  fclose (outfile);

  jpeg_destroy_compress (&cinfo);
}

void RGBImage::read_jpg(const char *filename) {
  struct jpeg_decompress_struct cinfo;
  struct my_error_mgr jerr;
  FILE *infile;
  JSAMPARRAY buffer;

  deallocate();

  if ((infile = fopen (filename, "rb")) == 0) {
    fprintf (stderr, "can't open %s\n", filename);
    return;
  }

  cinfo.err = jpeg_std_error (&jerr.pub);
  jerr.pub.error_exit = my_error_exit;

  if (setjmp (jerr.setjmp_buffer)) {
    jpeg_destroy_decompress (&cinfo);
    fclose (infile);
    delete[] _bit_map;
    _width = 0;
    _height = 0;
    _bit_map = 0;
    return;
  }

  jpeg_create_decompress (&cinfo);
  jpeg_stdio_src (&cinfo, infile);
  jpeg_read_header (&cinfo, TRUE);
  jpeg_start_decompress (&cinfo);

  _width = cinfo.output_width;
  _height = cinfo.output_height;
  int depth = cinfo.output_components;

  allocate();

  buffer =
    (*cinfo.mem->alloc_sarray) ((j_common_ptr) & cinfo, JPOOL_IMAGE,
                                _width * depth, 1);

  int y = 0;
  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines (&cinfo, buffer, 1);
    if(depth == 1) {
      for(int d = 0; d < RGB_DEPTH; d++)
        for(int x = 0; x < _width; x++)
          _bit_plans[d][y][x] =
            (unsigned char) ((buffer[0][x * depth] * 255) / (MAXJSAMPLE + 1));
    } else {
      for(int d = 0; d < depth; d++)
        for(int x = 0; x < _width; x++)
          _bit_plans[d][y][x] =
            (unsigned char) ((buffer[0][x * depth + d] * 255) / (MAXJSAMPLE + 1));
    }
    y++;
  }

  jpeg_finish_decompress (&cinfo);
  jpeg_destroy_decompress (&cinfo);

  fclose (infile);
}

void RGBImage::fill(unsigned char r, unsigned char g, unsigned char b) {
  for(int y = 0; y < _height; y++) {
    for(int x = 0; x < _width; x++) {
      _bit_plans[RED][y][x] = r;
      _bit_plans[GREEN][y][x] = g;
      _bit_plans[BLUE][y][x] = b;
    }
  }
}

void RGBImage::draw_line(int thickness,
                         unsigned char r, unsigned char g, unsigned char b,
                         scalar_t x0, scalar_t y0, scalar_t x1, scalar_t y1) {
  int l = 0;
  int dx, dy, h, v;
  int ix0 = int(x0 + 0.5), iy0 = int(y0 + 0.5), ix1 = int(x1 + 0.5), iy1 = int(y1 + 0.5);

  if(ix0 < ix1) { dx = 1; h = ix1 - ix0; } else { dx = -1; h = ix0 - ix1; }
  if(iy0 < iy1) { dy = 1; v = iy1 - iy0; } else { dy = -1; v = iy0 - iy1; }

  int x = ix0, y = iy0;

  if(h > v) {
    for(int i = 0; i < h + 1; i++) {
      for(int ex = - thickness / 2 - 1; ex < (thickness + 1) / 2 + 1; ex++) {
        for(int ey = - thickness / 2 - 1; ey < (thickness + 1) / 2 + 1; ey++) {
          if(ex * ex + ey * ey <= thickness * thickness / 4) {
            int xx = x + ex, yy = y + ey;
            if(xx >= 0 && xx < _width && yy >= 0 && yy < _height)
              set_pixel(xx, yy, r, g, b);
          }
        }
      }

      x += dx; l += v;
      if(l > 0) { y += dy; l -= h; }
    }

  } else {

    for(int i = 0; i < v + 1; i++) {
      for(int ex = - thickness / 2 - 1; ex < (thickness + 1) / 2 + 1; ex++) {
        for(int ey = - thickness / 2 - 1; ey < (thickness + 1) / 2 + 1; ey++) {
          if(ex * ex + ey * ey <= thickness * thickness / 4) {
            int xx = x + ex, yy = y + ey;
            if(xx >= 0 && xx < _width && yy >= 0 && yy < _height)
              set_pixel(xx, yy, r, g, b);
          }
        }
      }

      y += dy; l -= h;
      if(l < 0) { x += dx; l += v; }
    }

  }

}

void RGBImage::draw_ellipse(int thickness,
                            unsigned char r, unsigned char g, unsigned char b,
                            scalar_t xc, scalar_t yc, scalar_t radius_1, scalar_t radius_2, scalar_t tilt) {
  scalar_t ux1 =   cos(tilt) * radius_1, uy1 =   sin(tilt) * radius_1;
  scalar_t ux2 = - sin(tilt) * radius_2, uy2 =   cos(tilt) * radius_2;

  const int nb_points_to_draw = 80;
  scalar_t x, y, px = 0, py = 0;

  for(int i = 0; i <= nb_points_to_draw; i++) {
    scalar_t alpha = (M_PI * 2 * scalar_t(i)) / scalar_t(nb_points_to_draw);

    x = xc + cos(alpha) * ux1 + sin(alpha) * ux2;
    y = yc + cos(alpha) * uy1 + sin(alpha) * uy2;

    if(i > 0) {
      draw_line(thickness, r, g, b, px, py, x, y);
    }

    px = x; py = y;
  }
}

void RGBImage::transform(RGBImage *src,
                         int over_sampling,
                         scalar_t x_src, scalar_t y_src,
                         scalar_t x_dst, scalar_t y_dst,
                         scalar_t scale,
                         scalar_t angle) {
  scalar_t ux = cos(angle) / scale;
  scalar_t uy = sin(angle) / scale;

  for(int y = 0; y < _height; y++) {
    for(int x = 0; x < _width; x++) {

      int sr = 0, sg = 0, sb = 0, t = 0;
      int xo, yo;

      for(int yy = y * over_sampling; yy < (y + 1) * over_sampling; yy++) {
        for(int xx = x * over_sampling; xx < (x + 1) * over_sampling; xx++) {

          scalar_t a = scalar_t(xx)/scalar_t(over_sampling) - x_dst;
          scalar_t b = scalar_t(yy)/scalar_t(over_sampling) - y_dst;

          xo = int(x_src + a * ux + b * uy);
          yo = int(y_src - b * ux + a * uy);

          if(xo >= 0 && xo < src->_width && yo >= 0 && yo < src->_height) {
            t++;
            sr += src->_bit_plans[RED][yo][xo];
            sg += src->_bit_plans[GREEN][yo][xo];
            sb += src->_bit_plans[BLUE][yo][xo];
          }
        }

        if(t > 0) {
          _bit_plans[RED][y][x] = sr / t;
          _bit_plans[GREEN][y][x] = sg / t;
          _bit_plans[BLUE][y][x] = sb / t;
        }
        // else {
          // _bit_plans[RED][y][x] = 0;
          // _bit_plans[GREEN][y][x] = 0;
          // _bit_plans[BLUE][y][x] = 0;
        // }
      }
    }
  }
}
