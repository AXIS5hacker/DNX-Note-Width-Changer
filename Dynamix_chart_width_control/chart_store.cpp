/**
* Creator:AXIS5
* This is the realization of class chart_store
*/
#include "chart_store.h"
#include"defs.h"

using namespace std;
//functions used
bool numcheck_int(string s) {
	for (auto& p : s) {
		if ((p < '0' || p > '9') && p != '\n' && p != ' ' && p != '\t'&&p!='-'&&p!='+') {
			return false;
		}
	}
	return true;
}

bool numcheck_decimal(string s) {
	bool decimal_point_exists = false;
	for (auto& p : s) {
		if ((p < '0' || p > '9') && p != '\n' && p != ' ' && p != '\t' && p != '-' && p != '+') {
			if (p == '.'&& (!decimal_point_exists)) {
				decimal_point_exists = true;
			}
			else {
				return false;
			}
		}
	}
	return true;
}

//functions in class
void chart_store::skip_space() {
	/**
	*  The function skips the white space and new line in xml file
	*/
	while (t_buf[buf_index] == ' ' || t_buf[buf_index] == '\t' || t_buf[buf_index] == '\n' || t_buf[buf_index] == '\r') {
		if (t_buf[buf_index] == '\n') {
			lines++;
		}
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
	else {
		//get the number of lines the comment has
		string comm = t_buf.substr(buf_index, pos - buf_index);
		int nlines = count(comm.begin(), comm.end(), '\n');
		lines += nlines;
	}
	buf_index = pos + 3;
	return true;
}

void chart_store::clear() {
	//init note list
	m_notes.clear();
	m_left.clear();
	m_right.clear();
	//init hold-sub list 
	hold_mid.clear();
	hold_left.clear();
	hold_right.clear();
	sub_mid.clear();
	sub_left.clear();
	sub_right.clear();
	//init mismatched note list
	mismatched_notes.clear();

	chart_filename = "";
	name = "";
	name_id = "";
	offset = 0;
	barpm = 0;
	ltype = sides::UNKNOWN;
	rtype = sides::UNKNOWN;
	lines = 1;//beginning
}

int chart_store::readfile(string fn) {
	/** The function that reads a chart.
	*  fn:filename
	*  return:0:success;others:see defs.h;
	*/

	//empty the maps that stores the previous chart
	clear();

	string buf;
	ifstream fin;
	bool mismatch = false;//check if hold-sub mismatch

	modes = 0;//0->none 1->middle 2->left 3->right
	tempnote = NULL;
	note_trigger = false;
	note_reading = false;//if reading a note

	int chart_flags = 0;

	fin.open(fn);//open file
	if (fin.fail()) {
		throw std::logic_error("Cannot open file \"" + fn + "\", maybe you do not have access to it or it doesn't exist.");
		//cout << "Cannot open file \"" + fn + "\", maybe you do not have access to it or it doesn't exist." << endl;

		//return FAIL_TO_OPEN;
	}
	else {
		//a newer method of parsing the xml file
		stringstream ss;
		ss << fin.rdbuf();
		t_buf = ss.str();//t_buf now stores the file content
		fin.close();
		if (t_buf == "") {
			throw std::logic_error("Empty file.");
			return 1;
		}
		buf_index = 0;//points to the beginning of the file


		//parse xml stage
		skip_space();
		if (!parse_decl()) {
			//get error position
			char lln[64];
			snprintf(lln, sizeof(lln), "%d.", lines);

			throw std::logic_error("Parse declaration error at line " + string(lln));
			//not parsing declaration
			return 1;
		}
		skip_space();
		while (t_buf.compare(buf_index, 4, "<!--") == 0) {
			if (!parse_comment()) {
				//get error position
				char lln[64];
				snprintf(lln, sizeof(lln), "%d.", lines);

				throw std::logic_error("Parse comment error at line" + string(lln));
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
				throw std::logic_error(ex.what());
				//cout << ex.what() << endl;

				return 1;
			}
		}

		//chart checking stage
		//1.Check whether the side types are set
		if (ltype == sides::UNKNOWN) {
			//throw std::logic_error("Left side type is not specified! Please check your chart!");
			chart_flags |= LEFT_SIDE_MISSING;
		}
		if (rtype == sides::UNKNOWN) {
			//throw std::logic_error("Right side type is not specified! Please check your chart!");
			chart_flags |= RIGHT_SIDE_MISSING;
		}


		//2.check if barpm is missing
		if (barpm <= 0) {
			//throw std::logic_error("Illegal Barpm! Please check your chart!");
			chart_flags |= BARPM_MISSING;
		}

		//3.Hold-Sub match checking stage
		//middle
		for (auto& it : hold_mid) {
			if (sub_mid.find(it.second) == sub_mid.end()) {
				//store mismatch hold
				mismatched_notes.push_back(make_pair(it.first, "middle"));
				mismatch = true;
			}
			else {
				sub_mid.erase(it.second);
			}
		}
		if (!sub_mid.empty()) {
			//store mismatch sub
			for (auto& it : sub_mid) {
				mismatched_notes.push_back(make_pair(it, "middle"));
			}
			mismatch = true;
		}
		//left
		for (auto& it : hold_left) {
			if (sub_left.find(it.second) == sub_left.end()) {
				//store mismatch hold
				mismatched_notes.push_back(make_pair(it.first, "left"));
				mismatch = true;
			}
			else {
				sub_left.erase(it.second);
			}
		}
		if (!sub_left.empty()) {
			//store mismatch sub
			for (auto& it : sub_left) {
				mismatched_notes.push_back(make_pair(it, "left"));
			}
			mismatch = true;
		}
		//right
		for (auto& it : hold_right) {
			if (sub_right.find(it.second) == sub_right.end()) {
				//store mismatch hold
				mismatched_notes.push_back(make_pair(it.first, "right"));
				mismatch = true;
			}
			else {
				sub_right.erase(it.second);
			}
		}
		if (!sub_right.empty()) {
			//store mismatch sub
			for (auto& it : sub_right) {
				mismatched_notes.push_back(make_pair(it, "right"));
			}
			mismatch = true;
		}

		chart_filename = fn;//store the filename of a chart
		if (mismatch)chart_flags |= HOLD_SUB_MISMATCH;
		return chart_flags;

	}
}


void chart_store::parse_elem() {

	istringstream extr;//for extracting numbers

	buf_index++;// '<'
	skip_space();
	const string& tag = parse_elem_name();
	int tag_line = lines;//temporarily stores the position of the tag
	while (buf_index >= t_buf.length() || t_buf[buf_index] != '\0') {
		skip_space();
		if (t_buf[buf_index] == '/') {
			//empty xml tag:<.../>
			if (t_buf[buf_index + 1] == '>') {
				//end of an empty xml tag
				buf_index += 2;
				break;
			}
			else {
				//get error position
				char lln[64];
				snprintf(lln, sizeof(lln), "%d.", lines);

				throw std::logic_error("xml empty element is error. Occured at line " + string(lln));
				return;
			}
		}
		else if (t_buf[buf_index] == '>') {
			//normal xml tag:<...>
			buf_index++;//'>'
			if (tag == "m_notes") {//start reading notes
				if (modes == 0)modes = 1;//middle mode
				else if (modes == 1 || modes == 2 || modes == 3) {
					//start reading notes
					note_trigger = true;
				}
				else {
					//get error position
					char lln[64];
					snprintf(lln, sizeof(lln), "%d.", lines);

					throw std::logic_error("Read notes error: Syntax Error when reading middle notes. Occured at line " + string(lln));
					return;
				}
			}
			else if (tag == "CMapNoteAsset") {//head of a note
				if (note_trigger) {
					if (!note_reading) {
						note_reading = true;
						tempnote = new note;
					}
					else {
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read notes error: <CMapNoteAsset> note asset error, triggered at line "
							+ string(lln));
						return;
					}
				}
				else {
					//get error position
					char lln[64];
					snprintf(lln, sizeof(lln), "%d.", lines);

					throw std::logic_error("Read notes error: Unexpected note detected outside the note lists, triggered at line "
						+ string(lln));
					return;
				}
			}
			else if (tag == "m_notesLeft") {//left notes
				if (modes == 0)modes = 2;
				else {
					//get error position
					char lln[64];
					snprintf(lln, sizeof(lln), "%d.", lines);

					throw std::logic_error("Read notes error: Syntax Error when reading left notes. Occured at line " + string(lln));
					return;
				}
			}
			else if (tag == "m_notesRight") {//right notes
				if (modes == 0)modes = 3;
				else {
					//get error position
					char lln[64];
					snprintf(lln, sizeof(lln), "%d.", lines);

					throw std::logic_error("Read notes error: Syntax Error when reading right notes. Occured at line " + string(lln));
					return;
				}
			}
			//read text
			string text = "";
			try {
				text = parse_elem_text();
			}
			catch (exception& ex) {
				char lln[64];
				snprintf(lln, sizeof(lln), "%d.", tag_line);
				string errmsg = ex.what();
				throw std::logic_error(errmsg+" Tag begins at line " + string(lln));
				return;
			}
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
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read side error: Invalid Left Side Type. Occured at line " + string(lln));
						return;
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
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read side error: Invalid Right Side Type. Occured at line " + string(lln));
						return;
					}
				}
				else if (tag == "m_mapID") {//read MapID
					name_id = text;
				}
				else if (tag == "m_id") {//note id
					if (note_reading) {
						//only numbers should exist in id
						bool valid = numcheck_int(text);
						if (!valid) {
							//get error position
							char lln[64];
							snprintf(lln, sizeof(lln), "%d.", lines);

							throw std::logic_error("Read note error: ID is not an integer. Occured at line " + string(lln));
							return;
						}
						extr.str(text);
						if (tempnote != NULL) {
							extr >> tempnote->id;//read current note id
							if (extr.fail()) {
								printf("boo\n");
							}
						}
						else {
							throw std::logic_error("Read notes error: Unable to create object \"note\"");
							return;
						}
						extr.clear();
					}
					else {	//not in <CMapNoteAsset>
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read note error: Note info detected outside <CMapNoteAsset>. Occured at line " + string(lln));
						return;
					}
				}
				else if (tag == "m_type") {//note type
					if (note_reading) {
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
							else {
								char errnote[64], lln[64];
								snprintf(errnote, sizeof(errnote), "%d", tempnote->id);
								snprintf(lln, sizeof(lln), "%d.", lines);
								throw std::logic_error("Read notes error: Invalid note type at note #" + string(errnote) + ".\n Please check line " + string(lln));

							}
						}
						else {

							throw std::logic_error("Read notes error: Invalid note type at undefined note");
						}
					}
					else {//not in <CMapNoteAsset>
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read note error: Note info detected outside <CMapNoteAsset>. Occured at line " + string(lln));
						return;
					}
				}
				else if (tag == "m_time" && note_trigger == true) {//note time
					if (note_reading) {
						bool valid = numcheck_decimal(text);
						if (!valid) {
							//get error position
							char lln[64];
							snprintf(lln, sizeof(lln), "%d.", lines);

							throw std::logic_error("Read note error: time is not a valid decimal expression. Occured at line " + string(lln));
							return;
						}
						extr.str(text);
						if (tempnote != NULL) {
							extr >> tempnote->time;//read current note time
							extr.clear();
						}
						else {
							throw std::logic_error("Read notes error: Unable to create object \"note\"");
							return;
						}
					}
					else {//not in <CMapNoteAsset>
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read note error: Note info detected outside <CMapNoteAsset>. Occured at line " + string(lln));
						return;
					}
				}
				else if (tag == "m_position") {//note position
					if (note_reading) {
						bool valid = numcheck_decimal(text);
						if (!valid) {
							//get error position
							char lln[64];
							snprintf(lln, sizeof(lln), "%d.", lines);

							throw std::logic_error("Read note error: note position is not a valid decimal expression. Occured at line " + string(lln));
							return;
						}
						extr.str(text);
						if (tempnote != NULL) {
							extr >> tempnote->position;//read current note position
							extr.clear();
						}
						else {
							throw std::logic_error("Read notes error: Unable to create object \"note\"");
							return;
						}
					}
					else {//not in <CMapNoteAsset>
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read note error: Note info detected outside <CMapNoteAsset>. Occured at line " + string(lln));
						return;

					}
				}
				else if (tag == "m_width") {//note width
					if (note_reading) {
						bool valid = numcheck_decimal(text);
						if (!valid) {
							//get error position
							char lln[64];
							snprintf(lln, sizeof(lln), "%d.", lines);

							throw std::logic_error("Read note error: note width is not a valid decimal expression. Occured at line " + string(lln));
							return;
						}
						extr.str(text);
						if (tempnote != NULL) {
							extr >> tempnote->width;//read current note width
							extr.clear();
						}
						else {
							throw std::logic_error("Read notes error: Unable to create object \"note\"");
							return;
						}
					}
					else {//not in <CMapNoteAsset>
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read note error: Note info detected outside <CMapNoteAsset>. Occured at line " + string(lln));
						return;

					}
				}
				else if (tag == "m_subId") {//sub id of a hold,-1 for non-hold notes
					if (note_reading) {
						//only numbers should exist in id
						bool valid = numcheck_int(text);
						if (!valid) {
							//get error position
							char lln[64];
							snprintf(lln, sizeof(lln), "%d.", lines);

							throw std::logic_error("Read note error: note sub ID is not an integer. Occured at line " + string(lln));
							return;
						}
						extr.str(text);
						if (tempnote != NULL) {
							extr >> tempnote->subid;//read subId
							extr.clear();
						}
						else {
							throw std::logic_error("Read notes error: Unable to create object \"note\"");
							return;
						}
					}
					else {//not in <CMapNoteAsset>
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read note error: Note info detected outside <CMapNoteAsset>. Occured at line " + string(lln));
						return;

					}
				}
				//ignore all other tags
			}
		}
		else if (t_buf[buf_index] == '<') {
			if (t_buf[buf_index + 1] == '/') {
				//xml tag close:</...>

				//scan

				int buf_scan = buf_index + 2;//scan after "</"
				int pos_scan = buf_scan;
				int end_tag_line = lines;//the position of tag close

				//scan for tag close
				if (isalpha(t_buf[buf_scan]) || t_buf[buf_scan] == '_') {
					buf_scan++;
					while (isalnum(t_buf[buf_scan]) || t_buf[buf_scan] == '_' ||
						t_buf[buf_scan] == '-' || t_buf[buf_scan] == ':' ||
						t_buf[buf_scan] == '.') {
						//word analyze
						buf_scan++;
					}
				}
				string end_tag = t_buf.substr(pos_scan, buf_scan - pos_scan);
				//tag close does not match the tag
				if (end_tag != tag) {
					//get error position
					char lln[64];
					snprintf(lln, sizeof(lln), "%d.", tag_line);

					throw std::logic_error("xml element " + tag + " is not closed.\nThe tag is at line " + string(lln));
					return;
				}
				//tag close matches the tag
				else {
					buf_index = buf_scan;
				}
				skip_space();
				if (t_buf[buf_index] != '>') {
					//get error position
					char lln[64];
					snprintf(lln, sizeof(lln), "%d.", end_tag_line);

					throw std::logic_error("expected \'>\' at line " + string(lln));
					return;
				}
				else {
					buf_index++;//'>'
				}

				/*
				string end_tag_x = "</" + tag + ">";

				size_t pos = t_buf.find(end_tag, buf_index);
				if (pos == t_buf.npos) {
					//get error position
					char lln[64];
					sprintf_s(lln, "%d.", lines);

					throw std::logic_error("xml element " + tag + " is not closed.\nOccured at line " + string(lln));
					return;
				}
				buf_index = pos + end_tag.size();
				*/

				//action
				if (tag == "m_notes") {//stop reading notes
					if (note_trigger)note_trigger = false;
					else if (modes == 1) {
						modes = 0;
					}
					else {
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read notes error: Duplicated stop tags of reading a side.\nOccured at line " + string(lln));
						return;
					}
				}
				else if (tag == "CMapNoteAsset") {//end of a note
					if (note_reading) {
						char lln[64], id_string[64];
						int temp_id = tempnote->id;//note id(temporary)
						//checking note info integrity
						//id
						if (temp_id == -1) {

							snprintf(lln, sizeof(lln), "%d", tag_line);

							throw std::logic_error("Error: Note without id.\nThe note begins at line " + string(lln) + " in the XML.");
						}
						else {
							//note type
							if (tempnote->notetype == types::NULLTP) {
								snprintf(lln, sizeof(lln), "%d", tag_line);
								snprintf(id_string, sizeof(id_string), "%d", temp_id);
								throw std::logic_error("Error: Note #" + string(id_string) + " type not specified.\nThe note begins at line " + string(lln) + " in the XML.");
							}
							//note position
							if (tempnote->position < -1e7) {
								snprintf(lln, sizeof(lln), "%d", tag_line);
								snprintf(id_string, sizeof(id_string), "%d", temp_id);

								throw std::logic_error("Error: Note #" + string(id_string) + " position not specified.\nThe note begins at line " + string(lln) + " in the XML.");
							}
							//note width
							if (tempnote->width < -1e7) {
								snprintf(lln, sizeof(lln), "%d", tag_line);
								snprintf(id_string, sizeof(id_string), "%d", temp_id);

								throw std::logic_error("Error: Note #" + string(id_string) + " width not specified.\nThe note begins at line " + string(lln) + " in the XML.");
							}
							//note time
							if (tempnote->time < -1e7) {
								snprintf(lln, sizeof(lln), "%d", tag_line);
								snprintf(id_string, sizeof(id_string), "%d", temp_id);

								throw std::logic_error("Error: Note #" + string(id_string) + " time not specified.\nThe note begins at line " + string(lln) + " in the XML.");
							}
							//sub id
							if (tempnote->subid == INT_MIN) {
								snprintf(lln, sizeof(lln), "%d", tag_line);
								snprintf(id_string, sizeof(id_string), "%d", temp_id);

								throw std::logic_error("Error: Note #" + string(id_string) + " subId not specified.\nThe note begins at line " + string(lln) + " in the XML.");
							}
						}
						//note info is complete
						switch (modes) {
						case 1:
							//scan for duplicated note id
							if (m_notes.find(temp_id) == m_notes.end()) {
								m_notes.insert(make_pair(temp_id, *tempnote));
								//store hold for hold-sub checking
								if (tempnote->notetype == types::HOLD) {
									hold_mid.insert(make_pair(temp_id, tempnote->subid));
								}
								//store sub for hold-sub checking
								else if (tempnote->notetype == types::SUB) {
									sub_mid.insert(temp_id);
								}
							}
							//duplicated
							else {
								snprintf(lln, sizeof(lln), "%d", tag_line);
								snprintf(id_string, sizeof(id_string), "%d", temp_id);

								throw std::logic_error("Error: Duplicated note id: " + string(id_string) + ".\nThe note begins at line " + string(lln) + " in the XML.");
							}
							break;
						case 2:
							//scan for duplicated note id
							if (m_left.find(temp_id) == m_left.end()) {
								m_left.insert(make_pair(temp_id, *tempnote));
								//store hold for hold-sub checking
								if (tempnote->notetype == types::HOLD) {
									hold_left.insert(make_pair(temp_id, tempnote->subid));
								}
								//store sub for hold-sub checking
								else if (tempnote->notetype == types::SUB) {
									sub_left.insert(temp_id);
								}
							}
							//duplicated
							else {
								snprintf(lln, sizeof(lln), "%d", tag_line);
								snprintf(id_string, sizeof(id_string), "%d", temp_id);

								throw std::logic_error("Error: Duplicated note id: " + string(id_string) + ".\nThe note begins at line " + string(lln) + " in the XML.");
							}
							break;
						case 3:
							//scan for duplicated note id
							if (m_right.find(temp_id) == m_right.end()) {
								m_right.insert(make_pair(temp_id, *tempnote));
								//store hold for hold-sub checking
								if (tempnote->notetype == types::HOLD) {
									hold_right.insert(make_pair(temp_id, tempnote->subid));
								}
								//store sub for hold-sub checking
								else if (tempnote->notetype == types::SUB) {
									sub_right.insert(temp_id);
								}
							}
							//duplicated
							else {
								snprintf(lln, sizeof(lln), "%d", tag_line);
								snprintf(id_string, sizeof(id_string), "%d", temp_id);

								throw std::logic_error("Error: Duplicated note id: " + string(id_string) + ".\nThe note begins at line " + string(lln) + " in the XML.");
							}
							break;
						default:
							delete tempnote;
							//need to set to NULL after deletion
							tempnote = NULL;
							//get error position
							char lln[64];
							snprintf(lln, sizeof(lln), "%d.", lines);

							throw std::logic_error("Read notes error at line " + string(lln));
						}
						delete tempnote;
						//need to set to NULL after deletion
						tempnote = NULL;
						note_reading = false;
					}
					else {
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read notes error: Syntax error when stop reading a note.\nOccured at line " + string(lln));
						return;
					}
				}
				else if (tag == "m_notesLeft") {//left notes end
					if (modes == 2)modes = 0;
					else {
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read notes error: Syntax error when stop reading left notes.\nOccured at line " + string(lln));
						return;
					}
				}
				else if (tag == "m_notesRight") {//right notes end
					if (modes == 3)modes = 0;
					else {
						//get error position
						char lln[64];
						snprintf(lln, sizeof(lln), "%d.", lines);

						throw std::logic_error("Read notes error: Syntax error when stop reading right notes.\nOccured at line " + string(lln));
						return;
					}
				}

				//finish parsing element
				break;
			}
			else if (t_buf.compare(buf_index, 4, "<!--") == 0) {
				//xml comment
				if (!parse_comment()) {
					//get error position
					char lln[64];
					snprintf(lln, sizeof(lln), "%d.", lines);

					throw std::logic_error("error parsing comment. The comment begins at line " + string(lln));
					return;
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
				//get error position
				char lln[64];
				snprintf(lln, sizeof(lln), "%d.", lines);

				throw std::logic_error("xml attribute error:" + key + "\nOccured at line " + string(lln));
				return;
			}
			buf_index++;//'='
			skip_space();
			try {
				string val = parse_elem_attr_val();
			}
			catch (exception& ex) {
				char lln[64];
				snprintf(lln, sizeof(lln), "%d.",tag_line);
				string errmsg = ex.what();

				throw std::logic_error(errmsg+" The xml tag begins at line "+string(lln));
				return;
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

//parsing the text of an xml tag
string chart_store::parse_elem_text() {

	int pos = buf_index;
	//escape character trigger
	bool escape_char = false;
	//text without comment
	string ret_s = "";
	//need to ignore all comments here

text_parse:
	pos = buf_index;
	while (t_buf[buf_index] != '<') {
		//rough detection of escape character
		if (escape_char) {
			if (isalnum((int)t_buf[buf_index]) == 0) {
				//get error position
				char lln[64];
				snprintf(lln, sizeof(lln), "%d.", lines);

				throw std::logic_error("Illegal usage of escape character in XML file at line " + string(lln));
			}
			else {
				buf_index++;
			}
		}

		//end of escape character
		if (t_buf[buf_index] == ';') {
			escape_char = false;
			buf_index++;
		}

		//illegal characters
		if (t_buf[buf_index] == '\'' || t_buf[buf_index] == '\"' || t_buf[buf_index] == '>') {
			//get error position
			char lln[64];
			snprintf(lln, sizeof(lln), "%d.", lines);

			throw std::logic_error("Illegal character in XML file at line " + string(lln));
		}
		else if (t_buf[buf_index] == '&') {//start of escape character
			if (escape_char) {
				//get error position
				char lln[64];
				snprintf(lln, sizeof(lln), "%d.", lines);

				throw std::logic_error("Illegal usage of escape character in XML file at line " + string(lln));
			}
			else {
				escape_char = true;
				buf_index++;
			}
		}
		else {	//normal characters
			if (t_buf[buf_index] == '\n') {
				lines++;
			}
			buf_index++;
		}

		//missing <
		if (buf_index >= t_buf.length()) {
			throw std::logic_error("Tag end not found.");
			//return "";
		}
	}

	//end of escape character not found
	if (escape_char) {
		//get error position
		char lln[64];
		snprintf(lln, sizeof(lln), "%d.", lines);

		throw std::logic_error("Illegal usage of escape character in XML file at line " + string(lln));
	}
	ret_s += t_buf.substr(pos, buf_index - pos);
	if (t_buf.compare(buf_index, 4, "<!--") == 0) {
		//collect text

		//parse xml comment
		if (!parse_comment()) {
			//get error position
			char lln[64];
			snprintf(lln, sizeof(lln), "%d.", lines);

			throw std::logic_error("error parsing comment. The comment begins at line " + string(lln));

		}
		else {
			//continue parsing text
			goto text_parse;
		}
	}

	return ret_s;
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
		//get error position
		char lln[64];
		snprintf(lln, sizeof(lln), "%d.", lines);

		throw std::logic_error("attribute value missing \"\" at line " + string(lln));
		return "";
	}
	buf_index++;//'"'
	int pos = buf_index;
	while (t_buf[buf_index] != '\"') {
		//read value
		buf_index++;
		//missing the right quotation mark
		if (buf_index >= t_buf.length()) {
			throw std::logic_error("attribute value not closed (missing quotation mark).");
			return "";
		}
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
