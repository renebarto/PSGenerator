#include <iostream>
#include <include/Parser.h>

using namespace std;

int main(int argc, char * argv[])
{
    if (argc < 3)
    {
        cerr << "Usage " << argv[0] << " <input file> ... <output file>" << endl;
        return EXIT_FAILURE;
    }
    CPPParser::OptionsList options = { "-x", "c++" };
    std::vector<std::string> inputFiles;
    for (int i = 1; i < argc - 1; ++i)
    {
        if (argv[i][0] == '-')
            options.emplace_back(argv[i]);
        else
            inputFiles.emplace_back(argv[i]);
    }
    std::string outputFile = argv[argc - 1];
    std::vector<CPPParser::AST> symbolTrees;
    for (auto const & inputFile : inputFiles)
    {
        CPPParser::Parser parser(inputFile);
        if (!parser.Parse(options))
            return EXIT_FAILURE;
        symbolTrees.push_back(parser.GetAST());
    }

    for (auto & ast : symbolTrees)
    {
        ast.Show(cout, 0);
    }
    return EXIT_SUCCESS;
}