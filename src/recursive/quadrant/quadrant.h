#pragma once

#include "quadrant.h"
#include "rgb.h"

#include <variant>

struct Position {
    int i;
    int j;
};

struct Extents {
    Position top_left;
    int length;
};

using Subquadrant = std::variant<Extents, Position>;

Subquadrant nw_subquadrant(const Subquadrant& q);

Subquadrant ne_subquadrant(const Subquadrant& q);

Subquadrant se_subquadrant(const Subquadrant& q);

Subquadrant sw_subquadrant(const Subquadrant& q);
