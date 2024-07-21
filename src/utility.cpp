#include <cstdio>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <climits>
#include "AlfSimulator/utility.h"
#include "AlfSimulator/print.h"


vector<uint32_t> Utility::splitString2Num(const string &text, string by)
{
    vector<uint32_t> result;

    vector<string> nums = splitString(text, by);

    for (size_t j = 0; j < nums.size(); j++)
    {
        if (nums[j].find('x') != string::npos)
        {
            result.push_back(stoi(nums[j].substr(nums[j].find('x') + 1), NULL, 16));
        }
        else
        {
            result.push_back(stoi(nums[j], NULL, 10));
        }
    }

    return result;
}

vector<string> Utility::splitString(const string& text, string by)
{
    vector<string> result;
    string temp = text;
    
    while(temp.size())
    {
        size_t index = temp.find(by);
        if(index != string::npos)
        {
            result.push_back(temp.substr(0, index));
            temp = temp.substr(index + 1);
            /*if(temp.size() == 0)
            {
                result.push_back(temp);
            }*/
        }
        else
        {
            result.push_back(temp);
            temp = "";
            break;
        }
    }

    return result;
}

vector<vector<string> > Utility::splitMessage(const string& text)
{
    vector<vector<string> > result;

    vector<string> lines = splitString(text, "\n");

    for (size_t i = 0; i < lines.size(); i++)
    {
        result.push_back(splitString(lines[i], ","));
    }

    return result;
}

vector<vector<uint32_t> > Utility::splitMessage2Num(const string &text)
{
    vector<vector<uint32_t> > result;
    vector<string> lines = splitString(text, "\n");

    for (size_t i = 0; i < lines.size(); i++)
    {
        result.push_back(splitString2Num(lines[i], ","));
    }

    return result;
}

void Utility::removeWhiteSpaces(string& text)
{
    text.erase(remove_if(text.begin(), text.end(), [](unsigned char c){return isspace(c);}), text.end());
    text.erase(remove_if(text.begin(), text.end(), [](unsigned char c){return iscntrl(c);}), text.end());
}

void Utility::printVector(const vector<string> &data)
{
    std::cout<<"vector string\n";
    for (int j = 0; j < data.size(); j++) cout << data[j] << "\n";
}

void Utility::printVector(const vector<int32_t> &data)
{
    std::cout<<"vector int\n";
    for (int j = 0; j < data.size(); j++) cout << data[j] << "\n";
}

string Utility::randomString(int width)
{
    char random[width + 1];
    for (size_t j = 0; j < width; j++)
    {
       sprintf(random + j, "%x", rand() % 16);
    }
    
    return string(random);
}

bool Utility::isComment(string text)
{
    if (text.find("#") != string::npos) // If '#' is present, it means line is comment, ('#' should be leading line)
        return true;
    return false;
}