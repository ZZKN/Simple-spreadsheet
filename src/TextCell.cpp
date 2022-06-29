
#include "TextCell.hpp"


/**
 * Constructor of TextCell cell
 * @param input
 */
TextCell::TextCell(std::string input): Cell(input), output(input) {}


/**
 * Get the output value
 * @return ostream
 */
const std::string &TextCell::value() const {
    return output;
}

/**
 * Print value to stream
 * @param os
 * @return ostream
 */
std::ostream &TextCell::print(std::ostream &os) const {
    std::string exp = this->expression;
    if (exp[0] != '=' && std::count(exp.begin(), exp.end(), ',')){
        os << "\"" << exp << "\"";
    } else {
        os << output;
    }
    return os;
}


/**
 * Change the cell value
 */
bool TextCell::Update() {
    output = this->expression;
    return true;
}

