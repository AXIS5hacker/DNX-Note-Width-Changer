/*
* Creator:AXIS5
*/
#include"width_change.h"

using namespace std;
int width_change(string fn, double w, string _outf, double st, double ed);
extern bool _isNum(string s);
struct stat st;

int main(int argc, char* argv[])
{
	cout << "Dynamix Chart Width Changer v0.7.6" << endl;
	cout << "Created by AXIS5" << endl;
	cout << "Special thanks: i0ntempest" << endl << endl << endl;
	char pbuf[260];
#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
	_getcwd(pbuf, 260);
#else
	getcwd(pbuf, 260);
#endif
	string filename, _output = "";

#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
	string cwd = string(pbuf) + '\\';
#else
	string cwd = string(pbuf) + '/';
#endif
	//cout << cwd << endl;
	istringstream is;
	is.str("");
	double width = 1;
	double start_time = -1e10;
	double end_time = 1e10;
	//triggers
	bool help_only = false;//if argument -?
	bool false_usage = false;
	bool specified_width = true;
	bool next_width = false;
	bool next_filename = false;//if specifying filename
	bool default_filename = true;//if filename not specified

	bool def_stimestamp = true;//using default start time
	bool def_etimestamp = true;//using default end time

	bool next_stimestamp = false;//if specifying start time
	bool next_etimestamp = false;//if specifying end time
	//trigger end
	vector<string> arglist;
	for (int i = 0; i < argc; i++) {
		arglist.push_back(argv[i]);
		if (arglist[i] == "-?" || arglist[i] == "-h")help_only = true;
		else if (next_width) {//if next argument specifies the width
			if (!_isNum(arglist[i])) {
				cout << "Invalid width multiplier" << endl;
#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
				system("pause");
#endif
				return 0;
			}
			is.str(arglist[i]);
			is >> width;//convert to number
			is.clear();
			//cout << width << endl;
			next_width = false;
		}
		else if (next_stimestamp || next_etimestamp) {//if next argument specifies the time range
			if (!_isNum(arglist[i])) {
				cout << "Invalid time" << endl;
#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
				system("pause");
#endif
				return 0;
			}
			is.str(arglist[i]);
			if (next_stimestamp) {
				is >> start_time;//convert to number
				//cout << start_time << endl;
				is.clear();
				next_stimestamp = false;
				def_stimestamp = false;
			}
			else if (next_etimestamp) {
				is >> end_time;//convert to number
				//cout << width << endl;
				is.clear();
				next_etimestamp = false;
				def_etimestamp = false;
			}
			if (start_time > end_time) {
				cout << "Invalid time range" << endl;
#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
				system("pause");
#endif
				return 0;
			}
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
		else if (arglist[i] == "-s") {//start time specify trigger
			next_stimestamp = true;
		}
		else if (arglist[i] == "-e") {//end time specify trigger
			next_etimestamp = true;
		}
		else if (argc == 1 || argc > 10) {//not specified arguments or too many arguments
			false_usage = true;
		}
	}
	if (help_only) {
		cout << "usage:" << endl;
		cout << "filename [-w width_multiplier] [-o output_filename] [-s start_time(bar)] [-e end_time(bar)] [-?|-h]" << endl << endl;
		cout << "-w width_multiplier\tchange the width of a chart, width_multiplier is a decimal number" << endl;
		cout << "-o output_filename\tspecify the filename of the changed chart" << endl;
		cout << "-s start_time(bar)\tspecify the start time of the time range you want to change, in the unit of bar." << endl;
		cout << "-e end_time(bar)\tspecify the end time of the time range you want to change, in the unit of bar." << endl;
		cout << "-?\thelp" << endl;
		cout << "-h\thelp, same as -?" << endl;
	}
	else if (next_width) {
		cout << "please specify a width multiplier" << endl;
	}
	else if (false_usage) {
		cout << "invalid arguments" << endl;
		cout << "usage:" << endl;
		cout << "filename [-w width_multiplier] [-o output_filename] [-s start_time(bar)] [-e end_time(bar)] [-?|-h]" << endl << endl;
		cout << "-w width_multiplier\tchange the width of a chart, width_multiplier is a decimal number" << endl;
		cout << "-o output_filename\tspecify the filename of the changed chart" << endl;
		cout << "-s start_time(bar)\tspecify the start time of the time range you want to change, in the unit of bar." << endl;
		cout << "-e end_time(bar)\tspecify the end time of the time range you want to change, in the unit of bar." << endl;
		cout << "-?\thelp" << endl;
		cout << "-h\thelp, same as -?" << endl;
	}
	else {
		filename = arglist[1];
		if (default_filename) {
			_output = filename.substr(0, filename.length() - 4) + "_out.xml";

		}
		else {
			if (_output[_output.length() - 1] == '\\' || _output[_output.length() - 1] == '/') {//if directory
				_output += "out.xml";
			}
			else if (_output.length() < 4 || _output.substr(_output.length() - 4, 4) != ".xml") { //force xml format
				if (stat(_output.c_str(), &st) == 0) {
					if (st.st_mode & S_IFDIR) {//if a directory exists
#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
						_output += "\\out.xml";
#else
						_output += "/out.xml";
#endif
					}
					else {
						_output += ".xml";
					}
				}
				else {
					_output += ".xml";
				}
			}
		}
		//cout << width << endl;
		int success = width_change(filename, width, _output, start_time, end_time);//width=1 as default width multiplier
		if (success == 1)cout << "Cannot open file." << endl;//file not found or do not have access
		else if (success == 2)cout << "Cannot save changed chart file." << endl;//invalid output chart name
		else {
			cwd = cwd + _output;
			//cout << cwd << endl;
			if (def_stimestamp && def_etimestamp) {
				cout << "Changed entire chart." << endl;
			}
			else if (def_stimestamp) {
				cout << "Changed width from start to " << end_time << " bar." << endl;
			}
			else if (def_etimestamp) {
				cout << "Changed width from " << start_time << " bar to end." << endl;
			}
			else {
				cout << "Changed width from " << start_time << " bar to " << end_time << " bar." << endl;
			}
#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
			if (_access(cwd.c_str(), 0) == 0)
#else
			if (access(cwd.c_str(), 0) == 0)
#endif
			{
				cout << "Changed chart saved as \"" << cwd << "\"" << endl;
			}
			else {
				cout << "Changed chart saved as \"" << _output << "\"" << endl;
			}
		}
	}

#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
	system("pause");
#endif

	return 0;
}