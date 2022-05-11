#include <iostream>

#include "hello.hpp"
#include "image.hpp"
#include "quadtree.hpp"

int main() {
    hello();

    Quadtree root("image.jpg");
    root.write_to_file("result.jpg");

    return 0;
}
