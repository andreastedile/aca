#include <iostream>

#include "hello.h"
#include "image.h"

int main() {
  hello();

  {
    Image img("foo.jpeg");
    // img's destructor should free img's data
  }

  return 0;
}
