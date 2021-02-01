#pragma once
#include "TupleRead.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <regex>
#include <string>

bool isDigit(string& str)
{
    regex rx("\\s*-?\\d*(\\.)?\\d*\\s*");
    return (regex_match(str, rx) && str != ".");
}

template<typename T>
void writeToTuple(string& str, T& elem)
{
    stringstream ss(str);
    if (isDigit(str))
    {
        ss >> elem;
    }
    else
    {
        cerr << str << " is not a digit" << endl;
    }
} 

void writeToTuple(string& str, string& elem)
{
    elem = str;
}

void writeToTuple(string& str, char& elem)
{
    if (str.size() == 1)
    {
        elem = str[0];
    }
    else
    {
        cerr << str << " is not a char" << endl;
    }
}

void writeToTuple(string& str, bool& elem)
{
    if (str == "true")
    {
        elem = true;
        return;
    }
    if (str == "false")
    {
        elem = false;
        return;
    }
    if (isDigit(str))
    {
        elem = stof(str);
        return;
    }
    cerr << str << " is not a bool" << endl;
}

template <typename Tuple, int index>
typename enable_if<(index == 0), void>::type pack(Tuple& t, vector<string> words) {}

template <typename Tuple, int index>
typename enable_if<(index != 0), void>::type pack(Tuple& t, vector<string> words)
{
    writeToTuple(words[index - 1], get<index - 1>(t));
    pack<Tuple, index - 1>(t, words);
}

template <class... Args>
class CSVParser
{
private:
	vector <tuple<Args...>> data;
    char columnSplitSymbol;
    char lineSplitSymbol;
    char screenSymbol;
public:
	vector<string> getWords(string line)
	{
        vector<string> words;
        int wordStart = 0;
        bool screenFlag = 0;
        for (int i = 0; i < line.size(); ++i)
        {
            if (line[i] == screenSymbol)
            {
                line.erase(line.begin() + i);
                if (line[i] != screenSymbol)
                {
                    screenFlag = !screenFlag;
                    i--;
                }
            }
            else if (line[i] == columnSplitSymbol && !screenFlag)
            {
                words.push_back(line.substr(wordStart, i - wordStart));
                wordStart = i + 1;
            }
        }
        if (wordStart != line.size())
        {
            words.push_back(line.substr(wordStart, line.size()));
        }
        return words;
	}

	CSVParser(ifstream& input, int lineSkip, char css = ',', char lss = '\n', char ss = '\"') : columnSplitSymbol(css), lineSplitSymbol(lss), screenSymbol(ss)
	{
        try
        {
            if (!input.is_open())
            {
                throw invalid_argument("bad file");
            }
            if (columnSplitSymbol == lineSplitSymbol || columnSplitSymbol == screenSymbol || lineSplitSymbol == screenSymbol)
            {
                throw invalid_argument("delimeter and screen symbols must differ");
            }
            string line;
            for (int i = 0; i < lineSkip && !input.eof(); ++i)
            {
                getline(input, line, lineSplitSymbol);
            }
            for (int i = 1; !input.eof(); ++i)
            {
                getline(input, line, lineSplitSymbol);
                if (!line.empty())
                {
                    vector<string> words = getWords(line);
                    try
                    {
                        if (words.size() < sizeof...(Args))
                        {
                            throw make_pair(i, line.size() + 1);
                        }
                        if (words.size() > sizeof...(Args))
                        {
                            cerr << "too many elements at line " << i << endl;
                        }
                        tuple <Args...> t;
                        pack<decltype(t), sizeof...(Args)>(t, words);

                        data.push_back(t);
                    }
                    catch (pair<int, size_t> p)
                    {
                        cerr << "not enought elements (" << p.first << "," <<  p.second << ") " << "skipping line" << endl;
                    }
                }
                else
                {
                    cerr << "empty line " << i << endl;
                }
            }
        }
        catch (invalid_argument ia)
        {
            cerr << ia.what() << endl;
        }
	}
    class Iterator
    {
    private:
        vector <tuple<Args...>>* dataPtr;
        int index;
    public:
        Iterator(int index, vector <tuple<Args...>>* dataPtr) : index(index), dataPtr(dataPtr) {};
        bool operator!=(const Iterator& i) const
        {
            return !(i == *this);
        }
        bool operator==(const Iterator& i) const
        {
            return (i.dataPtr == dataPtr && i.index == index);
        }
        const tuple<Args...>& operator*() const
        {
            return (*dataPtr)[index];
        }
        Iterator& operator++()
        {
            ++index;
            return *this;
        }
    };
    Iterator begin()
    {
        return Iterator(0, &data);
    }
    Iterator end()
    {
        return Iterator(data.size(), &data);
    }
};