#ifndef NODE_HPP
#define NODE_HPP

#include "Overhead.hpp"

class Node {
public:
    virtual ~Node() = default;

    virtual double                  evaluate()      const = 0;
    virtual std::string             evaluateString()const = 0;
};
#endif 