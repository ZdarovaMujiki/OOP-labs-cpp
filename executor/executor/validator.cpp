#include "modules.h"

void Validator::emptinessCheck(map<uint, IWorker*> blocks, list<uint> order)
{
	try
	{
		if (blocks.empty())
		{
			throw invalid_argument("no blocks in workflow");
		}
		if (order.empty())
		{
			throw invalid_argument("order is empty");
		}
	}
	catch (invalid_argument ia)
	{
		cerr << ia.what() << endl << "shutting down" << endl;
		exit(-1);
	}
}

void Validator::inputOutputCheck(map<uint, IWorker*> blocks, list<uint> order, ifstream& input, ofstream& output)
{
	try
	{
		if (!dynamic_cast<Readfile*>(blocks[order.front()]) && !input.is_open())
		{
			throw invalid_argument("no input");
		}
		if (!dynamic_cast<Writefile*>(blocks[order.back()]) && !output.is_open())
		{
			throw invalid_argument("no output");

		}
	}
	catch (invalid_argument ia)
	{
		cerr << ia.what() << endl << "shutting down" << endl;
		exit(-1);
	}
}

void Validator::inputOutputRepeatCheck(map<uint, IWorker*> blocks, list<uint> order, ifstream& input, ofstream& output, char* outputName)
{
	try
	{
		if (dynamic_cast<Readfile*>(blocks[order.front()]) && input.is_open())
		{
			throw invalid_argument("input file already exist");
		}
	}
	catch (invalid_argument ia)
	{
		cerr << ia.what() << endl << "only workflow input saved" << endl;
		input.close();
	}

	try
	{
		if (dynamic_cast<Writefile*>(blocks[order.back()]) && output.is_open())
		{
			throw invalid_argument("output file already exist");
		}
	}
	catch (invalid_argument ia)
	{
		cerr << ia.what() << endl << "only workflow output saved" << endl;
		output.close();
		remove(outputName);
	}
}