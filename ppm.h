#ifndef __PPM_H__
#define __PPM_H__

#include <cstdint>
#include <sstream>
#include <fstream>
#include <vector>

struct RGBColor
{
  uint64_t red, green, blue;
  
  RGBColor(uint64_t red = 0, uint64_t green = 0, uint64_t blue = 0)
    : red(red), green(green), blue(blue) {}
  
  RGBColor operator+(const RGBColor &rhs) const
  {
    return RGBColor(red + rhs.red, green + rhs.green, blue + rhs.blue);
  }
  
  RGBColor& operator+=(const RGBColor &rhs)
  {
    red   += rhs.red; 
    green += rhs.green; 
    blue  += rhs.blue;
    return *this;
  }
  
  RGBColor operator/(uint64_t rhs) const
  {
    return RGBColor(red / rhs, green / rhs, blue / rhs);
  }
  
  RGBColor& operator/=(uint64_t rhs)
  {
    red   /= rhs; 
    green /= rhs; 
    blue  /= rhs;
    return *this;
  }
  
  uint64_t dist(const RGBColor &rhs) const
  {
    uint64_t r = red   > rhs.red   ? red   - rhs.red   : rhs.red   - red;
    uint64_t g = green > rhs.green ? green - rhs.green : rhs.green - green;
    uint64_t b = blue  > rhs.blue  ? blue  - rhs.blue  : rhs.blue  - blue;
    return r * r + g * g + b * b;
  }
};

class PPM
{
private:
  uint64_t _width, _height, _depth;
  std::vector<RGBColor> _bitmap;

public:
  PPM() {}
  PPM(uint64_t width, uint64_t height, uint64_t depth) : _width(width), _height(height), _depth(depth)
  {
    _bitmap = std::vector<RGBColor>(_height * _width);
  }
  
  uint64_t  width() const { return _width;  }
  uint64_t height() const { return _height; }
  
  int read_file(const char *file_name)
  {
    std::ifstream ifs(file_name, std::ios::binary);
    
    if (!ifs) {
      return 1;
    }

    std::string line;
    std::stringstream ss;
    
    do {
      std::getline(ifs, line);
    } while(line[0] == '#');
    
    if (line != "P6") {
      return 1;
    }
    
    do {
      std::getline(ifs, line);
    } while(line[0] == '#');
    
    ss = std::stringstream(line);
    ss >> _width >> _height;
    
    do {
      std::getline(ifs, line);
    } while(line[0] == '#');
    
    ss = std::stringstream(line);
    ss >> _depth;
    
    uint64_t buffer_size = _height * _width;
    _bitmap = std::vector<RGBColor>(buffer_size);
    
    for (uint64_t index = 0; index < buffer_size; ++index) {
      char buffer[4] = {0, 0, 0, 0};
      ifs.read(buffer, sizeof(char) * 3);
      _bitmap[index] = RGBColor(
        static_cast<uint8_t>(buffer[0]), 
        static_cast<uint8_t>(buffer[1]), 
        static_cast<uint8_t>(buffer[2])
      );
    }
    
    return 0;
  }
  
  int write_file(const char *file_name)
  {
    std::ofstream ofs(file_name, std::ios::binary);
    
    if (!ofs) {
      return 1;
    }

    ofs << "P6" << std::endl;
    
    ofs << _width << ' ' << _height << std::endl;
    
    ofs << _depth << std::endl;
    
    uint64_t buffer_size = _height * _width;
    for (uint64_t index = 0; index < buffer_size; ++index) {
      char buffer[4] = {
        static_cast<char>(_bitmap[index].red   & 0xFF), 
        static_cast<char>(_bitmap[index].green & 0xFF), 
        static_cast<char>(_bitmap[index].blue  & 0xFF), 
        0
      };
      ofs.write(buffer, sizeof(char) * 3);
    }
    
    return 0;
  }
  
  RGBColor get(uint64_t x, uint64_t y) const
  {
    return _bitmap[y * _width + x];
  }
  
  void set(uint64_t x, uint64_t y, const RGBColor &color)
  {
    _bitmap[y * _width + x] = color;
  }
};

#endif /* __PPM_H__ */
