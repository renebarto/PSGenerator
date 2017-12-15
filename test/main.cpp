#include <unittest-c++/UnitTestC++.h>

#include <fstream>
#include "unittest-c++/OSAL.h"
#include "unittest-c++/Console.h"

static const std::string moduleName = "PSGenerator";

int Usage(UnitTestCpp::Console & console)
{
    console << fgcolor(UnitTestCpp::ConsoleColor::Green) << "TODO: explain command line" << endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Default);
    return EXIT_FAILURE;
}

#if defined(UNICODE) || defined(_UNICODE)
int wmain(int argc, const wchar_t * argv[])
#else

int main(int argc, const char * argv[])
#endif
{
    UnitTestCpp::Console console;

    console << fgcolor(UnitTestCpp::ConsoleColor::Magenta | UnitTestCpp::ConsoleColor::Intensity);
    console << "Running tests for: " << moduleName << std::endl;
    console << fgcolor(UnitTestCpp::ConsoleColor::Default);
    std::string applicationName = argv[0];
    std::string xmlOutput;
    bool gtestEmulation{};

    console << fgcolor(UnitTestCpp::ConsoleColor::Yellow) << "Command line arguments:" << endl;
    for (int i = 1; i < argc; ++i)
    {
        console << i << ": " << argv[i] << endl;
    }
    if (argc > 2)
    {
        if (strcasecmp(argv[1], "--xml") == 0)
            xmlOutput = argv[2];
        else
            return Usage(console);
    } else if (argc > 1)
    {
        if (strcasecmp(argv[1], "--gtest") == 0)
            gtestEmulation = true;
    }
    console << fgcolor(UnitTestCpp::ConsoleColor::Default);

    console << "Application: " << applicationName << std::endl;
    console << "XML output : " << xmlOutput << std::endl;

    int result = 0;

    if (!xmlOutput.empty())
    {
        std::basic_ofstream<char> outputFile;

        outputFile.open(xmlOutput);
        UnitTestCpp::XMLTestReporter reporter(outputFile);
        result = RunAllTests(reporter);
    } else if (gtestEmulation)
    {
        UnitTestCpp::ConsoleGTestReporter reporter;
        result = RunAllTests(reporter);
    } else
    {
        UnitTestCpp::ConsoleTestReporter reporter;
        result = RunAllTests(reporter);
    }

    return result;
}