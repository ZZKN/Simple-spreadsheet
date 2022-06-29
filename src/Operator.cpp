
#include "Operator.hpp"

/**
 * Constructor for binary operation
 * @param op
 * @param left
 * @param right
 */
Operator::Operator(char op, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
        : op(op), left(std::move(left)), right(std::move(right)) {

}

/**
 * Evaluate numeric value
 * @return double
 */
double Operator::evaluate() const {
    if (op == '+') {
        return left->evaluate() + right->evaluate();
    } else if (op == '-') {
        return left->evaluate() - right->evaluate();
    } else if (op == '*') {
        return left->evaluate() * right->evaluate();
    } else if (op == '/') {
        double rightNumber = right->evaluate();
        if (rightNumber == 0)
            throw std::runtime_error("Division by 0");
        return left->evaluate() / right->evaluate();
    }

    throw std::runtime_error("Syntax error");
}

/**
 * Evaluete string expression
 * @return string
 */
std::string Operator::evaluateString() const {
    std::stringstream ss;
    ss << this->evaluate();
    std::string p = ss.str();
    return ss.str();
}


