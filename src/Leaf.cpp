
#include "Leaf.hpp"

/**
 * Constructor for simple numeric value
 * @param value
 */
LeafNode::LeafNode(std::string value) : stringValue(std::move(value)) {}

/**
 * Constructor for cell reference
 * @param cell
 */
LeafNode::LeafNode(std::weak_ptr<Cell> cell) : cellPtr(std::move(cell)) {}

/**
 * Evaluate string value
 * @return string
 */
std::string LeafNode::evaluateString() const {
    if (cellPtr.lock()) {
        return cellPtr.lock()->value();
    }
    return stringValue;
}

/**
 * Return numeric value of this endpoint
 * @return
 */
double LeafNode::evaluate() const {
    std::string tmp = stringValue;
    if (cellPtr.lock()) {
        tmp = cellPtr.lock()->value();
    }

    char *pEnd = nullptr;
    double number = std::strtod(tmp.c_str(), &pEnd);
    if (*pEnd) {
        throw StringEval("Cannot evaluate not a number.");
    }
    return number;
}