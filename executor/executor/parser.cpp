#include "modules.h"

map<uint, IWorker*> Parser::parseBlocks(ifstream& input)
{
	map<uint, IWorker*> blocks;
	string tmp1, tmp2, blockName;
	list <string> lines;
	uint id;

	try
	{
		getline(input, tmp1); //tmp 1 = desc
		if (tmp1 != "desc")
		{
			throw invalid_argument("\"desc\" needs to be the first line of workflow");
		}
		while (!input.eof() && tmp1 != "csed")
		{
			getline(input, tmp1);
			lines.push_back(tmp1);
		}
		if (tmp1 != "csed")
		{
			throw invalid_argument("no \"csed\" in workflow");
		}
	}
	catch (invalid_argument ia)
	{
		cerr << ia.what() << endl << "shutting down" << endl;
		exit(-1);
	}

	for (string line : lines)
	{
		istringstream inString(line);
		try
		{
			if (line == "csed")
			{
				break;
			}
			else
			{
				inString >> id;
				if (blocks.count(id))
				{
					throw invalid_argument("block #" + to_string(id) + " already exist");
				}
				inString >> tmp2; // tmp2 = "="
				if (tmp2 != "=")
				{
					throw invalid_argument("bad line: \"" + line + "\"\ntry: \"id = blockname <arg1> <arg2>\"");
				}
				inString >> blockName;

				tmp1.clear();
				tmp2.clear();
				if (blockName == "readfile")
				{
					inString >> tmp1;
					if (tmp1 == "")
					{
						throw invalid_argument("block #" + to_string(id) + " " + blockName + " - missing arguments");
					}
					blocks[id] = new Readfile(tmp1);
				}
				else if (blockName == "writefile")
				{
					inString >> tmp1;
					if (tmp1 == "")
					{
						throw invalid_argument("block #" + to_string(id) + " " + blockName + " - missing arguments");
					}
					blocks[id] = new Writefile(tmp1);
				}
				else if (blockName == "grep")
				{
					inString >> tmp1;
					if (tmp1 == "")
					{
						throw invalid_argument("block #" + to_string(id) + " " + blockName + " - missing arguments");
					}
					blocks[id] = new Grep(tmp1);
				}
				else if (blockName == "sort")
				{
					blocks[id] = new Sort();
				}
				else if (blockName == "replace")
				{
					inString >> tmp1 >> tmp2;
					if (tmp1 == "" || tmp2 == "")
					{
						throw invalid_argument("block #" + to_string(id) + " " + blockName + " - missing arguments");
					}
					blocks[id] = new Replace(tmp1, tmp2);
				}
				else if (blockName == "dump")
				{
					inString >> tmp1;
					if (tmp1 == "")
					{
						throw invalid_argument("block #" + to_string(id) + " " + blockName + " - missing arguments");
					}
					blocks[id] = new Dump(tmp1);
				}
				else
				{
					throw invalid_argument("bad block name: " + blockName);
				}
			}
		}
		catch (invalid_argument ia)
		{
			cerr << ia.what() << endl << "skipping line" << endl;
		}
	}
	return blocks;
}

list<uint> Parser::parseOrder(ifstream& input)
{
	list<uint> order;
	string tmp;
	uint id;

	while (!input.eof())
	{
		input >> id >> tmp;
		order.push_back(id);
	}
	return order;
}