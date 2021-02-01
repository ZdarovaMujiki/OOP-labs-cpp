#include "modules.h"

using namespace std;

IWorker::~IWorker() {};

void Readfile::task(vector<string> &text)
{
	string line;
	if (input.is_open())
	{
		while (!input.eof())
		{
			getline(input, line);
			text.push_back(line);
		}
	}
	else
	{
		cerr << "bad input file" << endl << "shutting down" << endl;
		exit(-1);
	}
}

void Writefile::task(vector<string> &text)
{
	for (string line : text)
	{
		output << line << endl;
	}
}

void Grep::task(vector<string> &text)
{
	vector<string> newText;
	regex rx("(\W*)(" + word + ")(\W*)", regex_constants::icase);
	for (string line : text)
	{
		if (regex_search(line, rx))
		{
			newText.push_back(line);
		}
	}
	text = newText;
}

void Sort::task(vector<string> &text)
{
	sort(text.begin(), text.end());
}

void Replace::task(vector<string> &text)
{
	regex rx(wordA);
	for (string &line : text)
	{
		line = regex_replace(line, rx, wordB);
	}
}
	
void Dump::task(vector<string> &text)
{
	for (string line : text)
	{
		output << line << endl;
	}
}

Readfile::Readfile(string filename)
{
	input.open(filename);
}

Writefile::Writefile(string filename)
{
	output.open(filename);
}

Grep::Grep(string wrd) : word(wrd) {};

Replace::Replace(string a, string b) : wordA(a), wordB(b) {};

Dump::Dump(string filename)
{
	output.open(filename);
}

Readfile::~Readfile()
{
	input.close();
}

Writefile::~Writefile()
{
	output.close();
}

Grep::~Grep()
{
	word.clear();
}

Replace::~Replace()
{
	wordA.clear();
	wordB.clear();
}

Dump::~Dump()
{
	output.close();
}