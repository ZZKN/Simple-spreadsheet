#include "Overhead.hpp"


/**
 * Convert hybrid coordinates to size_t values
 * @param str
 * @return pair<size_t,size_t>
 */
std::pair<size_t, size_t> convertToCellCoordinate(const std::string &str) {
    size_t x = 0;
    const char *colstr = str.c_str();
    size_t i;
    for (i = 0; i <= str.size() - 1; ++i) {
        if (str[i] < 65 || str[i] > 90)
            break;
        x = 26*x + colstr[i] - 'A' + 1;
    }

    return std::make_pair(x, std::stoul(str.substr(i)));
}
StringEval::StringEval(const std::string& message) : message_(message) {}
/**
 * Convert column number to alphabetical notation
 * @param x
 * @return string
 */
std::string convertToSheetCoordinate(size_t x) {

    std::string column = "";

    while (x > 0){
        int modulo = (x -1) % 26;
        char c = 'A'+modulo;
        column = std::string(1, c).append(column);
        x = (x - modulo) / 26;
    }
    return column;
}


