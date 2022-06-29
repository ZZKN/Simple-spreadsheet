#ifndef TERMCELL_H
#define TERMCELL_H

#include <utility>

#include "Overhead.hpp"
#include "Operator.hpp"
#include "Leaf.hpp"

class TermCell : public Cell {
public:
    TermCell(std::string input): Cell(std::move(input)){}

    bool                        Update() override;
    void                        setExpTree(std::shared_ptr<Node> &expTree) override;
    const std::string &         value() const override;
    std::ostream &              print(std::ostream &os) const override;
private:
    std::shared_ptr<Node> tree = nullptr;
    std::string output;

};

#endif 