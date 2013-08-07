#ifndef MYGL_TEXTURE_HPP
#define MYGL_TEXTURE_HPP

#include "Object.hpp"
#include "Type.hpp"
#include "Color.hpp"
#include <GL/glew.h>
#include <type_traits>

// TODO: add 1D and 3D textures

namespace MyGL
{
  enum class TextureTarget { Tex2D = GL_TEXTURE_2D };
  enum class PixelFormat { RGB = GL_RGB, RGBA = GL_RGBA };
  enum class Wrapping { ClampToEdge = GL_CLAMP_TO_EDGE, 
    ClampToBorder = GL_CLAMP_TO_BORDER, Repeat = GL_REPEAT, 
    MirroredRepeat = GL_MIRRORED_REPEAT };
  enum class Filtering { Nearest = GL_NEAREST, Linear = GL_LINEAR,
    NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST, 
    LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
    NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR, 
    LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR };

  template <TextureTarget Target> struct TextureTraits { };
  template <> 
  struct TextureTraits<TextureTarget::Tex2D>
  {
    static void Upload(GLint level, PixelFormat internalFormat, GLsizei width,
      GLsizei height, PixelFormat format, Type type, const GLvoid *data);
  };

  template <TextureTarget Target, class Traits = TextureTraits<Target>>
  class Texture : public Object
  {
    typedef Texture<Target, Traits> this_t;
    const static GLenum GlTarget = static_cast<GLenum>(Target);
  public:
    Texture() : Object(glGenTextures) { }
    Texture(this_t &&rhs) : Object(std::move(static_cast<Object&&>(rhs))) { }
    ~Texture() { Destroy(glDeleteTextures); }
    this_t &operator=(this_t &&rhs)
    {
      if(this != &rhs)
      {
        Destroy(glDeleteTextures);
        static_cast<Object&>(*this) = std::move(static_cast<Object&&>(rhs));        
      }
      return *this;
    }
    template <typename T>
    void SetBorderColor(const Color<T> &color)
    {
      glTexParameterfv(GlTarget, GL_TEXTURE_BORDER_COLOR, 
        reinterpret_cast<const GLfloat*>(&color));
    }
    template <typename T>
    void Upload(PixelFormat format, GLsizei width, GLsizei height, 
      const T &data)
    {
      Traits::Upload(0, format, width, height, format, 
        GetType<std::remove_all_extents<std::remove_cv<
        std::remove_pointer<T>::type>::type>::type>(),
        reinterpret_cast<const GLvoid*>(data));
    }
    void Bind(int active = -1) 
    { 
      if(active >= 0)
        glActiveTexture(GL_TEXTURE0 + active);
      glBindTexture(GlTarget, Id()); 
    }
    void SetWrapping(Wrapping wrapping)
    {
      GLint glWrapping = static_cast<GLint>(wrapping);
      glTexParameteri(GlTarget, GL_TEXTURE_WRAP_S, glWrapping);
      glTexParameteri(GlTarget, GL_TEXTURE_WRAP_T, glWrapping);
      glTexParameteri(GlTarget, GL_TEXTURE_WRAP_R, glWrapping);
    }
    void SetFiltering(Filtering filtering)
    {
      GLint glFiltering = static_cast<GLint>(filtering);
      glTexParameteri(GlTarget, GL_TEXTURE_MIN_FILTER, glFiltering);
      glTexParameteri(GlTarget, GL_TEXTURE_MAG_FILTER, glFiltering);
    }
    static void Unbind() { glBindTexture(GlTarget, 0); }
  };

  typedef Texture<TextureTarget::Tex2D> Texture2D;
}

#endif