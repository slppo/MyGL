#include "Texture.hpp"

namespace MyGL
{
  void TextureTraits<TextureTarget::Tex2D>::Upload(GLint level, 
    PixelFormat internalFormat, GLsizei width, GLsizei height, 
    PixelFormat format, Type type, const GLvoid *data)
  {
    glTexImage2D(GL_TEXTURE_2D, level, static_cast<GLint>(internalFormat), 
      width, height, 0, static_cast<GLenum>(format), static_cast<GLenum>(type), 
      data);
  }
}