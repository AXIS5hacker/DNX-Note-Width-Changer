#include<iostream>
#include<fstream>
#include<cstdlib>
#include<sstream>

#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
#include<direct.h>
#else
#include<unistd.h>
#endif

#include<vector>
#include<cstdio>
#include <io.h>
using namespace std;
extern int width_change(string fn, double w, string _outf);
int main(int argc, char* argv[])
{
	cout << "Dynamix Chart Width Changer v0.2" << endl;
	cout << "Created by AXIS5" << endl << endl << endl;
	char pbuf[_MAX_PATH];
#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
	_getcwd(pbuf, _MAX_PATH);
#else
	getcwd(pbuf, _MAX_PATH);
#endif
	string filename, _output = ""; 

#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
	string cwd = string(pbuf) + '\\';
#else
	string cwd = string(pbuf) + '/';
#endif

	istringstream is;
	is.str("");
	double width = 1;
	//triggers
	bool help_only = false;//if argument -?
	bool false_usage = false;
	bool specified_width = true;
	bool next_width = false;
	bool next_filename = false;//if specifying filename
	bool default_filename = true;//if filename not specified
	//trigger end
	vector<string> arglist;
	for (int i = 0; i < argc; i++) {
		arglist.push_back(argv[i]);
		if (arglist[i] == "-?")help_only = true;
		else if (next_width) {//if next argument specifies the width
			is.str(arglist[i]);
			is >> width;//convert to number
			//cout << width << endl;
			next_width = false;
		}
		else if (next_filename) {//if next argument specifies the output filename
			_output = arglist[i];
			//cout << width << endl;
			next_filename = false;
			default_filename = false;
		}
		else if (arglist[i] == "-w") {//width specify trigger
			next_width = true;
		}
		else if (arglist[i] == "-o") {//output name specify trigger
			next_filename = true;
		}
		else if (argc == 1 || argc > 6) {//not specified arguments or too many arguments
			false_usage = true;
		}
	}
	if (help_only) {
		cout << "usage:" << endl;
		cout << "filename [-w width_multiplier] [-o output_filename] [-?]" << endl << endl;
		cout << "-w width_multiplier\tchange the width of a chart,width_multiplier is a decimal number" << endl;
		cout << "-o output_filename\tspecify the filename of the changed chart" << endl;
		cout << "-?\thelp" << endl;
	}
	else if (next_width) {
		cout << "please specify a width multiplier" << endl;
	}
	else if (false_usage) {
		cout << "invalid arguments" << endl;
		cout << "usage:" << endl;
		cout << "filename [-w width_multiplier] [-o output_filename] [-?]" << endl << endl;
		cout << "-w width_multiplier\tchange the width of a chart,width_multiplier is a decimal number" << endl;
		cout << "-o output_filename\tspecify the filename of the changed chart" << endl;
		cout << "-?\thelp" << endl;
	}
	else {
		filename = arglist[1];
		if (default_filename) {
			_output = filename.substr(0, filename.length() - 4) + "_out.xml";

		}
		int success = width_change(filename, width, _output);//width=1 as default width multiplier
		if (success == 1)cout << "Cannot open file." << endl;//file not found or do not have access
		else if (success == 2)cout << "Cannot save changed chart file." << endl;//invalid output chart name
		else {
			cwd = cwd + _output;
			//cout << cwd << endl;
			if (_access(cwd.c_str(), 0) == 0) {
				cout << "Changed chart saved as \"" << cwd << "\"" << endl;
			}
			else {
				cout << "Changed chart saved as \"" << _output << "\"" << endl;
			}
		}
	}
	system("pause");
	return 0;
}