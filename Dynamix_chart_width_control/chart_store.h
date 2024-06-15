/**
* Creator:AXIS5
* This is the definition of class chart_store
* Note: this file must be the same in both CLI application and GUI application
*/
#ifndef CHART_STORE_H
#define CHART_STORE_H
#include<vector>
#include<string>
#include<sstream>
#include<climits>
#include<map>
#include<set>

using std::vector;
using std::string;
using std::ofstream;
using std::pair;
using std::map;
using std::set;

//added default note type: NULLTP
enum types { NORMAL = 1, CHAIN, HOLD, SUB, NULLTP };
//added default sides type: UNKNOWN
enum sides { PAD, MIXER, MULTI, UNKNOWN };
struct note {
	int id;
	types notetype;
	double time;
	double position;
	double width;
	int subid;
	//constructor
	note() :id(-1),
		notetype(types::NULLTP),
		time(-1e8),
		position(-1e8),
		width(-1e8),
		subid(INT_MIN) {}
};
class chart_store
{
public:
	//vector<note> m_notes, m_left, m_right;//note list(deprecated)

	//now we use map to store the notes, in order to deal with discrete note id
	//<note id,note>
	map<int, note> m_notes, m_left, m_right;//note list(the key is the note's id)
	//hold list and sub list(used for hold-sub checking)
	map<int, int> hold_mid, hold_left, hold_right;
	set<int> sub_mid, sub_left, sub_right;
	//mismatch note list(used for hold-sub autofix)
	vector<pair<int, string> > mismatched_notes;

	bool to_file(string f);//print to XML
	int readfile(string fn);//read XML
	string chart_filename;//filename of the chart
	void set_barpm(double f) { barpm = f; }
	void set_lside(sides x) { ltype = x; }
	void set_rside(sides x) { rtype = x; }

	//fetch note count
	int get_mid_count() { return m_notes.size(); }
	int get_left_count() { return m_left.size(); }
	int get_right_count() { return m_right.size(); }
	int get_tap_count() { return tap_count; }
	int get_chain_count() { return chain_count; }
	int get_hold_count() { return hold_count; }
private:

	string name;//song name
	string name_id;//mapID
	double offset;
	double barpm;//Bar per minute
	sides ltype, rtype;//left type and right type
	void side_out(const map<int, note>& v, ofstream& of);//output each side
	void clear();

	//xml parser members
	string t_buf;//the string buffer
	int buf_index;//the current index in the string buffer
	int modes;//0->none 1->middle 2->left 3->right
	note* tempnote;
	bool note_trigger;
	bool note_reading;//if reading a note
	int lines;//at which line in xml

	void skip_space();//skip the space when parsing
	bool parse_decl();//if declaration
	bool parse_comment();//if parsing comment
	void parse_elem();//start parsing element
	string parse_elem_name();//parsing element name
	string parse_elem_text();//parsing element text
	string parse_elem_attr_key();//parsing element attribute name
	string parse_elem_attr_val();//parsing element attribute value
	//note counter
	int tap_count;
	int chain_count;
	int hold_count;

};

#endif
