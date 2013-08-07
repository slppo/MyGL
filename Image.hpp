#ifndef TOJ_IMAGE_HPP
#define TOJ_IMAGE_HPP

#include <string>
#include <iosfwd>
#include <vector>

namespace toj
{
  enum class ImageType { BMP, PNG };
  enum class PixelFormat { RGBA, BGR };
  typedef unsigned char byte_t;

  class Image
  {
  public:
    class LoadError : public std::runtime_error
    {
    public:
      explicit LoadError(const std::string &msg) : 
        std::runtime_error(msg) { } 
    };

    explicit Image(const std::string &file);
    Image(std::istream &is, ImageType imageType);
    const std::vector<byte_t> &PixelData() const { return _pixelData; }
    int Width() const { return _width; }
    int Height() const { return _height; }
    PixelFormat PixelFormat() const { return _pixelFormat; }
  private:
    void _LoadFromStream(std::istream &is, ImageType imageType);
    void _LoadBMP(std::istream &is);
    void _LoadPNG(std::istream &is);

    std::vector<byte_t> _pixelData;
    int _width, _height;
    toj::PixelFormat _pixelFormat;
  };
}

#endif