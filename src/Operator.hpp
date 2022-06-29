#ifndef OPERATOR_HPP
#define OPERATOR_HPP
#include "Node.hpp"
class Operator : public Node {
public:
    Operator(char op, std::shared_ptr<Node> left, std::shared_ptr<Node> right);

    double      evaluate() const override;
    std::string evaluateString() const override;

private:
    char op;
    std::shared_ptr<Node> left;
    std::shared_ptr<Node> right;
};
#endif 