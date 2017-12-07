#include "include/Utility.h"

using namespace std;

namespace Utility
{

string Trim(const string & input)
{
    size_t first = 0;
    while ((first < input.length()) && (input[first] == ' '))
        first++;
    size_t last = input.length();
    while ((last > first) && (input[last - 1] == ' '))
        last--;
    return input.substr(first, last - first);
}

void Split(const string & input, char delimiter, vector<string> & output)
{
    size_t startPos = 0;
    size_t delimiterPos = input.find(delimiter, 0);
    output.clear();

    while (delimiterPos != string::npos)
    {
        output.push_back(Trim(input.substr(startPos, delimiterPos - startPos)));
        startPos = delimiterPos + 1;
        delimiterPos = input.find(delimiter, startPos);
    }
    output.push_back(Trim(input.substr(startPos)));
}

void SplitPath(const string & path, string & directory, string & fileName, string & extension)
{
    directory = {};
    fileName = {};
    extension = {};
    size_t lastSlashPos = path.find_last_of('/');
    if (lastSlashPos != string::npos)
    {
        directory = path.substr(0, lastSlashPos);
        fileName = path.substr(lastSlashPos + 1);
        size_t lastDotPos = fileName.find_last_of('.');
        if (lastDotPos != string::npos)
        {
            extension = fileName.substr(lastDotPos + 1);
            fileName = fileName.substr(0, lastDotPos);
        }
    }
    else
    {
        fileName = path;
        size_t lastDotPos = path.find_last_of('.');
        if (lastDotPos != string::npos)
        {
            extension = fileName.substr(lastDotPos + 1);
            fileName = fileName.substr(0, lastDotPos -  lastSlashPos - 1);
        }
    }
}

} // namespace Utility
