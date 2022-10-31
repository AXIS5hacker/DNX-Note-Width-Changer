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
int width_change(string fn, double w, string _outf) {
	/* The function that changes the width of a chart. */
	string buf;
	fin.open(fn);//open file
	double nw = 0, np = 0;
	if (fin.fail()) {
		return 1;
	}
	else {
		fout.open(_outf);
		if (!fout.fail()) {
			while (!fin.eof()) {
				getline(fin, buf);
				if (buf.find("<m_width>", 0) != buf.npos) {
					int endp = buf.find("</m_width>", 0);
					buf = buf.substr(9, endp);
					mult.str(buf);
					proc.str("");
					mult >> nw;
					np += nw * 0.5;
					nw *= w;
					np -= nw * 0.5;
					proc << "<m_position>" << np << "</m_position>" << endl;
					proc << "<m_width>" << nw << "</m_width>";
					buf = proc.str();
					fout << buf << endl;
				}
				else if (buf.find("<m_position>", 0) != buf.npos) {
					int endp = buf.find("</m_position>", 0);
					buf = buf.substr(12, endp);
					mult.str(buf);
					proc.str("");
					mult >> np;
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