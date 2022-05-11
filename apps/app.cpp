#include <iostream>

#include "hello.hpp"
#include "image.hpp"
#include "quadtree.hpp"

int main() {
    hello();

    Quadtree root("image.jpg");

    return 0;
}
