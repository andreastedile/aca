#include "node.h"

CUDA_HOSTDEV Node::Node(const RGB<float>& mean, const RGB<float>& std, Type type) : mean(mean), std(std), type(type){};
CUDA_HOSTDEV Node::Node() : Node({0, 0, 0}, {0, 0, 0}, Type::LEAF) {};
