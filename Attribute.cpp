#include "Attribute.hpp"
#include <cassert>

namespace MyGL
{
  namespace
  {
    GLsizei SizeOf(Type type)
    {
      switch(type)
      {
      case Type::Invalid: return 1;
      case Type::Float: return sizeof(GLfloat);
      default: assert(0); return 0;
      }
    }
  }

  Attribute &Attribute::Size(GLint size)
  {
    _size = size;
    return *this;
  }

  Attribute &Attribute::Type(MyGL::Type type)
  {
    _type = type;
    return *this;
  }

  Attribute &Attribute::Normalized(GLboolean normalized)
  {
    _normalized = normalized;
    return *this;
  }

  Attribute &Attribute::Stride(GLsizei stride)
  {
    _stride = stride * SizeOf(_type);
    return *this;
  }

  Attribute &Attribute::Offset(GLsizei offset)
  {
    _offset = offset * SizeOf(_type);
    return *this;
  }

  void Attribute::Enable()
  {
    glVertexAttribPointer(_idx, _size, static_cast<GLenum>(_type), _normalized,
      _stride, reinterpret_cast<const GLvoid*>(_offset));  
    glEnableVertexAttribArray(_idx);
  }

  Attribute::Attribute(GLint idx) : _idx(idx), _size(0), _type(Type::Invalid),
    _normalized(GL_FALSE), _stride(0), _offset(0)
  {

  }
}