#include "Program.hpp"
#include "Attribute.hpp"
#include "Uniform.hpp"
#include <string>
#include <stdexcept>

namespace MyGL
{
  void Program::BindOutput(const GLchar *name, GLuint idx)
  {
    glBindFragDataLocation(Id(), idx, name); 
  }

  void Program::Link()
  {
    glLinkProgram(Id());
#ifndef NDEBUG
    GLint status;
    glGetProgramiv(Id(), GL_LINK_STATUS, &status);
    if(status != GL_TRUE)
    {
      GLint length;
      glGetProgramiv(Id(), GL_INFO_LOG_LENGTH, &length);
      std::string buffer(length, '\0');
      glGetProgramInfoLog(Id(), length, 0, &buffer[0]);
      throw std::logic_error(buffer);
    }
#endif
  }

  Attribute Program::Attribute(const GLchar *name)
  {
    GLint idx = glGetAttribLocation(Id(), name);
#ifndef NDEBUG
    if(idx == -1)
      throw std::logic_error(std::string("Invalid attribute \"") + name + 
      "\"");  
#endif
    return MyGL::Attribute(idx);
  }

  Uniform Program::Uniform(const GLchar *name)
  {
    GLint idx = glGetUniformLocation(Id(), name);
#ifndef NDEBUG
    if(idx == -1)
      throw std::logic_error(std::string("Invalid uniform \"") + name + "\"");
#endif
    return MyGL::Uniform(idx);
  }
}