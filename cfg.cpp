#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

void generateCFG()
{
    ifstream tacFile("tac.txt");
    ofstream dotFile("cfg.dot");

    vector<string> instructions;
    string line;

    while(getline(tacFile, line))
    {
        if(line.size() > 0)
            instructions.push_back(line);
    }

    dotFile << "digraph CFG {\n";
    dotFile << "node [shape=box];\n";

    for(int i = 0; i < instructions.size(); i++)
    {
        dotFile << "B" << i
                << " [label=\""
                << instructions[i]
                << "\"];\n";
    }

    for(int i = 0; i < instructions.size()-1; i++)
    {
        dotFile << "B" << i
                << " -> "
                << "B" << i+1
                << ";\n";
    }

    dotFile << "}\n";

    tacFile.close();
    dotFile.close();

    cout << "CFG DOT file generated successfully!\n";
}

int main()
{
    generateCFG();
    return 0;
}