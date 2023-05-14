/**
* Creator:AXIS5
*/
#include "chart_store.h"
#include"width_change.h"

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


	int temp_sub = 0;//temporarily saves the id of a sub note

	fin.open(fn);//open file
	double nw = 0, np = 0, t = 0;
	//bool ch_trigger = false;//check if note is in the specified time range
	chart_store cs;//store the chart
	int fail = cs.readfile(fn);//open file

	if (fin.fail()) {
		return 1;
	}
	else {
		map<int, note>::iterator it;
		//middle

		for (it = cs.m_notes.begin(); it != cs.m_notes.end(); it++) {

			if ((it->second.notetype == NORMAL || it->second.notetype == CHAIN) &&
				it->second.time >= st && it->second.time <= ed) {

				nw = it->second.width;//read width
				np = it->second.position;//read position
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;//multiply
				it->second.width = nw;
				it->second.position = np;
			}
			else if (it->second.notetype == HOLD &&
				it->second.time >= st && it->second.time <= ed) {//for hold notes

				nw = it->second.width;//read width
				np = it->second.position;//read position
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;//multiply
				//writeback
				it->second.width = nw;
				it->second.position = np;

				int temp_sub = it->second.subid;//find its sub
				map<int, note>::iterator sub_it = cs.m_notes.find(temp_sub);//its sub note

				//sub not found
				if (sub_it == cs.m_notes.end() || sub_it->second.notetype != SUB) {
					cout << "Hold-sub mismatch!" << endl;
					return 2;
				}
				else {//sub found
					sub_it->second.width = it->second.width;
					sub_it->second.position = it->second.position;
				}
			}
		}

		//left
		for (it = cs.m_left.begin(); it != cs.m_left.end(); it++) {

			if ((it->second.notetype == NORMAL || it->second.notetype == CHAIN) &&
				it->second.time >= st && it->second.time <= ed) {
				nw = it->second.width;//read width
				np = it->second.position;//read position
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;//multiply
				//writeback
				it->second.width = nw;
				it->second.position = np;
			}
			else if (it->second.notetype == HOLD &&
				it->second.time >= st && it->second.time <= ed) {//for hold notes
				nw = it->second.width;//read width
				np = it->second.position;//read position
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;//multiply
				//writeback
				it->second.width = nw;
				it->second.position = np;

				int temp_sub = it->second.subid;//find its sub
				map<int, note>::iterator sub_it = cs.m_left.find(temp_sub);//its sub note

				//sub not found
				if (sub_it == cs.m_left.end() || sub_it->second.notetype != SUB) {
					cout << "Hold-sub mismatch!" << endl;
					return 2;
				}
				else {//sub found
					sub_it->second.width = it->second.width;
					sub_it->second.position = it->second.position;
				}
			}
		}

		//right
		for (it = cs.m_right.begin(); it != cs.m_right.end(); it++) {

			if ((it->second.notetype == NORMAL || it->second.notetype == CHAIN) &&
				it->second.time >= st && it->second.time <= ed) {
				nw = it->second.width;
				np = it->second.position;
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;
				it->second.width = nw;
				it->second.position = np;
			}
			else if (it->second.notetype == HOLD &&
				it->second.time >= st && it->second.time <= ed) {//for hold notes
				nw = it->second.width;
				np = it->second.position;
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;
				it->second.width = nw;
				it->second.position = np;

				int temp_sub = it->second.subid;//find its sub
				map<int, note>::iterator sub_it = cs.m_right.find(temp_sub);//its sub note

				//sub not found
				if (sub_it == cs.m_right.end() || sub_it->second.notetype != SUB) {
					cout << "Hold-sub mismatch!" << endl;
					return 2;
				}
				else {//sub found
					sub_it->second.width = it->second.width;
					sub_it->second.position = it->second.position;
				}
			}
		}
		//save
		if (!cs.to_file(_outf)) {
			return 2;
		}
		return 0;
	}
}