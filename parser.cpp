#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

std::shared_ptr<ASTNode> parseCode(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open file " << filename << "\n";
        return nullptr;
    }

    auto program = std::make_shared<ASTNode>("Program", "");

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string firstWord;
        iss >> firstWord;

        // ------------------ INT DECLARATION ------------------
        if (firstWord == "int") {
            std::string varDecl;
            std::getline(iss, varDecl);
            line = varDecl;

            size_t eqPos = line.find('=');
            if (eqPos != std::string::npos) {
                std::string varName = line.substr(0, eqPos);
                varName.erase(varName.find_last_not_of(" \t") + 1);
                varName.erase(0, varName.find_first_not_of(" \t"));

                auto declNode = std::make_shared<ASTNode>("Declaration", "");
                declNode->children.push_back(std::make_shared<ASTNode>("Type", "int"));
                declNode->children.push_back(std::make_shared<ASTNode>("Name", varName));
                program->children.push_back(declNode);

                std::string rhsExpr = line.substr(eqPos + 1);
                rhsExpr.erase(rhsExpr.find_last_not_of(" ;\t") + 1);
                rhsExpr.erase(0, rhsExpr.find_first_not_of(" \t"));

                auto assignNode = std::make_shared<ASTNode>("Assignment", varName);
                std::istringstream rhsStream(rhsExpr);
                std::string token;

                while (rhsStream >> token) {
                    if (token == "+" || token == "-" || token == "*" || token == "/") {
                        assignNode->children.push_back(std::make_shared<ASTNode>("Operator", token));
                    } else if (isdigit(token[0])) {
                        assignNode->children.push_back(std::make_shared<ASTNode>("Literal", token));
                    } else {
                        assignNode->children.push_back(std::make_shared<ASTNode>("Identifier", token));
                    }
                }

                program->children.push_back(assignNode);
            } else {
                std::istringstream nameStream(line);
                std::string varName;
                nameStream >> varName;

                if (!varName.empty() && varName.back() == ';')
                    varName.pop_back();

                auto declNode = std::make_shared<ASTNode>("Declaration", "");
                declNode->children.push_back(std::make_shared<ASTNode>("Type", "int"));
                declNode->children.push_back(std::make_shared<ASTNode>("Name", varName));
                program->children.push_back(declNode);
            }
        }

        // ------------------ IF STATEMENT ------------------
        else if (firstWord == "if") {
            size_t start = line.find('(');
            size_t end = line.find(')');

            if (start != std::string::npos && end != std::string::npos) {
                std::string condition = line.substr(start + 1, end - start - 1);

                std::istringstream condStream(condition);
                std::string left, op, right;
                condStream >> left >> op >> right;

                auto ifNode = std::make_shared<ASTNode>("If", "");

                auto condNode = std::make_shared<ASTNode>("Condition", "");
                condNode->children.push_back(std::make_shared<ASTNode>("Identifier", left));
                condNode->children.push_back(std::make_shared<ASTNode>("Operator", op));

                if (isdigit(right[0]))
                    condNode->children.push_back(std::make_shared<ASTNode>("Literal", right));
                else
                    condNode->children.push_back(std::make_shared<ASTNode>("Identifier", right));

                ifNode->children.push_back(condNode);

                program->children.push_back(ifNode);
            }
        }

        // ------------------ ASSIGNMENT ------------------
        else {
            size_t eqPos = line.find('=');
            if (eqPos != std::string::npos) {
                std::string varName = line.substr(0, eqPos);
                varName.erase(varName.find_last_not_of(" \t") + 1);
                varName.erase(0, varName.find_first_not_of(" \t"));

                auto assignNode = std::make_shared<ASTNode>("Assignment", varName);

                std::string rhsExpr = line.substr(eqPos + 1);
                rhsExpr.erase(rhsExpr.find_last_not_of(" ;\t") + 1);
                rhsExpr.erase(0, rhsExpr.find_first_not_of(" \t"));

                std::istringstream rhsStream(rhsExpr);
                std::string token;

                while (rhsStream >> token) {
                    if (token == "+" || token == "-" || token == "*" || token == "/") {
                        assignNode->children.push_back(std::make_shared<ASTNode>("Operator", token));
                    } else if (isdigit(token[0])) {
                        assignNode->children.push_back(std::make_shared<ASTNode>("Literal", token));
                    } else {
                        assignNode->children.push_back(std::make_shared<ASTNode>("Identifier", token));
                    }
                }

                program->children.push_back(assignNode);
            }
        }
    }

    return program;
}
