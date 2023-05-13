/*
* Creator:AXIS5
* This is the realization of class chart_store
*/
#include "chart_store.h"
#include"width_change.h"
using namespace std;

//erase the space each 
string& trim(string& s) {
	if (!s.empty()) {
		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
	}
	return s;
}

int chart_store::readfile(string fn) {
	/* The function that reads a chart.
	*  fn:filename
	*/
	string buf;
	ifstream fin;
	istringstream extr;//for extracting numbers
	int modes = 0;//1->middle 2->left 3->right
	bool note_trigger = false;
	bool note_reading = false;//if reading a note
	note* tempnote = NULL;
	fin.open(fn);//open file
	if (fin.fail()) {
		cout << "Cannot open file, maybe it doesn't exist." << endl;
		return 1;
	}
	else {
		while (!fin.eof()) {
			getline(fin, buf);
			if (buf.find("<m_path>", 0) != buf.npos) {//read name
				int endt = buf.find("</m_path>", 0);
				buf = buf.substr(8, endt - 8);
				name = buf;
			}
			else if (buf.find("<m_barPerMin>", 0) != buf.npos) {//read barpm
				int endt = buf.find("</m_barPerMin>", 0);
				buf = buf.substr(13, endt - 13);
				extr.str(buf);
				extr >> barpm;
				extr.clear();
			}
			else if (buf.find("<m_timeOffset>", 0) != buf.npos) {//read barpm
				int endt = buf.find("</m_timeOffset>", 0);
				buf = buf.substr(14, endt - 14);
				extr.str(buf);
				extr >> offset;
				extr.clear();
			}
			else if (buf.find("<m_leftRegion>", 0) != buf.npos) {//read left
				int endt = buf.find("</m_leftRegion>", 0);
				buf = buf.substr(14, endt - 14);
				if (buf == "PAD") {
					ltype = PAD;
				}
				else if (buf == "MIXER") {
					ltype = MIXER;
				}
				else if (buf == "MULTI") {
					ltype = MULTI;
				}
				else {
					cout << "Read side error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("<m_rightRegion>", 0) != buf.npos) {//read right
				int endt = buf.find("</m_rightRegion>", 0);
				buf = buf.substr(15, endt - 15);
				if (buf == "PAD") {
					rtype = PAD;
				}
				else if (buf == "MIXER") {
					rtype = MIXER;
				}
				else if (buf == "MULTI") {
					rtype = MULTI;
				}
				else {
					cout << "Read side error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("<m_mapID>", 0) != buf.npos) {//read MapID
				int endt = buf.find("</m_mapID>", 0);
				buf = buf.substr(9, endt - 9);
				name_id = buf;
			}
			else if (buf.find("<m_notes>", 0) != buf.npos) {//start reading notes
				if (modes == 0)modes = 1;
				else if (modes == 1 || modes == 2 || modes == 3) {
					note_trigger = true;
				}
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("</m_notes>", 0) != buf.npos) {//stop reading notes
				if (note_trigger)note_trigger = false;
				else if (modes == 1) {
					modes = 0;
				}
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("<CMapNoteAsset>", 0) != buf.npos) {//head of a note
				if (!note_reading) {
					note_reading = true;
					tempnote = new note;
				}
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("</CMapNoteAsset>", 0) != buf.npos) {//end of a note
				if (note_reading) {
					switch (modes) {
					case 1:
						m_notes.push_back(*tempnote);
						break;
					case 2:
						m_left.push_back(*tempnote);
						break;
					case 3:
						m_right.push_back(*tempnote);
						break;
					default:
						delete tempnote;
						cout << "Read notes error" << endl;
						fin.close();
						return 1;
					}
					delete tempnote;
					note_reading = false;
				}
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("<m_id>", 0) != buf.npos) {//note id
				int endt = buf.find("</m_id>", 0);
				buf = buf.substr(6, endt - 6);
				extr.str(buf);
				if (tempnote != NULL) {
					extr >> tempnote->id;//read current note id
				}
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
				extr.clear();
			}
			else if (buf.find("<m_type>", 0) != buf.npos) {
				int endt = buf.find("</m_type>", 0);
				buf = buf.substr(8, endt - 8);
				if (tempnote != NULL) {
					if (buf == "NORMAL")
					{
						tempnote->notetype = NORMAL;
					}
					else if (buf == "CHAIN") {
						tempnote->notetype = CHAIN;
					}
					else if (buf == "HOLD") {
						tempnote->notetype = HOLD;
					}
					else if (buf == "SUB") {
						tempnote->notetype = SUB;
					}
				}
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("<m_time>", 0) != buf.npos && note_trigger == true) {//time
				int endt = buf.find("</m_time>", 0);
				buf = buf.substr(8, endt - 8);
				extr.str(buf);
				if (tempnote != NULL) {
					extr >> tempnote->time;//read current note time
					extr.clear();
				}
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("<m_position>", 0) != buf.npos) {//position
				int endt = buf.find("</m_position>", 0);
				buf = buf.substr(12, endt - 12);
				extr.str(buf);
				if (tempnote != NULL) {
					extr >> tempnote->position;//read current note position
					extr.clear();
				}
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("<m_width>", 0) != buf.npos) {//width
				int endt = buf.find("</m_width>", 0);
				buf = buf.substr(9, endt - 9);
				extr.str(buf);
				if (tempnote != NULL) {
					extr >> tempnote->width;//read current note width
					extr.clear();
				}
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("<m_subId>", 0) != buf.npos) {//subId
				int endt = buf.find("</m_subId>", 0);
				buf = buf.substr(9, endt - 9);
				extr.str(buf);
				if (tempnote != NULL) {
					extr >> tempnote->subid;//read subId
					extr.clear();
				}
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}

			else if (buf.find("<m_notesLeft>", 0) != buf.npos) {//left notes
				if (modes == 0)modes = 2;
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("</m_notesLeft>", 0) != buf.npos) {//left notes end
				if (modes == 2)modes = 0;
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("<m_notesRight>", 0) != buf.npos) {//right notes
				if (modes == 0)modes = 3;
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
			else if (buf.find("</m_notesRight>", 0) != buf.npos) {//right notes end
				if (modes == 3)modes = 0;
				else {
					cout << "Read notes error" << endl;
					fin.close();
					return 1;
				}
			}
		}
		fin.close();
		return 0;
	}
}

void side_out(const vector<note>& v, ofstream& of) {//output each side
	of << fixed << setprecision(6);
	for (int i = 0; i < v.size(); i++) {
		of << "<CMapNoteAsset>" << endl;
		of << "<m_id>" << v[i].id << "</m_id>" << endl;
		of << "<m_type>";
		switch (v[i].notetype) {
		case types::NORMAL:
			of << "NORMAL";
			break;
		case types::CHAIN:
			of << "CHAIN";
			break;
		case types::HOLD:
			of << "HOLD";
			break;
		case types::SUB:
			of << "SUB";
			break;
		}
		of << "</m_type>" << endl;
		of << "<m_time>" << v[i].time << "</m_time>" << endl;
		of << "<m_position>" << v[i].position << "</m_position>" << endl;
		of << "<m_width>" << v[i].width << "</m_width>" << endl;
		of << "<m_subId>" << v[i].subid << "</m_subId>" << endl;
		of << "</CMapNoteAsset>" << endl;
	}
}
bool chart_store::to_file(string f) {
	ofstream fout;
	fout.open(f);
	if (!fout.fail()) {

		fout << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << endl;
		fout << "<CMap xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">" << endl;
		fout << "<m_path>" << name << "</m_path>" << endl;
		fout << "<m_barPerMin>" << barpm << "</m_barPerMin>" << endl;
		fout << "<m_timeOffset>" << offset << "</m_timeOffset>" << endl;
		fout << "<m_leftRegion>";
		switch (ltype) {
		case PAD:
			fout << "PAD";
			break;
		case MIXER:
			fout << "MIXER";
			break;
		case MULTI:
			fout << "MULTI";
			break;
		}
		fout << "</m_leftRegion>" << endl;
		fout << "<m_rightRegion>";
		switch (rtype) {
		case PAD:
			fout << "PAD";
			break;
		case MIXER:
			fout << "MIXER";
			break;
		case MULTI:
			fout << "MULTI";
			break;
		}
		fout << "</m_rightRegion>" << endl;
		fout << "<m_mapID>" << name_id << "</m_mapID>" << endl;
		//notes middle
		fout << "<m_notes>" << endl;
		fout << "<m_notes>" << endl;

		side_out(m_notes, fout);
		fout << "</m_notes>" << endl;
		fout << "</m_notes>" << endl;
		//notes left
		fout << "<m_notesLeft>" << endl;
		fout << "<m_notes>" << endl;
		side_out(m_left, fout);
		fout << "</m_notes>" << endl;
		fout << "</m_notesLeft>" << endl;
		//notes right
		fout << "<m_notesRight>" << endl;
		fout << "<m_notes>" << endl;
		side_out(m_right, fout);
		fout << "</m_notes>" << endl;
		fout << "</m_notesRight>" << endl;
		//end
		fout << "</CMap>" << endl;
		fout.close();
		return true;
	}
	else {
		return false;
	}
}