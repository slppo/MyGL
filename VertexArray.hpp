#ifndef MYGL_VERTEX_ARRAY_HPP
#define MYGL_VERTEX_ARRAY_HPP

#include "Object.hpp"
#include <GL/glew.h>

namespace MyGL
{
  class VertexArray : public Object
  {
  public:
    VertexArray() : Object(glGenVertexArrays) { }
    ~VertexArray() { Destroy(glDeleteVertexArrays); }
    void Bind() { glBindVertexArray(Id()); }
    static void Unbind() { glBindVertexArray(0); }
  };
}

#endif