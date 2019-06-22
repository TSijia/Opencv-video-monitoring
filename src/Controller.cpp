
#include"Controller.h"

void controller::controltask()
{
	string cmd;
	vector<string> cmdSplit;
	vector<string>::iterator it;
	while (1)
	{
		getline(cin, cmd);

		split(cmdSplit, cmd, is_any_of(" "), token_compress_on);

		if (cmdSplit[0] == "quit")
			break;

		it = find(cmdSplit.begin(), cmdSplit.end(), "threadID");

		if (it != cmdSplit.end())
		{
			int theadnum = atoi(cmdSplit[distance(cmdSplit.begin(), it) + 1].c_str());//第几个线程
			if (theadnum > 5)
			{
				ParameterTable.insert(pair<int, vector<CommandAndVar>>(theadnum, commandvar));
				threadoperator.insert(pair<int, int>(theadnum, 0));
			}
			ParameterTable[theadnum][0].var = cmdSplit[distance(cmdSplit.begin(), it) + 1];
			int j = 2;
			while (j < cmdSplit.size())//输入的字符串
			{
				for (int i = 1; i < commandvar.size(); i++)//现有的
				{
					if (cmdSplit[j] == ParameterTable[theadnum][i].command)
					{
						ParameterTable[theadnum][i].var = cmdSplit[++j];
					}
				}
				j++;
			}
			if (ParameterTable[theadnum][1].var == "open")
			{
				if (threadoperator[theadnum] == 0)
				{
					start(theadnum);
					threadoperator[theadnum] = 1;
				}
				else
					threadoperator[theadnum] = 2;
			}
			else if (ParameterTable[theadnum][1].var == "close")
			{
				if (threadoperator[theadnum] == 1 || (threadoperator[theadnum] == 2))
				{
					stop(theadnum);
					threadoperator[theadnum] = 0;
				}

				else
					cout << "please insure your operator" << endl;
			}
			else
			{
				cout << "please input your operator" << endl;
			}
		}
		else
		{
			cout << "please check your input, it must contain 'threadID'" << endl;
		}


	}

}

void controller::start(int threadID1)
{
	worker workers;
	int threadid = threadID1;
	workthread[threadid] = thread(&worker::detect_function, &workers, threadid);		
	cout << "NO." << threadid << " thread start " << endl;
}
void controller::stop(int threadID2)
{
	int threadid = threadID2;
	workthread[threadid].interrupt();
}

void controller::ComPrompt()
{
	cout << "the parameters you can set : " << endl;
	cout << "--------------------------------" << endl;
	cout <<"  COMMAND"<< "\t" <<"| " << "DEFAULT VALUE "<< "\t" << endl;
	cout << "--------------------------------" << endl;
	cout <<"  "<< commandvar[0].command << "\t" <<"| "<< commandvar[0].var << "\t"  << endl;
	cout << "--------------------------------" << endl;
	cout <<"  "<< commandvar[1].command <<"\t" << "| "<< commandvar[1].var <<"\t"  << endl;
	cout << "--------------------------------" << endl;
	cout <<"  "<< commandvar[2].command <<"\t"<<"| "<< commandvar[2].var << "\t" << endl;
	cout << "--------------------------------" << endl;
	cout <<"  "<< commandvar[3].command << "\t" << "| "<< commandvar[3].var << "\t"  << endl;
	cout << "--------------------------------" << endl;
	cout <<"  "<< commandvar[4].command << "\t" << "| "<< commandvar[4].var << "\t"  << endl;
	cout << "--------------------------------" << endl;
	cout <<"  "<< commandvar[5].command << "\t" << "| "<< commandvar[5].var << "\t"  << endl;
	cout << "--------------------------------" << endl;
	cout <<"  "<< commandvar[6].command << "\t" << "| "<< commandvar[6].var << "\t"  << endl;
	cout << "--------------------------------" << endl;

	cout << "the parameters you have to set is 'threadID'" << endl;
	cout << "other parameters can use default values if you don't want to change them" << endl;
	cout << "eg: threadID 0 threadOP open" << endl;
	cout << "you can change the value of the parameter at any time" << endl;
	cout << "you can colse thread x by 'threadID x threadOP close' or exit the program by 'quit'" << endl;
}


