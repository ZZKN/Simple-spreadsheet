
#include "SheetController.hpp"

/**
 * Validate user input of coordinates
 * @param input
 * @return std::pair<size_t, size_t>
 */
std::pair<size_t, size_t> SheetController::validateCoordinate(const std::string & input) const{
    if (!std::regex_match(input,std::regex("^[A-Z]+[1-9][0-9]*$") )) {
        std::cout << "Invalid input\n";
        return {0,0};
    }

    auto coor = convertToCellCoordinate(input);
    size_t column = coor.first;
    size_t row = coor.second;

    if(column > maxCol-10 || row > maxRow-10 ){
        std::cout << "Invalid input\n";
        return {0,0};
    }
    return coor;

}

/**
 * Print header and content of cells
 * @param sheet
 */
void SheetController::printTable(Sheet & sheet){
    auto sheetTable = sheet.getTable();
    std::ostringstream oss;
    std::string arrayTable[11][11];
    auto itLow = sheetTable.lower_bound({curColumn,curRow});
    auto itUp = sheetTable.upper_bound({curColumn+10,curRow+10});


    while ( itLow != itUp ){
        oss << itLow->second->value();
        if (itLow->first.second >= curRow && itLow->first.first >= curColumn && itLow->first.second <= curRow+10 && itLow->first.first <= 10+curColumn )  {
            arrayTable[itLow->first.second-curRow+1][itLow->first.first-curColumn+1] = oss.str();
        }

        oss.str("");
        itLow++;
    }

    std::cout << "\n#########################################################################################################################\n";

    std::cout << std::setw(10) << "" << "|";
    for ( size_t i = curColumn; i < curColumn+10; i++) {
        std::cout << std::setw(10) << convertToSheetCoordinate(i) << "|";
    }

    std::cout << "\n";
    for ( int i = 0; i <= 10; i++) {
        std::cout <<  "__________" << "|";
    }

    std::cout << "\n";

    for ( int i = 1; i <= 10; i++){
        std::cout << std::setw(10) << i+curRow-1 << "|";

        for (int j = 1; j <= 10; j++){
            if (arrayTable[i][j].empty()){
                std::cout << std::setw(10) << "" << "|";
            } else {
                std::string str = arrayTable[i][j];
                if (str.size() > 10) {
                    std::cout << str.substr(0, 8).append("..") << "|";
                } else {
                    std::cout << std::setw(10) << str << "|";
                }
            }
        }
        std::cout << "\n";
    }

}

/**
 * Print input options
 */
void SheetController::printHelp(){

    std::cout << "\nPress:\n";
    std::cout << "\t-'p' to print the sheet\n" ;
    std::cout << "\t-'c' to see details of a cell.\n" ;
    std::cout << "\t-'e' to edit the contents of a cell.\n" ;
    std::cout << "\t-'x' to remove the contents of a cell.\n" ;
    std::cout << "\t-'m' to move the sheet.\n" ;
    std::cout << "\t-'s' to save the spreadsheet.\n" ;
    std::cout << "\t-'i' to see further instructions.\n" ;
    std::cout << "\t-'q' to quit.\n" ;
}


/**
 * Print detailed instructions
 */
void SheetController::printInstructions() {
    std::cout << "\nInput:\n";
    std::cout << "\t-p == print view of the table in size 10x10, this view can be adjusted by option \"-m == move\"\n" ;
    std::cout << "\t\t coordinates are noted in the header and the left-most column \n" ;
    std::cout << "\t-e == edit edit the contents of a cell, input cell names in style of \"A1\" (case sensitive)\n" ;
    std::cout << "\t-x == remove the the cell from the table.\n" ;
    std::cout << "\nOperators:\n" ;
    std::cout << "\t+, -, *, /\n" ;
    std::cout << "\nFunctions:\n" ;
    std::cout << "\tSIN(x), COS(x), TAN(x), ABS(x), EXP(x), LOG(X), CONCAT(X,Y,..)\n" ;
    std::cout << "\tCONCAT supports only cell references, the rest support all mathematical expression (within the capabilities of this spreadsheet)\n" ;
    std::cout << "\nAggregate Functions:\n" ;
    std::cout << "\tAVG(x:y), MAX(x:y), MIN(x:y), SUM(x:y)\n" ;
    std::cout << "\tsupports only cell references\n" ;
}

/**
 * Input cell and edit its content
 * @param sheet
 */
void SheetController::editCell(Sheet & sheet) const{
    std::string input;
    std::cout << "(edit)>Enter cell to edit: ";
    getline(std::cin,input);
    std::pair<size_t, size_t> coor = validateCoordinate(input);
    if(coor.first == 0){
        return;
    }
    std::cout << "(edit)>Enter expression: ";
    getline(std::cin,input);

    sheet.editCell(coor.first,coor.second,input);

}

