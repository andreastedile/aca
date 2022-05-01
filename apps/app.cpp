#include <iostream>

#include "hello.hpp"
#include "image.hpp"

int main() {
    hello();

    {
        Image img("foo.jpeg");
        // img's destructor should free img's data
    }

    return 0;
}
