#ifndef CELL_H
#define CELL_H


#include "Overhead.hpp"
#include "Node.hpp"

class Cell {
public:
    Cell(std::string input);
    virtual ~Cell() = default;

    virtual bool                Update() = 0 ;
    virtual std::ostream &      print(std::ostream &os) const = 0;
    virtual const std::string & value() const = 0;
    virtual void                setExpTree(std::shared_ptr<Node> &expTree);

    const std::string&                          getExp() const;
    void                                        setExp(std::string& src);
    void                                        updateReferences(std::pair<size_t,size_t> newDownStream);
    const std::set<std::pair<size_t,size_t>>&   getChildren() const;
    void                                        setChildren(std::set<std::pair<size_t,size_t>>& refs);
    void                                        clearRefs();
    friend std::ostream                         &operator<<(std::ostream &os, const Cell &cell);
protected:
    std::string expression;
    std::set<std::pair<size_t, size_t>> children;
};

#endif 