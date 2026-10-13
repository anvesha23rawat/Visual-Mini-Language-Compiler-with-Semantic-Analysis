#include <iostream>
#include <memory>
#include <fstream>

#include "parser.h"
#include "semantic.h"
#include "cfg.h"
#include "parser.h"

int main() {

    std::cout << "Compiler Started...\n";

    // Parse Code
    std::shared_ptr<ASTNode> root = parseCode("Code.txt");

    // Open semantic output file
    std::ofstream outfile("semantic.txt");

    if (!outfile) {
        std::cerr << "Error: Could not open semantic.txt for writing\n";
        return 1;
    }

    // Redirect semantic errors to file
    std::streambuf* original_cerr = std::cerr.rdbuf();
    std::cerr.rdbuf(outfile.rdbuf());

    // Semantic Analysis
    SemanticAnalyzer analyzer;
    analyzer.analyze(root);

    // Restore cerr
    std::cerr.rdbuf(original_cerr);

    // Write Symbol Table
    outfile << "\n--- Symbol Table ---\n";

    const auto& table = analyzer.getSymbolTable();

    for (const auto& entry : table) {
        outfile << entry.first
                << " : "
                << entry.second
                << "\n";
    }

    outfile.close();

    std::cout << "Semantic Analysis Completed!\n";

    // Generate CFG
    generateCFG();

    std::cout << "CFG Generated Successfully!\n";

    std::cout << "\n--- AST ---\n";
    printAST(root);
    return 0;
}