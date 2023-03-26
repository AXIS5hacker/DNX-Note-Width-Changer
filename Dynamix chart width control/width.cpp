/*
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
		//middle
		for (int i = 0; i < cs.m_notes.size(); i++) {

			if ((cs.m_notes[i].notetype == NORMAL || cs.m_notes[i].notetype == CHAIN) &&
				cs.m_notes[i].time >= st && cs.m_notes[i].time <= ed) {

				nw = cs.m_notes[i].width;//read width
				np = cs.m_notes[i].position;//read position
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;//multiply
				cs.m_notes[i].width = nw;
				cs.m_notes[i].position = np;
			}
			else if (cs.m_notes[i].notetype == HOLD &&
				cs.m_notes[i].time >= st && cs.m_notes[i].time <= ed) {//for hold notes

				nw = cs.m_notes[i].width;//read width
				np = cs.m_notes[i].position;//read position
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;//multiply
				//writeback
				cs.m_notes[i].width = nw;
				cs.m_notes[i].position = np;
				int temp_sub = cs.m_notes[i].subid;//find its sub
				if (cs.m_notes[temp_sub].id != temp_sub || cs.m_notes[temp_sub].notetype != SUB) {
					cout << "Hold-sub mismatch!" << endl;
					return 2;
				}
				else {
					cs.m_notes[temp_sub].width = cs.m_notes[i].width;
					cs.m_notes[temp_sub].position = cs.m_notes[i].position;
				}
			}
		}
		//left
		for (int i = 0; i < cs.m_left.size(); i++) {

			if ((cs.m_left[i].notetype == NORMAL || cs.m_left[i].notetype == CHAIN) &&
				cs.m_left[i].time >= st && cs.m_left[i].time <= ed) {
				nw = cs.m_left[i].width;//read width
				np = cs.m_left[i].position;//read position
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;//multiply
				//writeback
				cs.m_left[i].width = nw;
				cs.m_left[i].position = np;
			}
			else if (cs.m_left[i].notetype == HOLD &&
				cs.m_left[i].time >= st && cs.m_left[i].time <= ed) {//for hold notes
				nw = cs.m_left[i].width;//read width
				np = cs.m_left[i].position;//read position
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;//multiply
				//writeback
				cs.m_left[i].width = nw;
				cs.m_left[i].position = np;
				int temp_sub = cs.m_left[i].subid;//find its sub
				if (cs.m_left[temp_sub].id != temp_sub || cs.m_left[temp_sub].notetype != SUB) {
					cout << "Hold-sub mismatch!" << endl;
					return 2;
				}
				else {
					cs.m_left[temp_sub].width = cs.m_left[i].width;
					cs.m_left[temp_sub].position = cs.m_left[i].position;
				}
			}
		}
		//right
		for (int i = 0; i < cs.m_right.size(); i++) {

			if ((cs.m_right[i].notetype == NORMAL || cs.m_right[i].notetype == CHAIN) &&
				cs.m_right[i].time >= st && cs.m_right[i].time <= ed) {
				nw = cs.m_right[i].width;
				np = cs.m_right[i].position;
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;
				cs.m_right[i].width = nw;
				cs.m_right[i].position = np;
			}
			else if (cs.m_right[i].notetype == HOLD &&
				cs.m_right[i].time >= st && cs.m_right[i].time <= ed) {//for hold notes
				nw = cs.m_right[i].width;
				np = cs.m_right[i].position;
				np += nw * 0.5;
				nw *= w;
				np -= nw * 0.5;
				cs.m_right[i].width = nw;
				cs.m_right[i].position = np;
				int temp_sub = cs.m_right[i].subid;//find its sub
				if (cs.m_right[temp_sub].id != temp_sub || cs.m_right[temp_sub].notetype != SUB) {
					cout << "Hold-sub mismatch!" << endl;
					return 2;
				}
				else {
					cs.m_right[temp_sub].width = cs.m_right[i].width;
					cs.m_right[temp_sub].position = cs.m_right[i].position;
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