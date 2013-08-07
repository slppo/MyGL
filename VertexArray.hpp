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
    VertexArray(VertexArray &&rhs) : 
      Object(std::move(static_cast<Object&&>(rhs))) { }
    ~VertexArray() { Destroy(glDeleteVertexArrays); }
    VertexArray &operator=(VertexArray &&rhs)
    {
      if(this != &rhs)
      {
        Destroy(glDeleteVertexArrays);
        static_cast<Object&>(*this) = std::move(static_cast<Object&&>(rhs));
      }
      return *this;
    }
    void Bind() { glBindVertexArray(Id()); }
    static void Unbind() { glBindVertexArray(0); }
  };
}

#endif