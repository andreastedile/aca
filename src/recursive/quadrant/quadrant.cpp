#include "quadrant.h"
#include "../overloaded.h"

Subquadrant nw_subquadrant(const Subquadrant& subq) {
    return std::visit(overloaded{
                          [](const Extents& extents) -> Subquadrant {
                              return Extents{extents.top_left.i, extents.top_left.j, extents.length / 2};
                          },
                          [](const Position& position) -> Subquadrant {
                              return position;
                          }},
                      subq);
}

Subquadrant ne_subquadrant(const Subquadrant& subq) {
    return std::visit(overloaded{
                          [](const Extents& extents) -> Subquadrant {
                              return Extents{extents.top_left.i, extents.top_left.j + extents.length / 2,
                                             extents.length / 2};
                          },
                          [](const Position& position) -> Subquadrant {
                              return position;
                          }},
                      subq);
}

Subquadrant se_subquadrant(const Subquadrant& subq) {
    return std::visit(overloaded{
                          [](const Extents& extents) -> Subquadrant {
                              return Extents{extents.top_left.i + extents.length / 2, extents.top_left.j + extents.length / 2,
                                             extents.length / 2};
                          },
                          [](const Position& position) -> Subquadrant {
                              return position;
                          }},
                      subq);
}

Subquadrant sw_subquadrant(const Subquadrant& subq) {
    return std::visit(overloaded{
                          [](const Extents& extents) -> Subquadrant {
                              return Extents{extents.top_left.i + extents.length / 2, extents.top_left.j,
                                             extents.length / 2};
                          },
                          [](const Position& position) -> Subquadrant {
                              return position;
                          }},
                      subq);
}
