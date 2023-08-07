/**
* Creator:AXIS5
* This is the realization of class chart_store
*/
#include "chart_store.h"
#include"width_change.h"
using namespace std;

void chart_store::skip_space() {
	/**
	*  The function skips the white space and new line in xml file
	*/
	while (t_buf[buf_index] == ' ' || t_buf[buf_index] == '\t' || t_buf[buf_index] == '\n' || t_buf[buf_index] == '\r') {
		buf_index++;
	}
}

bool chart_store::parse_decl() {
	//check if parsing declaration
	if (t_buf.compare(buf_index, 5, "<?xml") != 0) {
		return false;
	}
	buf_index += 5;
	size_t pos = t_buf.find("?>", buf_index);
	if (pos == t_buf.npos) {
		return false;
	}
	buf_index = pos + 2;
	return true;
}

bool chart_store::parse_comment() {
	//check if parsing comment
	if (t_buf.compare(buf_index, 4, "<!--") != 0) {
		return false;
	}
	buf_index += 4;
	size_t pos = t_buf.find("-->", buf_index);
	if (pos == t_buf.npos) {
		return false;
	}
	buf_index = pos + 3;
	return true;
}

int chart_store::readfile(string fn) {
	/** The function that reads a chart.
	*  fn:filename
	*  return:0:success;1:fail
	*/

	//empty the maps that stores the previous chart
	m_notes.clear();
	m_left.clear();
	m_right.clear();

	string buf;
	ifstream fin;

	modes = 0;//0->none 1->middle 2->left 3->right
	tempnote = NULL;
	note_trigger = false;
	note_reading = false;//if reading a note


	fin.open(fn);//open file
	if (fin.fail()) {
		cout << "Cannot open file \"" + fn + "\", maybe you do not have access to it or it doesn't exist." << endl;
		return 1;
	}
	else {
		//a newer method of parsing the xml file
		stringstream ss;
		ss << fin.rdbuf();
		t_buf = ss.str();//t_buf now stores the file content
		buf_index = 0;//points to the beginning of the file


		//parse xml
		skip_space();
		if (!parse_decl()) {
			//not parsing declaration
			return 1;
		}
		skip_space();
		while (t_buf.compare(buf_index, 4, "<!--") == 0) {
			if (!parse_comment()) {
				//error parsing comment
				return 1;
			}
			skip_space();
		}
		if (t_buf[buf_index] == '<' &&
			(isalpha(t_buf[buf_index + 1]) || t_buf[buf_index + 1] == '_'))
		{
			//start to parse the root node of the xml chart
			try {
				parse_elem();
			}
			catch (exception& ex) {
				cout << ex.what() << endl;
				return 1;
			}
		}
		fin.close();
		return 0;

	}
}

