#ifndef SHEET_H
#define SHEET_H

#include "Overhead.hpp"
#include "Cell.hpp"
#include "TermCell.hpp"
#include "Function.hpp"
#include "TextCell.hpp"

class Sheet {
public:
    Sheet() = default;
    ~Sheet() = default;

    bool                        editCell                (size_t column, size_t row, std::string exp);
    bool                        createCell              (std::shared_ptr<Cell> &newCellPtr, const std::string &str);
    void                        deleteCell              (size_t column, size_t row);
    void                        updateDimensions        (size_t column, size_t row);
    void                        updateParentalCells     (size_t column, size_t row, bool change);
    void                        updateSheet             (const std::set<std::pair<size_t, size_t>> & children, size_t column, size_t row);
    void                        cleanSheet              (const std::set<std::pair<size_t, size_t>> & children, size_t column, size_t row);
    bool                        cycleCreation           (std::shared_ptr<Cell> & newCell,size_t column, size_t row);
    std::string                 removeWhiteSpace        (const std::string& value);
    std::pair<size_t, size_t>   getTableDimensions      () const;
    bool                        numericMatch            (const std::string& value) const;
    bool                        referenceMatch          (const std::string& value) const;
    bool                        functionMatch           (const std::string& value) const;
    bool                        aggfunctionMatch        (const std::string& value) const;
    bool                        concatMatch             (const std::string& value) const;

    std::shared_ptr<Node>       parseStringToTree       (std::shared_ptr<Cell> &newCellPtr, const std::string &str);
    std::shared_ptr<Node>       addReference            (std::shared_ptr<Cell> &newCellPtr, const std::string &str);
    std::shared_ptr<Node>       basicFunctHandler       (std::shared_ptr<Cell> &newCellPtr, const std::string &str);
    std::shared_ptr<Node>       aggregateFunctHandler   (std::shared_ptr<Cell> &newCellPtr, const std::string &str);
    std::shared_ptr<Node>       concatHandler           (std::shared_ptr<Cell> &newCellPtr, const std::string &str);

    const std::map <std::pair<size_t, size_t>, std::shared_ptr<Cell>>& getTable() const { return table; }
private:
    std::map <std::pair<size_t, size_t>, std::shared_ptr<Cell>>                 table;
    std::map <std::pair<size_t, size_t>, std::set<std::pair<size_t, size_t>>>   references;

    size_t maxX = 0;
    size_t maxY = 0;
    //std::pair<size_t, size_t>   convertToCellCoordinate(const std::string &str);
    //std::string                 convertToSheetCoordinate(size_t x);

};

#endif 