//thie program is going to parser .lis file to .csv file

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

void read_lis_file(char*);

string lis_filename;

int main(int argc,char **argv)
{
	read_lis_file(argv[argc-1]);
	//cout<<"Hello World!\n";
	return 0;
}

void read_lis_file(char* filename)
{
	ifstream lis_file;
	string operate1;
	lis_file.open(filename, ios::binary | ios::in);

	if(!lis_file.is_open())
	{
		cerr<<"Error!! Can't open pattern file: ";
		cerr<<filename;
		cerr<<" !!!"<<endl;
		return;
	}
	else
	cout<<"file open successfully!!!"<<endl;

	while(!lis_file.eof())
	{
		lis_file>>operate1;
		if(lis_file.get()!='\n')
		{
			cout<<operate1<<" ";
		}
		else
		{
			cout<<operate1<<"\n";
		}
	}
}
