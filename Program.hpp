#ifndef MYGL_PROGRAM_HPP
#define MYGL_PROGRAM_HPP

#include "Object.hpp"
#include "Shader.hpp"
#include <GL/glew.h>

namespace MyGL
{
  class Attribute;
  class Uniform;

  class Program : public Object
  {
  public:
    Program() : Object(glCreateProgram()) { }
    Program(Program &&rhs) : Object(std::move(static_cast<Object&&>(rhs))) { }
    ~Program() { glDeleteProgram(Id()); }
    template <typename ShaderT>
    friend Program &operator<<(Program &program, 
      const ShaderT &shader);
    Program &operator=(Program &&rhs);
    void BindOutput(const GLchar *name, GLuint idx);
    void Link();
    Attribute Attribute(const GLchar *name);
    Uniform Uniform(const GLchar *name);
    void Use() { glUseProgram(Id()); }
    static void Unuse() { glUseProgram(0); }
  };

  template <typename ShaderT>
  Program &operator<<(Program &program, 
    const ShaderT &shader)
  {
    glAttachShader(program, shader);
    return program;
  }
}

#endif