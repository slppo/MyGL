#ifndef MYGL_OBJECT_HPP
#define MYGL_OBJECT_HPP

#include <GL/glew.h>

namespace MyGL
{
  class Object
  {
  public:
    operator const GLuint() const { return _id; }    
    const GLuint Id() const { return _id; }
    operator GLuint() { return _id; }    
    GLuint Id() { return _id; }
  protected:
    explicit Object(GLuint id) : _id(id) { }
    Object(Object &&rhs) : _id(rhs._id) { rhs._id = 0; }
    template <typename PtrGeneratorFn>
    explicit Object(PtrGeneratorFn generator) { generator(1, &_id); }
    virtual ~Object() { }
    template <typename PtrDestroyerFn>
    void Destroy(PtrDestroyerFn destroyer) { destroyer(1, &_id); }
  private:
    GLuint _id;
  };
}

#endif