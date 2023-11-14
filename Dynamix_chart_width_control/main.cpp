/**
* Creator:AXIS5
* This project is compiled in C++14 standard
*/
#include"defs.h"
#include"chart_store.h"
#include"../version.h"
#include<iostream>
using namespace std;

extern bool _isNum(string s);
struct stat st;

int main(int argc, char* argv[])
{
	cout << "Dynamix Chart Width Changer " << VERSION_H << endl;
	cout << "Created by AXIS5" << endl;
	cout << "Special thanks: i0ntempest, NordLandeW" << endl << endl << endl;
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

	int random_trigger = 0;
	//if activated random width mode,0:not activated,1:random mode 1,2:random mode 2

	//side triggers
	bool all_change = true;//if not specifying sides for changing
	int side_mask = 0x7;
	//default:111(all sides),0 for not change, 1 for change
	//first digit is middle side, second digit is left, third digit is right

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
		else if (arglist[i] == "-l") {//left side specify trigger
			if (all_change) {
				all_change = false;
				side_mask = 0;
			}
			side_mask = side_mask | LEFT_CHANGE;
		}
		else if (arglist[i] == "-r") {//right side specify trigger
			if (all_change) {
				all_change = false;
				side_mask = 0;
			}
			side_mask = side_mask | RIGHT_CHANGE;
		}
		else if (arglist[i] == "-m") {//middle side specify trigger
			if (all_change) {
				all_change = false;
				side_mask = 0;
			}
			side_mask = side_mask | MID_CHANGE;
		}
		else if (arglist[i] == "-rnd1") {
			random_trigger = 1;
		}
		else if (arglist[i] == "-rnd2") {
			random_trigger = 2;
		}
		else if (argc == 1 || argc > 14) {//not specified arguments or too many arguments
			false_usage = true;
		}
		else if (i > 1) {//wrong argument detected
			false_usage = true;
		}
	}
	if (help_only) {
		cout << "usage:" << endl;
		cout << "filename [-w width_multiplier|-rnd1|-rnd2] [-o output_filename] [-s start_time(bar)] [-e end_time(bar)] [-?|-h] [-m] [-l] [-r]" << endl << endl;
		cout << "-w width_multiplier\tchange the width of a chart, width_multiplier is a decimal number" << endl;
		cout << "-o output_filename\tspecify the filename of the changed chart" << endl;
		cout << "-s start_time(bar)\tspecify the start time of the time range you want to change, in the unit of bar." << endl;
		cout << "-e end_time(bar)\tspecify the end time of the time range you want to change, in the unit of bar." << endl;
		cout << "-m\tchange the middle side." << endl;
		cout << "-l\tchange the left side." << endl;
		cout << "-r\tchange the right side." << endl;
		cout << "*If none of the triggers in \"-m, -l, -r\" is specified, all sides will be changed." << endl;
		cout << "-rnd1\tchange the note width randomly, using random mode 1(will ignore the \"-w\" argument)" << endl;
		cout << "-rnd2\tchange the note width randomly, using random mode 2(will ignore the \"-w\" argument)" << endl;
		cout << "-?\thelp" << endl;
		cout << "-h\thelp, same as -?" << endl;
	}
	else if (next_width) {
		cout << "please specify a width multiplier" << endl;
	}
	else if (false_usage) {
		cout << "invalid arguments" << endl;
		cout << "usage:" << endl;
		cout << "filename [-w width_multiplier|-rnd1|-rnd2] [-o output_filename] [-s start_time(bar)] [-e end_time(bar)] [-?|-h] [-m] [-l] [-r]" << endl << endl;
		cout << "-w width_multiplier\tchange the width of a chart, width_multiplier is a decimal number" << endl;
		cout << "-o output_filename\tspecify the filename of the changed chart" << endl;
		cout << "-s start_time(bar)\tspecify the start time of the time range you want to change, in the unit of bar." << endl;
		cout << "-e end_time(bar)\tspecify the end time of the time range you want to change, in the unit of bar." << endl;
		cout << "-m\tchange the middle side." << endl;
		cout << "-l\tchange the left side." << endl;
		cout << "-r\tchange the right side." << endl;
		cout << "*If none of the triggers in \"-m, -l, -r\" is specified, all sides will be changed." << endl;
		cout << "-rnd1\tchange the note width randomly, using random mode 1(will ignore the \"-w\" argument)" << endl;
		cout << "-rnd2\tchange the note width randomly, using random mode 2(will ignore the \"-w\" argument)" << endl;
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

		//create chart store class
		chart_store cs;//store the chart
		try {
			int fail_read = cs.readfile(filename);//open file
			//fixing stage
			//missing barpm
			if (fail_read & BARPM_MISSING) {
				double new_barpm = 0;
				string ll;
				istringstream istr;
				cout << "Illegal Barpm! Please enter a valid Barpm:" << endl;;
				getline(cin, ll);
				istr.str(ll);
				istr >> new_barpm;

				while (new_barpm <= 0) {
					cout << "Illegal Barpm! Please re-enter a valid Barpm:" << endl;
					getline(cin, ll);
					istr.clear();
					istr.str(ll);
					istr >> new_barpm;
				}
				char new_barpm_string[64];
				snprintf(new_barpm_string, sizeof(new_barpm_string), "%f", new_barpm);
				cs.set_barpm(new_barpm);
				cout << "Barpm is set to " + string(new_barpm_string) + "." << endl;
				cout << "===============================" << endl;

				fail_read &= (~(int)BARPM_MISSING);
			}
			//missing left side
			if (fail_read & LEFT_SIDE_MISSING) {
				sides s = sides::UNKNOWN;
				string side_string;
				cout << "Left side type is not specified! Please enter a valid side type (pad, mixer, multi):" << endl;
				while (s == sides::UNKNOWN) {
					cin >> side_string;
					//lowercase
					for (auto& ch : side_string) {
						ch = tolower(ch);
					}

					if (side_string == "pad") {
						s = sides::PAD;
					}
					else if (side_string == "mixer") {
						s = sides::MIXER;
					}
					else if (side_string == "multi") {
						s = sides::MULTI;
					}
					else {
						cout << "Invalid side type! Please enter again! (pad, mixer, multi)" << endl;
					}
				}
				cs.set_lside(s);
				cout << "Left side fixed." << endl;
				cout << "===============================" << endl;
				fail_read &= (~(int)LEFT_SIDE_MISSING);
			}
			//missing right side
			if (fail_read & RIGHT_SIDE_MISSING) {
				sides s = sides::UNKNOWN;
				string side_string;
				cout << "Right side type is not specified! Please enter a valid side type (pad, mixer, multi):" << endl;
				while (s == sides::UNKNOWN) {
					cin >> side_string;
					//lowercase
					for (auto& ch : side_string) {
						ch = tolower(ch);
					}
					if (side_string == "pad") {
						s = sides::PAD;
					}
					else if (side_string == "mixer") {
						s = sides::MIXER;
					}
					else if (side_string == "multi") {
						s = sides::MULTI;
					}
					else {
						cout << "Invalid side type! Please enter again! (pad, mixer, multi)" << endl;
					}
				}
				cs.set_rside(s);
				cout << "Right side fixed." << endl;
				cout << "===============================" << endl;
				fail_read &= (~(int)RIGHT_SIDE_MISSING);
			}
			//Hold-sub mismatch autofix
			if (fail_read & HOLD_SUB_MISMATCH) {
				cout << "mismatched notes found:" << endl;
				for (auto& ii : cs.mismatched_notes) {
					cout << ii.first << '\t' << ii.second << endl;
					//fix
					if (ii.second == "middle") {
						cs.m_notes.erase(ii.first);
					}
					else if (ii.second == "left") {
						cs.m_left.erase(ii.first);
					}
					else if (ii.second == "right") {
						cs.m_right.erase(ii.first);
					}
				}
				cout << "\nThe mismatching Holds and Subs have been fixed automatically." << endl;
				cout << "===============================" << endl;
				//set status to success
				fail_read &= (~(int)HOLD_SUB_MISMATCH);
			}

			if (fail_read == 0) {

				//this function now only processes chart store class
				int success = width_change(cs, width, start_time, end_time, side_mask, random_trigger);//width=1 as default width multiplier

				//hold-sub mismatch
				if (success == 2) {
					cout << "Cannot save changed chart file." << endl;//hold-sub mismatch
				}
				//save
				else if (!cs.to_file(_output)) {
					cout << "Cannot save changed chart file." << endl;//invalid output chart name
				}
				else {
					cout << "Changed sides: ";
					if (side_mask & MID_CHANGE) {
						cout << "middle ";
					}
					if (side_mask & LEFT_CHANGE) {
						cout << "left ";
					}
					if (side_mask & RIGHT_CHANGE) {
						cout << "right ";
					}
					cout << endl;
					cwd = cwd + _output;
					//cout << cwd << endl;
					if (random_trigger == 1) {
						cout << "Note width has been randomized, using randomize mode 1" << endl;
					}
					else if (random_trigger == 2) {
						cout << "Note width has been randomized, using randomize mode 2" << endl;
					}
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
			else {
				cout << "Unknown error" << endl;
			}
	}
		catch (exception& ex) {
			cout << ex.what() << endl;
		}
}
#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
	system("pause");
#endif

	return 0;
}