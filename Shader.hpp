#ifndef MYGL_SHADER_HPP
#define MYGL_SHADER_HPP

#include "Object.hpp"
#include <GL/glew.h>
#include <string>
#include <stdexcept>

namespace MyGL
{
  enum class ShaderType { Vertex = GL_VERTEX_SHADER, 
    Fragment = GL_FRAGMENT_SHADER };

  template <ShaderType Type>
  class Shader : public Object
  {
  public:
    Shader() : Object(glCreateShader(static_cast<GLenum>(Type)))
    { }
    ~Shader() { glDeleteShader(Id()); }
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
}

#endif