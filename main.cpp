#include <iostream>
#include <include/Parser.h>

using namespace std;

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        cerr << "Usage " << argv[0] << " <file>" << endl;
        return EXIT_FAILURE;
    }
    CPPParser::Parser parser(argv[1]);
    CPPParser::OptionsList options;
    if (!parser.Parse(options))
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}