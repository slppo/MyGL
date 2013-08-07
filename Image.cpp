#include "Image.hpp"
#include <png.h>
#include <istream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <cstdint>
#include <cctype>

namespace toj
{
  namespace
  {
    const std::string BmpFiles[] = { "bmp" };
    const std::string PngFiles[] = { "png" };

    template <std::size_t Size>
    bool IsIn(const std::string (&haystack)[Size], const std::string &needle)
    {
      return std::find(std::begin(haystack), std::end(haystack), needle) !=
        std::end(haystack);
    }

    ImageType GuessImageType(const std::string &ext)
    {
      std::string lower;
      std::transform(ext.cbegin(), ext.cend(), std::back_inserter(lower),
        std::tolower);
      if(IsIn(BmpFiles, lower))
        return ImageType::BMP;
      else if(IsIn(PngFiles, lower))
        return ImageType::PNG;
      return static_cast<ImageType>(-1);
    }
  }

  Image::Image(const std::string &file)
  {
    std::ifstream ifs(file, std::ios::binary);
    if(!ifs)
      throw LoadError("Failed to open file " + file);    
    std::size_t extIdx = file.find_last_of('.');
    if(extIdx == std::string::npos || extIdx + 1 == file.size())
      throw LoadError("Cannot infer file type of " + file);
    std::string ext  = file.substr(extIdx + 1);
    ImageType imageType = GuessImageType(ext);
    if(static_cast<int>(imageType) == -1)
      throw LoadError("Cannot infer file type of " + file);
    _LoadFromStream(ifs, imageType);
  }

  Image::Image(std::istream &is, ImageType imageType)
  {
    _LoadFromStream(is, imageType);
  }

  void Image::_LoadFromStream(std::istream &is, ImageType imageType)
  {
    switch(imageType)
    {
    case ImageType::BMP: _LoadBMP(is); break;
    case ImageType::PNG: _LoadPNG(is); break;
    }
  }

  void Image::_LoadBMP(std::istream &is)
  {
#pragma pack(1)
    struct {
      std::uint8_t MagicId[2];
      std::uint32_t FileSize;
      std::uint16_t Reserved[2];
      std::uint32_t ImageOffset;
    } fileHeader;
    struct {
      std::uint32_t HeaderSize;
      std::int32_t ImageWidth;
      std::int32_t ImageHeight;
      std::uint16_t ColorPlanes;
      std::uint16_t BitsPerPixel;
      std::uint32_t CompressionMethod;
      std::uint32_t ImageSize;
      std::int32_t HorizontalResolution;
      std::int32_t VerticalResolution;
      std::uint32_t Colors;
      std::uint32_t ImportantColors;
    } bmpInfoHeader;
#pragma pack()
    static const std::uint8_t BmpMagicId[2] = { 'B', 'M' };

    is.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    if(!is)
      throw LoadError("Could not read BMP.");
    if(std::memcmp(fileHeader.MagicId, BmpMagicId, sizeof(BmpMagicId)))
      throw LoadError("Invalid BMP. Magic ID incorrect.");

    is.read(reinterpret_cast<char*>(&bmpInfoHeader), sizeof(bmpInfoHeader));
    if(!is)
      throw LoadError("Could not read BMP.");
    if(bmpInfoHeader.HeaderSize != sizeof(bmpInfoHeader))
      throw LoadError("Unrecognized BMP header.");

    switch(bmpInfoHeader.BitsPerPixel)
    {
    case 24: _pixelFormat = PixelFormat::BGR; break;
    }

    _width = std::abs(bmpInfoHeader.ImageWidth);
    _height = std::abs(bmpInfoHeader.ImageHeight);

    unsigned int bytesPerPixel = bmpInfoHeader.BitsPerPixel / CHAR_BIT;

    _pixelData.resize(_height * _width * bytesPerPixel);
    for(int i = 0; i < _height; ++i)
    {
      std::vector<byte_t> row(_width * bytesPerPixel);
      for(int j = 0; j < _width; ++j)
      {
        is.read(reinterpret_cast<char*>(&row[j * bytesPerPixel]), bytesPerPixel);
      }
      std::copy(row.cbegin(), row.cend(), 
        _pixelData.begin() + _width * bytesPerPixel * (_height - i - 1));
    }
  }

  void Image::_LoadPNG(std::istream &is)
  {
    png_image img = { 0 };
    img.version = PNG_IMAGE_VERSION;

    std::unique_ptr<void> inputBuffer;
    is.seekg(0, std::ios::end);
    std::size_t size = static_cast<std::size_t>(is.tellg());
    inputBuffer.reset(operator new(size));
    is.seekg(0, std::ios::beg);
    is.read(reinterpret_cast<char*>(inputBuffer.get()), size);

    if(!png_image_begin_read_from_memory(&img, inputBuffer.get(), size))
      throw LoadError("Could not read PNG."); 

    img.format = PNG_FORMAT_RGBA;
    _pixelData.resize(PNG_IMAGE_SIZE(img));
    if(!png_image_finish_read(&img, 0, &_pixelData[0], 0, 0))
      throw LoadError("Could not read PNG."); 

    _width = img.width;
    _height = img.height;
    _pixelFormat = PixelFormat::RGBA;
  }
}