void chart_store::parse_elem() {

	istringstream extr;//for extracting numbers


	buf_index++;// '<'
	skip_space();
	const string& tag = parse_elem_name();
	while (t_buf[buf_index] != '\0') {
		skip_space();
		if (t_buf[buf_index] == '/') {
			//empty xml tag:<.../>
			if (t_buf[buf_index + 1] == '>') {
				//end of an empty xml tag
				buf_index += 2;
				break;
			}
			else {
				throw std::logic_error("xml empty element is error");
			}
		}
		else if (t_buf[buf_index] == '>') {
			//normal xml tag:<...>
			buf_index++;//'>'
			if (tag == "m_notes") {//start reading notes
				if (modes == 0)modes = 1;
				else if (modes == 1 || modes == 2 || modes == 3) {
					note_trigger = true;
				}
				else {
					throw std::logic_error("Read notes error: Syntax Error when reading middle notes");
				}
			}
			else if (tag == "CMapNoteAsset") {//head of a note
				if (!note_reading) {
					note_reading = true;
					tempnote = new note;
				}
				else {
					throw std::logic_error("Read notes error: <CMapNoteAsset> note asset error, triggered at"
						+ __LINE__ + (string)"in" + __FILE__);
				}
			}
			else if (tag == "m_notesLeft") {//left notes
				if (modes == 0)modes = 2;
				else {
					throw std::logic_error("Read notes error: Syntax Error when reading left notes");
				}
			}
			else if (tag == "m_notesRight") {//right notes
				if (modes == 0)modes = 3;
				else {
					throw std::logic_error("Read notes error: Syntax Error when reading right notes");
				}
			}
			//read text
			string text = parse_elem_text();
			if (text != "") {
				if (tag == "m_path") { //read name
					name = text;
				}
				else if (tag == "m_barPerMin") {//read barpm
					extr.str(text);
					extr >> barpm;
					extr.clear();
				}
				else if (tag == "m_timeOffset") {//read offset
					extr.str(text);
					extr >> offset;
					extr.clear();
				}
				else if (tag == "m_leftRegion") {//read left
					if (text == "PAD") {
						ltype = PAD;
					}
					else if (text == "MIXER") {
						ltype = MIXER;
					}
					else if (text == "MULTI") {
						ltype = MULTI;
					}
					else {
						throw std::logic_error("Read side error: Invalid Left Side Type");
					}
				}
				else if (tag == "m_rightRegion") {//read right
					if (text == "PAD") {
						rtype = PAD;
					}
					else if (text == "MIXER") {
						rtype = MIXER;
					}
					else if (text == "MULTI") {
						rtype = MULTI;
					}
					else {
						throw std::logic_error("Read side error: Invalid Right Side Type");
					}
				}
				else if (tag == "m_mapID") {//read MapID
					name_id = text;
				}
				else if (tag == "m_id") {//note id
					extr.str(text);
					if (tempnote != NULL) {
						extr >> tempnote->id;//read current note id
					}
					else {
						throw std::logic_error("Read notes error: Unable to create object \"note\"");
					}
					extr.clear();
				}
				else if (tag == "m_type") {//note type
					if (tempnote != NULL) {
						if (text == "NORMAL")
						{
							tempnote->notetype = NORMAL;
						}
						else if (text == "CHAIN") {
							tempnote->notetype = CHAIN;
						}
						else if (text == "HOLD") {
							tempnote->notetype = HOLD;
						}
						else if (text == "SUB") {
							tempnote->notetype = SUB;
						}
					}
					else {
						if (tempnote != NULL) {
							throw std::logic_error("Read notes error: Invalid note type at note #" + tempnote->id);
						}
						else {
							throw std::logic_error("Read notes error: Invalid note type at undefined note");
						}
					}
				}
				else if (tag == "m_time" && note_trigger == true) {//note time
					extr.str(text);
					if (tempnote != NULL) {
						extr >> tempnote->time;//read current note time
						extr.clear();
					}
					else {
						throw std::logic_error("Read notes error: Unable to create object \"note\"");
					}
				}
				else if (tag == "m_position") {//note position
					extr.str(text);
					if (tempnote != NULL) {
						extr >> tempnote->position;//read current note position
						extr.clear();
					}
					else {
						throw std::logic_error("Read notes error: Unable to create object \"note\"");
					}
				}
				else if (tag == "m_width") {//note width
					extr.str(text);
					if (tempnote != NULL) {
						extr >> tempnote->width;//read current note width
						extr.clear();
					}
					else {
						throw std::logic_error("Read notes error: Unable to create object \"note\"");
					}
				}
				else if (tag == "m_subId") {//sub id of a hold,-1 for non-hold notes
					extr.str(text);
					if (tempnote != NULL) {
						extr >> tempnote->subid;//read subId
						extr.clear();
					}
					else {
						throw std::logic_error("Read notes error: Unable to create object \"note\"");
					}
				}
			}
		}
		else if (t_buf[buf_index] == '<') {
			if (t_buf[buf_index + 1] == '/') {
				//xml tag end:</...>

				//scan
				string end_tag = "</" + tag + ">";
				size_t pos = t_buf.find(end_tag, buf_index);
				if (pos == t_buf.npos) {
					throw std::logic_error("xml element " + tag + " end tag not found.");
				}
				buf_index = pos + end_tag.size();

				//action
				if (tag == "m_notes") {//stop reading notes
					if (note_trigger)note_trigger = false;
					else if (modes == 1) {
						modes = 0;
					}
					else {
						throw std::logic_error("Read notes error: Duplicated stop tags of reading a side");
					}
				}
				else if (tag == "CMapNoteAsset") {//end of a note
					if (note_reading) {
						int temp_id;//note id(temporary)
						switch (modes) {
						case 1:
							temp_id = tempnote->id;
							m_notes.insert(make_pair(temp_id, *tempnote));
							break;
						case 2:
							temp_id = tempnote->id;
							m_left.insert(make_pair(temp_id, *tempnote));
							break;
						case 3:
							temp_id = tempnote->id;
							m_right.insert(make_pair(temp_id, *tempnote));
							break;
						default:
							delete tempnote;
							throw std::logic_error("Read notes error");
						}
						delete tempnote;
						note_reading = false;
					}
					else {
						throw std::logic_error("Read notes error: Syntax error when stop reading a note");
					}
				}
				else if (tag == "m_notesLeft") {//left notes end
					if (modes == 2)modes = 0;
					else {
						throw std::logic_error("Read notes error: Syntax error when stop reading left notes");
					}
				}
				else if (tag == "m_notesRight") {//right notes end
					if (modes == 3)modes = 0;
					else {
						throw std::logic_error("Read notes error: Syntax error when stop reading right notes");
					}
				}

				//finish parsing element
				break;
			}
			else if (t_buf.compare(buf_index, 4, "<!--") == 0) {
				//xml comment
				if (!parse_comment()) {
					throw std::logic_error("error parsing comment");
				}
			}
			else {
				//parse child
				parse_elem();
			}
		}
		else {
			//xml tag attributes
			string key = parse_elem_attr_key();
			skip_space();
			if (t_buf[buf_index] != '=') {
				throw std::logic_error("xml attribute error:" + key);
			}
			buf_index++;//'='
			skip_space();
			try {
				string val = parse_elem_attr_val();
			}
			catch (exception& ex) {
				throw std::logic_error(ex.what());
			}
		}
	}

}

