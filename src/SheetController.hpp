#ifndef SHEETCONTROLLER_H
#define SHEETCONTROLLER_H

#include "Overhead.hpp"
#include "Sheet.hpp"

#define END 0

#define PRINT 1

#define EDIT 2

#define MOVE 3

#define PRINTCELL 4

#define HELP 5

#define SAVE 6

#define CLEAR 7

#define ASSERT 8

#define ASSIST 9


class SheetController {
public:
    SheetController() = default;
    ~SheetController() = default;


    bool    load                (Sheet & sheet);
    void    run                 (Sheet &sheet);
    int     inputReader         ();
    void    printTable          (Sheet & sheet);
    void    printHelp           ();
    void    printInstructions   ();
    void    moveTableView       ();
    void    editCell            (Sheet & sheet) const;
    void    deleteCell          (Sheet & sheet) const;
    void    showCell            (Sheet & sheet) const;
    void    saveFile            (Sheet & sheet);
    void    assertTest          (Sheet & sheet);

    std::pair<size_t, size_t> validateCoordinate(const std::string &str) const;
private:
    size_t curColumn = 1;
    size_t curRow = 1;
    size_t maxCol = 100000;
    size_t maxRow = 100000;
};

#endif 