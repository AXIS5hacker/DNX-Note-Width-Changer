/*
* Creator:AXIS5
* This is the definition of class chart_store
*/
#ifndef CHART_STORE_H
#define CHART_STORE_H
#include<vector>
#include<string>
using namespace std;
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
	string name;//song name
	string name_id;//mapID
	double offset;
	double barpm;//Bar per minute
	sides ltype, rtype;//left type and right type
	vector<note> m_notes, m_left, m_right;//note list
	bool to_file(string f);
	int readfile(string fn);//read XML
};

#endif
