#include<iostream>
#include<fstream>
#include<cstdlib>
#include<sstream>
#include<vector>
using namespace std;
ifstream fin;
ofstream fout;
istringstream mult;
ostringstream proc;

bool _isNum(string s) {//detect if the string is a number
	stringstream _test(s);
	double d;
	char c;
	if (!(_test >> d)) {
		//extract number failure
		return false;
	}
	if (_test >> c) {
		//error input
		return false;
	}
	return true;
}

int width_change(string fn, double w, string _outf, double st, double ed) {
	/* The function that changes the width of a chart.
	*  fn:filename, w:width multiplier, _outf: output filename, st:start time, ed:end time
	*/
	string buf;
	fin.open(fn);//open file
	double nw = 0, np = 0, t = 0;
	bool ch_trigger = false;//check if note is in the specified time range
	if (fin.fail()) {
		return 1;
	}
	else {
		fout.open(_outf);
		if (!fout.fail()) {
			while (!fin.eof()) {
				getline(fin, buf);
				if (buf.find("<m_time>", 0) != buf.npos) {
					fout << buf << endl;
					int endp = buf.find("</m_time>", 0);
					buf = buf.substr(8, endp - 8);
					mult.str(buf);
					proc.str("");
					mult >> t;//read time
					mult.clear();
					if (t >= st && t <= ed) {
						ch_trigger = true;
					}
				}
				else if (buf.find("<m_width>", 0) != buf.npos) {
					if (ch_trigger) {
						int endp = buf.find("</m_width>", 0);
						buf = buf.substr(9, endp - 9);
						mult.str(buf);
						proc.str("");
						mult >> nw;
						mult.clear();
						//cout << nw << endl;
						np += nw * 0.5;
						nw *= w;
						np -= nw * 0.5;
						proc << "<m_position>" << np << "</m_position>" << endl;
						proc << "<m_width>" << nw << "</m_width>";
						buf = proc.str();
						ch_trigger = false;
						fout << buf << endl;
					}
					else {
						fout << buf << endl;
					}
				}
				else if (buf.find("<m_position>", 0) != buf.npos) {
					if (ch_trigger) {
						int endp = buf.find("</m_position>", 0);
						buf = buf.substr(12, endp - 12);
						//cout << buf << " " << endp << endl;
						mult.str(buf);
						proc.str("");
						mult >> np;
						mult.clear();
					}
					else {
						fout << buf << endl;
					}
				}
				else {
					fout << buf << endl;
				}
			}
			fin.close();
			fout.close();
			return 0;
		}
		else {
			fin.close();
			return 2;
		}
	}
}