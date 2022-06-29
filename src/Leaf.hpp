#ifndef LEAF_HPP
#define LEAF_HPP

#include "Node.hpp"
#include "Cell.hpp"
class LeafNode : public Node {
public:
    explicit LeafNode(std::string value);
    explicit LeafNode(std::weak_ptr<Cell> cell);

    double                  evaluate()          const override;
    std::string             evaluateString()    const override;

private:
    std::string stringValue;
    std::weak_ptr<Cell> cellPtr;
};
#endif 