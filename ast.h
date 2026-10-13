#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>

struct ASTNode {
    std::string type;
    std::string value;
    std::vector<std::shared_ptr<ASTNode>> children;

    ASTNode(const std::string& t, const std::string& v) : type(t), value(v) {}
};

inline void printAST(const std::shared_ptr<ASTNode>& node, int indent = 0) {
    if (!node) return;
    std::cout << std::string(indent, ' ') << node->type << ": " << node->value << "\n";
    for (auto& child : node->children) {
        printAST(child, indent + 2);
    }
}

#endif 
