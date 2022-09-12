#include "quadtree.h"
#include "../overloaded.h"

RGB<float> mean_of(const QtNode& node) {
    return std::visit(overloaded{
                          [](const Fork& fork) { return fork.mean; },
                          [](const Leaf& leaf) { return RGB<float>(leaf.pixel.r, leaf.pixel.g, leaf.pixel.b); }},
                      node);
};

RGB<float> std_of(const QtNode& node) {
    return std::visit(overloaded{
                          [](const Fork& fork) { return fork.std; },
                          [](const Leaf& leaf) { return RGB<float>{0.f, 0.f, 0.f}; }},
                      node);
};