/**
 * Control input of coordinates and move the table so that given cell is in the left upper corner
 */
void SheetController::moveTableView(){
    std::string input;
    std::cout << "(move)>Enter cell to view in the left upper corner: ";
    getline(std::cin,input);
    auto coor = validateCoordinate(input);
    if(coor.first == 0){
        return;
    }
    curColumn = coor.first;
    curRow = coor.second;
}


/**
 * Control input coordinates and write out contents of the cell
 * @param sheet
 *
 */
void SheetController::showCell(Sheet & sheet) const{
    std::string input;
    std::cout << "(zoom-in)>Enter cell: ";
    std::cin >> input;
    std::ostringstream oss;

    auto coor = validateCoordinate(input);


    auto table = sheet.getTable();
    auto cellIt = table.find({coor.first,coor.second});

    if ( cellIt == table.end()){
        std::cout << "Cell: " << input << " is empty" << std::endl;
        return;
    }
    cellIt->second->print(oss);
    std::cout << "(zoom-in)Contents of: " << input << std::endl;
    std::cout << "(zoom-in)Expression: " << cellIt->second->getExp() << std::endl;
    std::cout << "(zoom-in)Value: " << oss.str() << std::endl;

}


/**
 * Read user input
 */
int SheetController::inputReader() {
    char x;
    std::cin >> x;
    std::cin.ignore(INT_MAX, '\n');

    if(std::cin.eof()){
        return END;
    }

    switch (x) {
        case 'p':
            return PRINT;
            break;
        case 'e':
            return EDIT;
            break;
        case 'x':
            return CLEAR;
            break;
        case 'm':
            return MOVE;
            break;
        case 'c':
            return PRINTCELL;
            break;
        case 'h':
            return HELP;
            break;
        case 'a':
            return ASSERT;
            break;
        case 'i':
            return ASSIST;
            break;
        case 's':
            return SAVE;
            break;
        case 'q':
            return END;
            break;
        default:
            std::cout << "Please select a valid option." << std::endl;
            return -1;
    }
}


/**
 * Handle user input and following actions
 * @param sheet
 */
void SheetController::run(Sheet & sheet){

    printHelp();
    while (1) {
        // print prompt
        std::cout << ">";
        // get user input
        int choice = inputReader();

        if (choice == PRINT) {
            printTable(sheet);
        } else if (choice == EDIT) {
            editCell(sheet);
            printTable(sheet);
        } else if (choice == MOVE) {
            moveTableView();
        } else if (choice == CLEAR) {
            deleteCell(sheet);
        } else if (choice == PRINTCELL) {
            showCell(sheet);
        } else if (choice == ASSERT) {
            assertTest(sheet);
        } else if (choice == ASSIST) {
            printInstructions();
        } else if (choice == HELP) {
            printHelp();
        } else if (choice == SAVE) {
            saveFile(sheet);
        } else if (choice == END) {
            break;
        }


        printHelp();
    }


}

/**
 * Control user input of file name and load sheet
 * Returns false if unable to open file
 * @param sheet
 * @return bool
 */
bool SheetController::load(Sheet &sheet) {
    std::fstream savedSheet;
    std::cout << "Enter name of the spreadsheet (no format):" << std::endl;
    std::string sheetName;

    while (std::getline(std::cin, sheetName)) {
        if (!sheetName.empty()) {
            savedSheet.open("./examples/" + sheetName.append(".csv"));
            break;
        }
    }

    if(!savedSheet.good()){
        std::cout << "Unable to open file.\n";
        return false;
    }

    std::string line, token;
    size_t y = 1, x = 1;
    size_t pos;
    size_t quotes = 0;
    while (getline(savedSheet, line)) {
        x = 1;
        pos = line.find(',');
        while ( pos != std::string::npos) {
            token += line.substr(0, pos);
            line.erase(0, pos + 1);
            quotes = std::count(token.begin(), token.end(), '"');
            if (!token.empty() && ( quotes == 0 ||  (token.back() == '"' && (quotes & 1) == 0)) ) {
                sheet.editCell(x, y, token);
                token = "";
            } else if (!token.empty()){
                token.append(",");
            }
            ++x;
            pos = line.find(',');
        }
        ++y;
    }

    return true;
}

/**
 * Control user input of file name and save sheet
 * @param sheet
 */
