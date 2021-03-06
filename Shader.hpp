#ifndef MYGL_SHADER_HPP
#define MYGL_SHADER_HPP

#include "Object.hpp"
#include <GL/glew.h>
#include <string>
#include <stdexcept>

namespace MyGL
{
  enum class ShaderType { Vertex = GL_VERTEX_SHADER, 
    Fragment = GL_FRAGMENT_SHADER, Geometry = GL_GEOMETRY_SHADER };

  template <ShaderType Type>
  class Shader : public Object
  {
    typedef Shader<Type> this_t;
  public:
    Shader() : Object(glCreateShader(static_cast<GLenum>(Type))) { }
    Shader(this_t &&rhs) : Object(std::move(static_cast<Object&&>(rhs))) { }
    ~Shader() { glDeleteShader(Id()); }
    Shader &operator=(this_t &rhs)
    {
      if(this != &rhs)
      {
        glDeleteShader(Id());
        static_cast<Object&>(*this) = std::move(static_cast<Object&&>(rhs));
      }
      return *this;
    }
    void Compile(const GLchar *src)
    {
      glShaderSource(Id(), 1, &src, 0);
      glCompileShader(Id());
#ifndef NDEBUG
      GLint status;
      glGetShaderiv(Id(), GL_COMPILE_STATUS, &status);
      if(status != GL_TRUE)
      {
        GLint length;
        glGetShaderiv(Id(), GL_INFO_LOG_LENGTH, &length);
        std::string buffer(length, '\0');
        glGetShaderInfoLog(Id(), length, 0, &buffer[0]);
        throw std::logic_error(buffer);
      }      
#endif      
    }
  };

  typedef Shader<ShaderType::Vertex> VertexShader;
  typedef Shader<ShaderType::Fragment> FragmentShader;
  typedef Shader<ShaderType::Geometry> GeometryShader;
}

#endif