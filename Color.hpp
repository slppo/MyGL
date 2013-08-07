#ifndef MYGL_COLOR_HPP
#define MYGL_COLOR_HPP

#include <GL/glew.h>

namespace MyGL
{
  template <typename T>
  struct Color
  {
    Color() { }
    Color(T r, T g, T b, T a = 1) : R(r), G(g), B(b), A(a) { }

    T R;
    T G;
    T B;
    T A;
  };

  typedef Color<GLfloat> ColorF;
}

#endif