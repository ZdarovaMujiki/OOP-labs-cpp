#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <list>
#include <map>

typedef unsigned int uint;

using namespace std;

class IWorker
{
public:
	virtual void task(vector<string> &text) = 0;
	virtual ~IWorker();
};

class Readfile : public IWorker
{
private:
	ifstream input;
public:
	Readfile(string filename);
	void task(vector<string> &text) override;
	~Readfile() override;
};

class Writefile : public IWorker
{
private:
	ofstream output;
public:
	Writefile(string filename);
	void task(vector<string> &text) override;
	~Writefile() override;
};

class Grep : public IWorker
{
private:
	string word;
public:
	Grep(string wrd);
	void task(vector<string> &text) override;
	~Grep() override;
};

class Sort : public IWorker
{
public:
	void task(vector<string> &text) override;
};

class Replace : public IWorker
{
private:
	string wordA, wordB;
public:
	Replace(string a, string b);
	void task(vector<string> &text) override;
	~Replace() override;
};

class Dump : public IWorker
{
private:
	ofstream output;
public:
	Dump(string filename);
	void task(vector<string> &text) override;
	~Dump() override;
};

class Parser
{
public:
	static map<uint, IWorker*> parseBlocks(ifstream& input);
	static list<uint> parseOrder(ifstream& input);
};

class Executor
{
public:
	static void execute(map<uint, IWorker*> blocks, list<uint> order, ifstream& input, ofstream& output);
};

class Validator
{
public:
	static void emptinessCheck(map<uint, IWorker*> blocks, list<uint> order);
	static void inputOutputCheck(map<uint, IWorker*> blocks, list<uint> order, ifstream& input, ofstream& output);
	static void inputOutputRepeatCheck(map<uint, IWorker*> blocks, list<uint> order, ifstream& input, ofstream& output, char* outputName);
};