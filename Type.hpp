#ifndef MYGL_TYPE_HPP
#define MYGL_TYPE_HPP

#include <GL/glew.h>
#include <cstddef>

namespace MyGL
{
  // TODO: add all the types
  enum class Type { Invalid, Float = GL_FLOAT, Byte = GL_UNSIGNED_BYTE };
  
  template <typename T>
  Type GetType() { static_assert(false, "Unrecognized type"); }
  template <>
  inline Type GetType<GLfloat>() { return Type::Float; }
  template <>
  inline Type GetType<GLubyte>() { return Type::Byte; }
}

#endif