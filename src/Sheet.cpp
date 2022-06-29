#include "Sheet.hpp"

/**
 * Remove whitespace from string parameter
 * @param value
 * @return string
 */
std::string Sheet::removeWhiteSpace(const std::string &value) {
    std::string str = value;
    std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
    str.erase(end_pos, str.end());
    return str;
}

/**
 * Get max column and row of the table
 * @return pair<size_t,size_t>
 */
std::pair<size_t, size_t> Sheet::getTableDimensions() const{
    return std::make_pair(maxX,maxY);
}

/**
 * To the table adds correct type of cell based on its expression
 * Returns false if syntax of expression is incorrect or if a cycle is to be created
 * In case of error detection cell is not modified, previous value is kept if it existed
 * @param column
 * @param row
 * @param exp
 * @return bool
 */
bool Sheet::editCell(size_t column, size_t row,std::string exp) {

    std::shared_ptr<Cell> newCell = nullptr;
    std::shared_ptr<Node> treeRoot = nullptr;
    std::set<std::pair<size_t, size_t>> downstream;
    auto tableCell = table.find(std::make_pair(column,row));

    //determine type of cell
    if(exp[0] == '=' && exp.length() == 1 ){
        newCell = std::make_shared<TextCell>(exp);
    } else if ((exp[0] == '=' &&  exp[1] == '"')){
        size_t last = exp.find_last_of('"');
        if (last == 1){
            return false;
        }
        newCell = std::make_shared<TextCell>(exp.substr(2,last-2));
        newCell->setExp(exp);
    } else if (exp[0] != '=' ){
        newCell = std::make_shared<TextCell>(exp);
    } else {
        //input is mathematical exp
        newCell = std::make_shared<TermCell>(exp);
        //create expression tree
        treeRoot = parseStringToTree(newCell, removeWhiteSpace(exp.erase(0, 1)));
        newCell->setExpTree(treeRoot);
        if (treeRoot == nullptr || !newCell->Update()) {
            return false;
        }
        //check for cycled references
        downstream = newCell->getChildren();
        if (cycleCreation(newCell, column, row)) {
            std::cout << "Cycle detection.\n";
            return false;
        }
    }

    //new cell is added
    if( tableCell == table.end() ){
        table.emplace(std::make_pair(std::make_pair(column,row),newCell));
        updateSheet(downstream, column, row);
        updateDimensions(column,row);
        return true;
    }

    std::string ss = tableCell->second->getExp();
    bool bp = (ss.length() != 1 && ss[0] == '=' && ss[1]!= '\"');

    //change the values of Cell but not its type
    if ( (bp&&treeRoot) || (!bp&&!treeRoot)){
        std::string newExp = newCell->getExp();
        tableCell->second->setExp(newExp);
        if(treeRoot){
            //remove children from reference table + clear cell references + assign new exp tree + set cell dependencies
            cleanSheet(tableCell->second->getChildren(),column,row);
            tableCell->second->clearRefs();
            tableCell->second->setExpTree(treeRoot);
            tableCell->second->setChildren(downstream);
        }
        // update output value by evaluating exp tree
        tableCell->second->Update();
        //add references to table
        updateSheet(newCell->getChildren(),column,row);
        //notify any cells referencing this one but do not re-make their exp tree as only value has changed
        updateParentalCells(column,row,false);

    } //change Cell type
    else  {
        cleanSheet(tableCell->second->getChildren(),column,row);
        tableCell->second = newCell;
        updateSheet(newCell->getChildren(),column,row);
        updateParentalCells(column,row,true);
    }
    updateDimensions(column,row);

    return true;
}


/**
 * Recursive descent parser for expressions in infix notation
 * Converts expression into an expression tree
 * @param newCell
 * @param str
 * @return tree root
 */
