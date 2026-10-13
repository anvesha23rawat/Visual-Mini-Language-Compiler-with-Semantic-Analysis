#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
#include <unordered_map>
#include <memory>
#include "ast.h"

class SemanticAnalyzer {
private:
    std::unordered_map<std::string, std::string> symbolTable;

    // Recursively check used variables
    void checkUsedVariables(const std::shared_ptr<ASTNode>& exprNode) const;

public:

    // Insert declaration
    void insert(const std::string& name, const std::string& type);

    // Analyze AST
    void analyze(const std::shared_ptr<ASTNode>& node);

    // Print symbol table
    void print() const;

    // ADD THIS FUNCTION
    const std::unordered_map<std::string, std::string>& getSymbolTable() const {
        return symbolTable;
    }
};

#endif