string chart_store::parse_elem_name() {
	//parsing a tag of xml
	int pos = buf_index;
	if (isalpha(t_buf[buf_index]) || t_buf[buf_index] == '_') {
		buf_index++;
		while (isalnum(t_buf[buf_index]) || t_buf[buf_index] == '_' ||
			t_buf[buf_index] == '-' || t_buf[buf_index] == ':' ||
			t_buf[buf_index] == '.') {
			//word analyze
			buf_index++;
		}
	}
	return t_buf.substr(pos, buf_index - pos);
}

string chart_store::parse_elem_text() {
	//parsing the text of an xml tag
	int pos = buf_index;
	while (t_buf[buf_index] != '<') {
		buf_index++;
	}
	return t_buf.substr(pos, buf_index - pos);
}

string chart_store::parse_elem_attr_key() {
	//parsing an attribute of an xml tag
	int pos = buf_index;
	if (isalpha(t_buf[buf_index]) || t_buf[buf_index] == '_') {
		buf_index++;
		while (isalnum(t_buf[buf_index]) || t_buf[buf_index] == '_' ||
			t_buf[buf_index] == '-' || t_buf[buf_index] == ':') {
			//word analyze
			buf_index++;
		}
	}
	return t_buf.substr(pos, buf_index - pos);
}

string chart_store::parse_elem_attr_val() {
	//parsing the value of an attribute
	if (t_buf[buf_index] != '\"') {
		throw std::logic_error("attribute value missing \"\"");
	}
	buf_index++;//'"'
	int pos = buf_index;
	while (t_buf[buf_index] != '\"') {
		//read value
		buf_index++;
	}
	buf_index++;//'"'
	return t_buf.substr(pos, buf_index - pos - 1);
}

void chart_store::side_out(const map<int, note>& v, ofstream& of) {//output each side
	of << fixed << setprecision(6);
	map<int, note>::const_iterator it;//the iterator that accesses constant maps
	for (it = v.begin(); it != v.end(); it++) {
		of << "<CMapNoteAsset>" << endl;
		of << "<m_id>" << it->second.id << "</m_id>" << endl;
		of << "<m_type>";
		switch (it->second.notetype) {
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
		of << "<m_time>" << it->second.time << "</m_time>" << endl;
		of << "<m_position>" << it->second.position << "</m_position>" << endl;
		of << "<m_width>" << it->second.width << "</m_width>" << endl;
		of << "<m_subId>" << it->second.subid << "</m_subId>" << endl;
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