void SheetController::saveFile(Sheet & sheet){
    std::ofstream newFile;
    std::cout << "Enter name of the spreadsheet (no format):" << std::endl;
    std::string sheetName;
    while (std::getline(std::cin, sheetName)) {
        if (!sheetName.empty()) {
            newFile.open("./examples/" + sheetName.append(".csv"));
            break;
        }
    }
    auto table = sheet.getTable();
    auto dim = sheet.getTableDimensions();

    for (size_t y = 1; y <= dim.second; ++y) {
        for (size_t x = 1; x <= dim.first; ++x) {
            auto cellsIter = table.find(std::make_pair(x, y));
            if(cellsIter == table.end()){
                newFile << ',';
                continue;
            }
            newFile << *cellsIter->second << ',';
        }
        newFile << '\n';
    }

    std::cout << "File saved.";
}


/**
 * Empty out a cell
 * @param sheet
 */
void SheetController::deleteCell(Sheet &sheet) const {
    std::string input;
    std::cout << "(delete)>Enter cell to delete: ";
    getline(std::cin,input);
    std::pair<size_t, size_t> coor = validateCoordinate(input);
    if(coor.first == 0){
        return;
    }

    sheet.deleteCell(coor.first,coor.second);
    std::cout << "(delete)>Cell successfully deleted";
}


/**
 * Asserts for testing
 * @param sheet
 */
void SheetController::assertTest(Sheet & sheet){

    //operator priority and parentheses levels
    assert(sheet.editCell(1,9,"=10+2* 6 + 2"));
    assert(sheet.editCell(1,10,"=10-0.7* (6 + 2)"));
    assert(! sheet.editCell(3,6,"=10+2* 6 + 2)"));
    assert(! sheet.editCell(3,7,"=10+(2*) 6 + 2"));

    //incorrect exp embedded in functions
    assert(sheet.editCell(3,4,"=SIN(10+2* 6 + 2)"));
    assert(! sheet.editCell(3,5,"=SIN(10+2* (6 + aaa))"));
    assert(! sheet.editCell(3,6,"=LOG(AA2"));
    assert(! sheet.editCell(3,6,"=CONCAT(aaa)"));
    assert(! sheet.editCell(3,6,"=SUM(H11:aaa)"));

    //cycle detection in function
    assert(sheet.editCell(2,2,"=1"));
    assert(sheet.editCell(1,1,"=B2+C2"));
    assert(sheet.editCell(3,2,"=3+C3"));
    assert(sheet.editCell(3,1,"=10+ SIN(1)"));
    assert(! sheet.editCell(3,3,"=10+ SIN(10+C2)"));
    assert(sheet.editCell(3,3,"=10+ SIN(10+B2)"));

    //if cell is empty its value is 0
    assert(sheet.editCell(1,1,"=1"));
    assert(sheet.editCell(1,2,"=1"));
    assert(sheet.editCell(1,3,"=1"));
    assert(sheet.editCell(1,4,"=1"));

    //assert(sheet.editCell(1,5,"=1"));
    assert(sheet.editCell(1,6,"=1"));
    assert(sheet.editCell(1,7,"=10 + SUM(A1:A6)"));
    assert(sheet.editCell(1,6,"=10"));

    //text cells test
    assert(sheet.editCell(1,1,"=1"));
    assert(! sheet.editCell(1,2,"=SIN()"));
    assert(sheet.editCell(1,3,"=CONCAT(A2, A1 )"));
    assert(sheet.editCell(2,5,"7,aaa\" p,aa "));
    assert(sheet.editCell(1,1,"ffff") );
    assert(sheet.editCell(1,4,"=\" --  \" --"));
    assert(sheet.editCell(2,2,"=CONCAT(A2,A1)"));

    //syntax error with self reference
    assert(sheet.editCell(4,1,"=1"));
    assert(sheet.editCell(4,2,"=D1+1"));
    assert(sheet.editCell(4,1,"aaa"));
    //first is detected evaluation of not a number
    assert(! sheet.editCell(4,3,"=SUM(D1:E10)"));
    assert(sheet.editCell(4,1,"=1"));
    //then self referencing
    assert(! sheet.editCell(4,3,"=SUM(D1:E10)"));

    //cycle detection
    assert(sheet.editCell(4,1,"=1"));
    assert(sheet.editCell(4,2,"=1+D1+D4"));
    assert(sheet.editCell(4,4,"=1+D3"));
    assert(! sheet.editCell(4,3,"=2+D2"));

    //unknown operator test
    assert(! sheet.editCell(5,1,"=(1*6 ) + 4^2"));
    assert(sheet.editCell(5,1,"=(1*6 ) + 4/2"));

    //cycle detection
    assert(sheet.editCell(5,1,"=E2+F2"));
    assert(sheet.editCell(5,2,"=1"));
    assert(sheet.editCell(6,2,"=F1+F3"));
    assert(sheet.editCell(6,4,"=E1+1"));
    assert(sheet.editCell(7,1,"=F4+1"));
    assert(sheet.editCell(7,2,"=1"));
    assert(!sheet.editCell(6,3,"=G2+G1"));

}