std::shared_ptr<Node> Sheet::parseStringToTree(std::shared_ptr<Cell> &newCell, const std::string &str) {
    int level = 0;  // keep track of nested parentheses
    // case + or - outside of upper level parentheses
    for (size_t i = (str.size() - 1); i < str.size(); --i) {
        char c = str[i];
        if (c == ')') {
            ++level;
            continue;
        }
        if (c == '(') {
            --level;
            continue;
        }
        if (level > 0) continue;
        if ((c == '+' || c == '-') && i != 0) {
            std::string left(str.substr(0, i));
            std::string right(str.substr(i + 1));

            auto leftSubTree = parseStringToTree(newCell, left);
            auto rightSubTree = parseStringToTree(newCell, right);

            if(!leftSubTree || !rightSubTree){
                return nullptr;
            }
            return std::make_shared<Operator>(c, leftSubTree, rightSubTree);
        }
    }
    // case * or / outside of upper level parentheses
    for (size_t i = (str.size() - 1); i < str.size(); --i) {
        char c = str[i];
        if (c == ')') {
            ++level;
            continue;
        }
        if (c == '(') {
            --level;
            continue;
        }
        if (level > 0) continue;
        if (c == '*' || c == '/') {
            std::string left(str.substr(0, i));
            std::string right(str.substr(i + 1));

            auto leftSubTree = parseStringToTree(newCell, left);
            auto rightSubTree = parseStringToTree(newCell, right);

            if(!leftSubTree || !rightSubTree){
                return nullptr;
            }
            return std::make_shared<Operator>(c, leftSubTree, rightSubTree);
        }
    }
    if (str[0] == '(') {
        for (int i = 0; i < static_cast<int>(str.size()); ++i) {
            if (str[i] == '(') {
                ++level;
                continue;
            }
            if (str[i] == ')') {
                --level;
                if (level == 0) {
                    std::string exp(str.substr(1, i - 1));
                    return parseStringToTree(newCell, exp);
                }
                continue;
            }
        }
    } else {

        if (functionMatch(str)) {
            return basicFunctHandler(newCell, str);
        } else if (concatMatch(str)) {
            return concatHandler(newCell, str);
        } else if (aggfunctionMatch(str)) {
            return aggregateFunctHandler(newCell, str);
        } else if (referenceMatch(str)) {
            return addReference(newCell, str);
        } else if (numericMatch(str)) {
            return std::make_shared<LeafNode>(str);
        }

    }
    std::cout << "Syntax error.\n";
    return nullptr;
}


/**
 * Helper function of parseStringToTree to create function expression
 * @param newCell
 * @param str
 * @return shared_ptr<Node>
 */
std::shared_ptr<Node> Sheet::basicFunctHandler( std::shared_ptr<Cell> &newCell, const std::string &str) {
    size_t first = str.find_first_of  ('(');
    size_t last = str.find_last_of (')');
    std::string insideFunction = str.substr(first+1,last-first-1);
    std::string functionName = str.substr(0,first);

    auto tree = parseStringToTree(newCell, insideFunction);
    if(tree == nullptr){
        return nullptr;
    }
    return std::make_shared<Function>(functionName, tree);
}

/**
 * Helper function of parseStringToTree to create concat expression
 * @param newCell
 * @param str
 * @return shared_ptr<Node>
 */
std::shared_ptr<Node> Sheet::concatHandler (std::shared_ptr<Cell> &newCell, const std::string &str){
    std::vector<std::shared_ptr<Node>> references;

    size_t first = str.find_first_of  ('(');
    size_t last = str.find_last_of (')');
    std::string insideFunction = str.substr(first+1,last-first-1);
    std::string functionName = str.substr(0,first);

    size_t pos = 0;
    std::string token;

    while ((pos = insideFunction.find(',')) != std::string::npos) {
        token = insideFunction.substr(0, pos);
        if (!referenceMatch(token)){
            return nullptr;
        }
        references.push_back(parseStringToTree(newCell, token));
        insideFunction.erase(0, pos + 1);
    }
    if (!referenceMatch(insideFunction)){
        return nullptr;
    }
    references.push_back(parseStringToTree(newCell, insideFunction));

    return std::make_shared<Function>(functionName, references);
}

/**
 * Helper function of parseStringToTree to make references in expression tree
 * Assumes correct format of input
 * @param newCell
 * @param str
 * @return shared_ptr<Node>
 */
std::shared_ptr<Node> Sheet::addReference(std::shared_ptr<Cell>& newCell, const std::string &str) {

    std::pair<size_t ,size_t > coord = convertToCellCoordinate(str);

    auto tableCell = table.find(std::make_pair(coord.first,coord.second));
    std::shared_ptr<Cell> emptyCell = std::make_shared<TextCell>("");
    std::shared_ptr<Cell> offspring = nullptr;

    if (tableCell == table.end()){
        table.emplace(std::make_pair(coord.first,coord.second), emptyCell);
        offspring = emptyCell;
    } else {
        offspring = tableCell->second;
    }
    newCell->updateReferences(std::make_pair(coord.first,coord.second));
    updateDimensions(coord.first,coord.second);
    return std::make_shared<LeafNode>(offspring);

}

/**
 * Helper function of parseStringToTree to create aggregate functions in expression tree
 * @param newCell
 * @param str
 * @return shared_ptr<Node>
 */
