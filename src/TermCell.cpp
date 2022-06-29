#include "TermCell.hpp"


/**
 * Evaluates expression tree and change the output
 * If expression tree is invalid output is changed to error message
 * @return bool
 */
bool TermCell::Update() {

    try {
        output = tree->evaluateString();
    } catch (StringEval& ex) {
        std::cout << ex.what() << std::endl;
        output = "#ERROR";
        return false;
    }

    return true;
}

/**
 * Set new expression tree;
 * @param expTree
 */
void TermCell::setExpTree(std::shared_ptr<Node> &expTree) {
    tree = expTree;
}

/**
 * Get the value of expression tree
 * @return string
 */
const std::string &TermCell::value() const {
    return output;
}

/**
 * Get the value of expression tree to ostream
 * @param
 * @return ostream
 */
std::ostream &TermCell::print(std::ostream &os) const {
    std::string exp = this->expression;
    if (std::regex_match(exp,std::regex("^=CONCAT\\(.+\\)$") )) {
        os << "=\"" << output << "\"";
    } else if(output == "#ERROR") {
        os << output << ": " << exp;
    } else {
        os << output;
    }
    return os;
}
