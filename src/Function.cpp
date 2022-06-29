#include "Function.hpp"

/**
 * Constructor with expression tree root for basic functions
 * @param name
 * @param expTree
 */
Function::Function(std::string  name, std::shared_ptr<Node>& expTree): name(std::move(name)) {
    tree = expTree;
}

/**
 * Constructor with set of all referenced cells in an aggregate function
 * @param name
 * @param refs
 */
Function::Function(std::string name, std::vector<std::shared_ptr<Node>>& refs): name(std::move(name)),  tree(nullptr), cluster(refs) {}


/**
 * Evaluate mathematical expression
 * @return
 */
double Function::evaluate() const {
    return std::stod(this->evaluateString());
}

/**
 * Evaluate expression tree
 * @return string
 */

std::string Function::evaluateString() const {
    std::stringstream ss;

    if(tree == nullptr){
        return evaluateCluster();
    }

    double out = tree->evaluate();


    if ( name == "ABS" ){
        ss << std::abs(out);
    } else if ( name == "SIN" ) {
        ss << sin(out);
    } else if ( name == "COS" ) {
        ss << cos(out);
    } else if ( name == "TAN" ) {
        ss << tan(out);
    } else if ( name == "EXP" ) {
        ss << exp(out);
    } else if ( name == "LOG" ) {
        ss << log10((out));
    }

    return ss.str();

}

/**
 * Evaluate  function
 * @return string
 */
std::string Function::evaluateCluster() const {
    std::stringstream ss;
    double out = 0;

    if( name == "CONCAT" ){
        for (const auto &expTree : cluster) {
            ss << expTree->evaluateString();
        }
    } else if ( name == "AVG" ){
        for (auto &item: cluster)
            out += item->evaluate();
        ss << out/cluster.size();

    } else if ( name == "MAX" ) {

        double max = std::stod( (*cluster.begin())->evaluateString() );
        for (auto &item: cluster) {
            double curr = item->evaluate();
            if ( curr > max){
                max = curr;
            }
        }
        ss << max;
    } else if ( name == "MIN" ) {
        double min = std::stod( (*cluster.begin())->evaluateString() );
        for (auto &item: cluster) {
            double curr = item->evaluate();
            if ( curr < min){
                min = curr;
            }
        }
        ss << min;
    } else if ( name == "SUM" ) {
        for (auto &item: cluster)
            out += item->evaluate();
        ss << out;
    }


    return ss.str();
}

