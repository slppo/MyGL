#ifndef MYGL_BUFFER_HPP
#define MYGL_BUFFER_HPP

#include "Object.hpp"
#include <GL/glew.h>
#include <vector>

namespace MyGL
{
  enum class UsageHint { StaticDraw = GL_STATIC_DRAW, 
    StreamDraw = GL_STREAM_DRAW, DynamicDraw = GL_DYNAMIC_DRAW };

  template <GLenum Target>
  class Buffer : public Object
  {
  public:
    Buffer() : Object(glGenBuffers) { }
    Buffer(Buffer &&rhs) : Object(std::move(static_cast<Object&&>(rhs))) { }
    ~Buffer() { Destroy(glDeleteBuffers); }
    Buffer &operator=(Buffer &&rhs)
    {
      if(this != &rhs)
      {
        Destroy(glDeleteBuffers);
        _id = rhs._id;
        rhs._id = 0;
      }
      return *this;
    }
    template <typename T>
    void Upload(const T &data, UsageHint usageHint)
    {
      glBufferData(Target, sizeof(data), data, static_cast<GLenum>(usageHint));
    }
    template <typename T>
    void Upload(const std::vector<T> &data, UsageHint usageHint)
    {
      glBufferData(Target, data.size() * sizeof(T), &data[0], 
        static_cast<GLenum>(usageHint));
    }
    void Bind() const { glBindBuffer(Target, Id()); }
    static void Unbind() { glBindBuffer(Target, 0); }
  private:
    Buffer(const Buffer &);
    Buffer &operator=(const Buffer &);
  };

  typedef Buffer<GL_ARRAY_BUFFER> VBO;
  typedef Buffer<GL_ELEMENT_ARRAY_BUFFER> EBO;
}

#endif