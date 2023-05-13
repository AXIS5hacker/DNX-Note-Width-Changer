/**
* Creator:AXIS5
* This is the definition of class chart_store
*/
#ifndef CHART_STORE_H
#define CHART_STORE_H
#include<vector>
#include<string>
#include<sstream>
using std::vector;
using std::string;
using std::ofstream;

enum types { NORMAL = 1, CHAIN, HOLD, SUB };
enum sides { PAD, MIXER, MULTI };
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
	vector<note> m_notes, m_left, m_right;//note list
	bool to_file(string f);
	int readfile(string fn);//read XML
private:
	string name;//song name
	string name_id;//mapID
	double offset;
	double barpm;//Bar per minute
	sides ltype, rtype;//left type and right type
	void side_out(const vector<note>& v, ofstream& of);//output each side

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
