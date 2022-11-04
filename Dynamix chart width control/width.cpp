#include<iostream>
#include<fstream>
#include<cstdlib>
#include<sstream>
#include<vector>
#include<map>
using namespace std;
ifstream fin;
ofstream fout;
istringstream mult;
ostringstream proc;

map<int, double> subs;//used for processing hold notes
map<int, double>::iterator it;

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

	int temp_id = 0;//temporarily saves the id of a note
	int temp_sub = 0;//temporarily saves the id of a sub note

	fin.open(fn);//open file
	double nw = 0, np = 0, t = 0;
	bool ch_trigger = false;//check if note is in the specified time range
	bool hold_trigger = false;//check if current note is a hold
	bool sub_trigger = false;//check if current note is a sub
	if (fin.fail()) {
		return 1;
	}
	else {
		fout.open(_outf);
		if (!fout.fail()) {
			while (!fin.eof()) {
				getline(fin, buf);
				if (buf.find("<m_id>", 0) != buf.npos) {
					fout << buf << endl;
					int endp = buf.find("</m_id>", 0);
					buf = buf.substr(6, endp - 6);
					mult.str(buf);
					proc.str("");
					mult >> temp_id;//read current note id
					mult.clear();
				}
				else if (buf.find("<m_type>", 0) != buf.npos) {
					fout << buf << endl;
					int endp = buf.find("</m_type>", 0);
					buf = buf.substr(8, endp - 8);
					if (buf == "HOLD")
					{
						hold_trigger = true;
						sub_trigger = false;
					}
					else if (buf == "SUB") {//change policy for sub notes
						hold_trigger = false;
						sub_trigger = true;
						it = subs.find(temp_id);
						if ((*it).second >= st && (*it).second <= ed) {
							ch_trigger = true;
						}
						subs.erase(it);
					}
					else {
						hold_trigger = false;
						sub_trigger = false;
					}
				}
				else if (buf.find("<m_time>", 0) != buf.npos) {
					fout << buf << endl;
					if (!sub_trigger) {
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
					else {
						sub_trigger = false;
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
				else if (buf.find("<m_subId>", 0) != buf.npos && hold_trigger) {
					fout << buf << endl;
					int endp = buf.find("</m_subId>", 0);
					buf = buf.substr(9, endp - 9);
					//cout << buf << " " << endp << endl;
					mult.str(buf);
					proc.str("");
					mult >> temp_sub;
					mult.clear();
					subs.insert(make_pair(temp_sub, t));//add a sub and its hold begin time to the sub map
					hold_trigger = false;
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