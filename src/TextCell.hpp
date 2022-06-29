#ifndef TEXT_HPP
#define TEXT_HPP

#include "Cell.hpp"
class TextCell : public Cell {
public:
    explicit TextCell(std::string formula);

    bool                Update() override;
    const std::string & value() const override;
    std::ostream &      print(std::ostream &os) const override;
private:
    std::string output;
};
#endif 