#include "modules.h"

void Executor::execute(map<uint, IWorker*> blocks, list<uint> order, ifstream& input, ofstream& output)
{
	vector<string> text;
	int i = -1;
	try
	{
		if (input.is_open())
		{
			string line;
			while (!input.eof())
			{
				getline(input, line);
				text.push_back(line);
			}
		}
		for (int id : order)
		{
			++i;
			try
			{
				if (dynamic_cast<Readfile*>(blocks[id]) && i != 0 || dynamic_cast<Writefile*>(blocks[id]) && i != (order.size() - 1))
				{
					throw invalid_argument("writing / reading is not allowed in the middle of workflow");
				}
				if (!blocks[id])
				{
					throw invalid_argument("no block #" + to_string(id));
				}
				blocks[id]->task(text);
			}
			catch (invalid_argument ia)
			{
				cerr << ia.what() << endl << "skipping step" << endl;
			}
		}
		if (output.is_open())
		{
			for (string line : text)
			{
				output << line << endl;
			}
		}
	}
	catch (invalid_argument ia)
	{
		cerr << ia.what() << endl;
	}
}