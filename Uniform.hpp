#ifndef MYGL_UNIFORM_HPP
#define MYGL_UNIFORM_HPP

#include <GL/glew.h>

namespace MyGL
{
  class Program;

  // TODO add all the glUniform* functions
  class Uniform
  {
    friend class Program;
  public:
    void Set(GLfloat value) { glUniform1f(_idx, value); }
    void Set(GLfloat x, GLfloat y, GLfloat z) { glUniform3f(_idx, x, y, z); }
    void Set(GLint value) { glUniform1i(_idx, value); }
    void Set(GLuint value) { glUniform1ui(_idx, value); }
  private:
    explicit Uniform(GLint idx) : _idx(idx) { }
    Uniform(const Uniform &);
    Uniform &operator=(const Uniform &);

    GLint _idx;
  };
}

#endif