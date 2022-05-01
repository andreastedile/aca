#pragma once

#include <string>

class Image {
 private:
  std::string filename;
  int x, y, n;
  unsigned char* data;

 public:
  Image(const std::string& file_name);
  ~Image();
};
