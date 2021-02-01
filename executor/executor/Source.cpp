#include "modules.h"

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	map<uint, IWorker*> blocks;
	vector<string> text;
	list<uint> order;
	ifstream workflow;
	ifstream input;
	ofstream output;
	char* outputName = 0;

	try
	{
		if (argc < 2)
		{
			throw invalid_argument("missing workflow file");
		}
		workflow.open(argv[1]);
		if (!workflow.is_open())
		{
			throw invalid_argument("bad workflow file");
		}

		if (argc > 2)
		{
			for (int i = 2; i < argc; i += 2)
			{
				if ((string)argv[i] == "-i" && !input.is_open() && (string)argv[i + 1] != (string)argv[1])
				{
					input.open(argv[i + 1]);
					if (!input.is_open())
					{
						throw invalid_argument("bad input file");
					}
				}
				else if ((string)argv[i] == "-o" && !output.is_open() && (string)argv[i + 1] != (string)argv[1])
				{
					output.open(argv[i + 1]);
					outputName = argv[i + 1];
				}
				else
				{
					throw invalid_argument("bad args");
				}
			}
		}
	}
	catch (invalid_argument a)
	{
		cerr << a.what() << endl << "shutting down" << endl;
		exit(-1);
	}

	blocks = Parser::parseBlocks(workflow);
	order = Parser::parseOrder(workflow);

	Validator::emptinessCheck(blocks, order);
	Validator::inputOutputCheck(blocks, order, input, output);
	Validator::inputOutputRepeatCheck(blocks, order, input, output, outputName);

	Executor::execute(blocks, order, input, output);

	cerr << "executor completed the job successfully" << endl;
	return 0;
}