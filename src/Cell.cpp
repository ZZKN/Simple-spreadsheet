
#include "Cell.hpp"

/**
 * Abstract class constructor
 * @param input
 */
Cell::Cell(std::string input): expression(std::move(input)){ }

/**
 * Return expression
 * @return string
 */
const std::string& Cell::getExp() const{
    return expression;
}

/**
 * Operator << to call print of derived classes
 * @param os
 * @param cell
 * @return ostream
 */
std::ostream &operator<<(std::ostream &os, const Cell &cell) {

    return cell.print(os);
}

/**
 * Set new expression tree root
 * @param treeRoot
 */
void Cell::setExpTree(std::shared_ptr<Node>& treeRoot) {
    treeRoot->evaluateString();
}


/**
 * Set new expression
 * @param src
 */
void Cell::setExp(std::string & src) {
    expression = std::move(src);
}

/**
 * Add cell to the set of downstream references
 * @param newDownStream
 */
void Cell::updateReferences(std::pair<size_t, size_t> newDownStream) {
    children.insert(newDownStream);

}
/**
 * Get all downstream references contained in expression of the cell
 * @return set of references
 */
const std::set<std::pair<size_t , size_t>> &Cell::getChildren() const {
    return children;
}

/**
 * Set
 * @return std::set<std::pair<size_t , size_t>>
 */
void Cell::setChildren(std::set<std::pair<size_t, size_t>> & refs)  {
    Cell::children = refs;
}


/**
 * Get all downstream references
 */
void Cell::clearRefs()  {
    return children.clear();
}


