#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>

using namespace std;

class Utility
{
public:
    static vector<uint32_t> splitString2Num(const string& text, string by);
    static vector<string> splitString(const string& text, string by);
    static vector<vector<string> > splitMessage(const string& text);
    static vector<vector<uint32_t> > splitMessage2Num(const string& text);
    static void removeWhiteSpaces(string& text);
    static void printVector(const vector<string>& data);
    static void printVector(const vector<int32_t>& data);
    static bool isComment(string text);
    static string randomString(int width);
};

#endif // UTILITY_H
