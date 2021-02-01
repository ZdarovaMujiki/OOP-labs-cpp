#include "TupleRead.h"
#include "CSVParser.h"
#include <iostream>
#include <tuple>

int main()
{
	ifstream file("tests/ba.txt");
	//CSVParser<int, float, bool> parser(file, 0);					//(id, ed) 1, 2
	//CSVParser<bool, int, float, char, string> parser(file, 0);	//(ba) 3, 4, 5
	//CSVParser<int, string> parser(file, 0, '.', ';');				//(gp) 6
	//CSVParser<int, string> parser(file, 0, '.', ';', '.');			//(--) 7
	//CSVParser<bool, int, char> parser(file, 0);					//(tc) 8
	//CSVParser<int, string> parser(file, 2, '.', ';');				//(ls) 9, 10
	//CSVParser<int, string> parser(file, 0, ',', '\n', '*');		//(s), 11

	cout << endl;
	for (auto& rs : parser)
	{
		cout << rs << endl;
	}

	return 0;
}