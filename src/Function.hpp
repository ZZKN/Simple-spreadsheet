#ifndef FUNCTION_HPP

#include "Node.hpp"

class Function : public Node {
public:
    Function(std::string  name, std::shared_ptr<Node>& tree);
    Function(std::string  name, std::vector<std::shared_ptr<Node>>& expTree);

    double      evaluate()          const override;
    std::string evaluateString()    const override;
    std::string evaluateCluster()   const;

private:
    std::string name;
    std::shared_ptr<Node> tree;
    std::vector<std::shared_ptr<Node>> cluster;
};
#endif