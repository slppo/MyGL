#ifndef MYGL_ATTRIBUTE_HPP
#define MYGL_ATTRIBUTE_HPP

#include "Type.hpp"
#include <GL/glew.h>

namespace MyGL
{
  class Program;

  class Attribute
  {
    friend class Program;
  public:
    Attribute &Size(GLint size);
    Attribute &Type(Type type);
    Attribute &Normalized(GLboolean normalized);
    Attribute &Stride(GLsizei stride);
    Attribute &Offset(GLsizei offset);
    void Enable();
  private:
    explicit Attribute(GLint idx);
    Attribute(const Attribute &);
    Attribute &operator=(const Attribute &);
  
    GLint _idx;
    GLint _size;
    MyGL::Type _type;
    GLboolean _normalized;
    GLsizei _stride;
    GLsizei _offset;
  };
}

#endif