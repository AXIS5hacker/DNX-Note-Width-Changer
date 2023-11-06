/**
* Creator:AXIS5
* This is the definition of class chart_store
*/
#ifndef CHART_STORE_H
#define CHART_STORE_H
#include<vector>
#include<string>
#include<sstream>
#include<map>

using std::vector;
using std::string;
using std::ofstream;
using std::pair;
using std::map;

enum types { NORMAL = 1, CHAIN, HOLD, SUB };
//added default sides type: UNKNOWN
enum sides { PAD, MIXER, MULTI, UNKNOWN };
struct note {
	int id;
	types notetype;
	double time;
	double position;
	double width;
	int subid;
};
class chart_store
{
public:
	//vector<note> m_notes, m_left, m_right;//note list(deprecated)

	//now we use map to store the notes, in order to deal with discrete note id
	//<note id,note>
	map<int, note> m_notes, m_left, m_right;//note list(the key is the note's id)

	bool to_file(string f);//print to XML
	int readfile(string fn);//read XML
	string chart_filename;//filename of the chart
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

	void skip_space();//skip the space when parsing
	bool parse_decl();//if declaration
	bool parse_comment();//if parsing comment
	void parse_elem();//start parsing element
	string parse_elem_name();//parsing element name
	string parse_elem_text();//parsing element text
	string parse_elem_attr_key();//parsing element attribute name
	string parse_elem_attr_val();//parsing element attribute value


};

#endif
