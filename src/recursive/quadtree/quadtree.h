#pragma once

#include "quadrant.h"
#include "rgb.h"

#include <memory>
#include <variant>

struct Fork;
struct Leaf;

using QtNode = std::variant<Fork, Leaf>;

struct Fork {
    std::unique_ptr<const QtNode> nw;
    std::unique_ptr<const QtNode> ne;
    std::unique_ptr<const QtNode> se;
    std::unique_ptr<const QtNode> sw;
    Extents extents;
    RGB<float> std;
    RGB<float> mean;
};

struct Leaf {
    Position position;
    RGB<unsigned char> pixel;
};

RGB<float> mean_of(const QtNode& node);

RGB<float> std_of(const QtNode& node);
