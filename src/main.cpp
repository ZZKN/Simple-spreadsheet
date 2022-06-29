#include <iostream>

#include "SheetController.hpp"
#include "Sheet.hpp"


void help(){
    std::cout << "\nSpreadsheet Editor\n";
    std::cout << "\tAn application that implements a simple editor that contains a table where it processes text and mathematical expressions.\n";

    std::cout << "\tCells modify their contents based on changes in referenced cells.\n";
    std::cout << "\tThe editor detects cycles and does not allow the creation of an expression that would cause the cycle.\n";
    std::cout << "\tThe editor allows you to process parentheses and aggregate functions across columns, rows, and areas.\n";
    std::cout << "\tEach newly created table can be saved to a file and reloaded.\n";
}

void options(){
    std::cout << "\nOptions:\n";
    std::cout << "\t-'n' for a new sheet.\n" ;
    std::cout << "\t-'l' to load a sheet.\n" ;
    std::cout << "\t-'h' for help.\n" ;
    std::cout << "\t-'q' to quit.\n" ;
}

int main() {
    std::cout << "\n######################\n  Simple Spreadsheet  \n######################\n" << std::endl;

    char key;

    options();
    std::cout << ">";
    while (std::cin >> key) {
        if (key == 'n') {
            Sheet sheet;
            SheetController().run(sheet);
            break;
        } else if (key == 'l') {
            Sheet sheet;
            SheetController controller;
            if(controller.load(sheet)) {
                controller.run(sheet);
            }
            break;
        } else if (key == 'q') {
            break;
        } else if (key == 'h') {
            help();
        } else {
            std::cout << "Please select a valid option.\n>";
        }

        std::cout << ">";
    }

    std::cout<< "\nGoodbye!!\n";
    return 0;
}