std::shared_ptr<Node> Sheet::aggregateFunctHandler( std::shared_ptr<Cell> &newCell, const std::string &str) {
    std::vector<std::shared_ptr<Node>> children;

    size_t first = str.find_first_of  ('(');
    size_t last = str.find_last_of (')');

    std::string insideFunction = str.substr(first+1,last-first-1);
    std::string functionName = str.substr(0,first);
    size_t ff = insideFunction.find_last_of (':');

    std::ostringstream oss;

    auto Scoor = convertToCellCoordinate(insideFunction.substr(0,ff));
    auto Ecoor = convertToCellCoordinate(insideFunction.substr(ff+1,last));


    for(size_t i = Scoor.first; i <= Ecoor.first; i++){

        for(size_t j = Scoor.second; j <= Ecoor.second; j++){

            children.emplace_back(addReference(newCell,convertToSheetCoordinate(i).append(std::to_string(j))));
        }
    }

    return std::make_shared<Function>(functionName, children);
}

/**
 * In the table of upstream references update all parental cells of given coordinates
 * @param column
 * @param row
 * @param change
 */
void Sheet::updateParentalCells(size_t column, size_t row, bool change) {

    std::shared_ptr<Node> treeRoot;
    auto sheetCell = references.find(std::make_pair(column, row));
    if (sheetCell != references.end()) {
        for (const auto & parent : sheetCell->second) {
            auto tableCell = table.find({parent.first,parent.second});
            if(tableCell == table.end()){
                continue;
            }
            if(change){
                treeRoot = parseStringToTree(tableCell->second, removeWhiteSpace(tableCell->second->getExp()).substr(1));
                tableCell->second->setExpTree(treeRoot);
            }
            tableCell->second->Update();

            updateParentalCells(parent.first,parent.second,change);
        }
    }


}

/**
 * To the table of upstream references adds all children of the cell
 * @param column
 * @param row
 * @param downstream
 */
void Sheet::updateSheet(const std::set<std::pair<size_t, size_t>> & downstream, size_t column, size_t row) {

    for(const auto coord: downstream){
        auto item = references.find(coord);
        if(item == references.end()){
            references.insert({{coord.first, coord.second},{{column,row}}});
        } else {
            item->second.insert(std::make_pair(column,row));
        }
    }


}

/**
 * In the table of upstream references remove all instances of parental reference of the cell given by coordinates
 * @param children
 * @param column
 * @param row
 */
void Sheet::cleanSheet(const std::set<std::pair<size_t, size_t>> & children, size_t column, size_t row){
    for(auto child : children){
        auto sheetCell = references.find({child.first, child.second});
        if(sheetCell != references.end()){
            sheetCell->second.erase({column,row});
        }
    }

}

/**
 * DFS in reference table, search for cycle "walking" backwards by parental references
 * @param newCell
 * @param column
 * @param row
 * @return true if cycle is to be created by adding the new cell
 */
bool Sheet::cycleCreation(std::shared_ptr<Cell> & newCell,size_t column, size_t row) {

    std::set<std::pair<size_t, size_t>> visited;
    std::stack<std::pair<size_t, size_t>> stack;
    auto children = newCell->getChildren();
    if(children.find({column, row} ) != children.end() ){
        return true;
    }

    stack.push({column, row});
    while(!stack.empty()){
        auto top = stack.top();
        stack.pop();
        if(  children.find(top) != children.end()){
            return true;
        }
        auto sheetCell = references.find(top);
        if(sheetCell == references.end())
            continue;

        for( auto it : sheetCell->second){
            if(visited.count(it) == 0 ){
                stack.push(it);
            }
        }

        visited.insert(top);

    }

    return false;

}


/**
 * Expand table dimensions
 * @param column
 * @param row
 */
void Sheet::updateDimensions (size_t column, size_t row) {
    if (maxX < column)
        maxX = column;
    if (maxY < row)
        maxY = row;

}

/**
 * Clear out cell content
 * @param column
 * @param row
 */
void Sheet::deleteCell (size_t column, size_t row) {
    editCell(column, row, "");
}

/**
 * @return true if string is a number
 */
bool Sheet::numericMatch (const std::string& value) const{
    return (std::regex_match(value,std::regex(R"(^-?[0-9]\d*(\.\d+)?$)") ));
}

/**
 * @return true if string is a cell reference
 */
bool Sheet::referenceMatch (const std::string& value) const{
    return (std::regex_match(value,std::regex("^[A-Z]+[1-9][0-9]*$") ));
}


/**
 * @return true if string is a basic function
 */
bool Sheet::functionMatch (const std::string& value) const{
    return (std::regex_match(value,std::regex("^(SIN|COS|TAN|ABS|EXP|LOG)\\(.+\\)$") ));
}


/**
 * @return true if string is a aggregate function
 */
bool Sheet::aggfunctionMatch (const std::string& value) const{
    return (std::regex_match(value,std::regex("^(AVG|MAX|MIN|SUM)\\([A-Z]+[1-9][0-9]*:[A-Z]+[1-9][0-9]*\\)$") ));
}


/**
 * @return true if string is a concat function
 */
bool Sheet::concatMatch (const std::string& value) const{
    return (std::regex_match(value,std::regex("^CONCAT\\(.+\\)$" )));
}



