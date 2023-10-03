#include"defs.h"
using namespace std;


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

int width_change(chart_store& cs, double w, double st, double ed, int side_mask,int random) {
    /* The function that changes the width of a chart.
    *  cs:chart store object, w:width multiplier, st:start time, ed:end time
    *  side_mask: specify the sides to change
    *  random: random mode settings: 0 = no random, 1 = random mode 1(multiply), 2 = random mode 2(overwrite)
    */
    string buf;
    srand(time(NULL));
    int temp_sub = 0;//temporarily saves the id of a sub note

    double nw = 0, np = 0, t = 0;
    //bool ch_trigger = false;//check if note is in the specified time range

    map<int, note>::iterator it;
    //middle
    if (side_mask & MID_CHANGE) {
        for (it = cs.m_notes.begin(); it != cs.m_notes.end(); it++) {

            if ((it->second.notetype == NORMAL || it->second.notetype == CHAIN) &&
                it->second.time >= st && it->second.time <= ed) {

                nw = it->second.width;//read width
                np = it->second.position;//read position
                np += nw * 0.5;
                //randomized width mode 1
                if (random == 1) {
                    w = (rand() % 501 + 1) / 100.0;
                }
                //randomized width mode 2
                if (random == 2) {
                    nw = (rand() % 601 + 1) / 100.0;
                }
                else {
                    nw *= w;
                }
                np -= nw * 0.5;//multiply
                it->second.width = nw;
                it->second.position = np;
            }
            else if (it->second.notetype == HOLD &&
                it->second.time >= st && it->second.time <= ed) {//for hold notes

                nw = it->second.width;//read width
                np = it->second.position;//read position
                np += nw * 0.5;
                //randomized width mode 1
                if (random == 1) {
                    w = (rand() % 501 + 1) / 100.0;
                }
                //randomized width mode 2
                if (random == 2) {
                    nw = (rand() % 601 + 1) / 100.0;
                }
                else {
                    nw *= w;
                }
                np -= nw * 0.5;//multiply
                //writeback
                it->second.width = nw;
                it->second.position = np;

                int temp_sub = it->second.subid;//find its sub
                map<int, note>::iterator sub_it = cs.m_notes.find(temp_sub);//its sub note

                //sub not found
                if (sub_it == cs.m_notes.end() || sub_it->second.notetype != SUB) {
                    //cout << "Hold-sub mismatch!" << endl;
                    return 2;
                }
                else {//sub found
                    sub_it->second.width = it->second.width;
                    sub_it->second.position = it->second.position;
                }
            }
        }
    }
    //left
    if (side_mask & LEFT_CHANGE) {
        for (it = cs.m_left.begin(); it != cs.m_left.end(); it++) {

            if ((it->second.notetype == NORMAL || it->second.notetype == CHAIN) &&
                it->second.time >= st && it->second.time <= ed) {
                nw = it->second.width;//read width
                np = it->second.position;//read position
                np += nw * 0.5;
                //randomized width mode 1
                if (random == 1) {
                    w = (rand() % 501 + 1) / 100.0;
                }
                //randomized width mode 2
                if (random == 2) {
                    nw = (rand() % 601 + 1) / 100.0;
                }
                else {
                    nw *= w;
                }
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
                //randomized width mode 1
                if (random == 1) {
                    w = (rand() % 501 + 1) / 100.0;
                }
                //randomized width mode 2
                if (random == 2) {
                    nw = (rand() % 601 + 1) / 100.0;
                }
                else {
                    nw *= w;
                }
                np -= nw * 0.5;//multiply
                //writeback
                it->second.width = nw;
                it->second.position = np;

                int temp_sub = it->second.subid;//find its sub
                map<int, note>::iterator sub_it = cs.m_left.find(temp_sub);//its sub note

                //sub not found
                if (sub_it == cs.m_left.end() || sub_it->second.notetype != SUB) {
                    //cout << "Hold-sub mismatch!" << endl;
                    return 2;
                }
                else {//sub found
                    sub_it->second.width = it->second.width;
                    sub_it->second.position = it->second.position;
                }
            }
        }
    }
    //right
    if (side_mask & RIGHT_CHANGE) {
        for (it = cs.m_right.begin(); it != cs.m_right.end(); it++) {

            if ((it->second.notetype == NORMAL || it->second.notetype == CHAIN) &&
                it->second.time >= st && it->second.time <= ed) {
                nw = it->second.width;
                np = it->second.position;
                np += nw * 0.5;
                //randomized width mode 1
                if (random == 1) {
                    w = (rand() % 501 + 1) / 100.0;
                }
                //randomized width mode 2
                if (random == 2) {
                    nw = (rand() % 601 + 1) / 100.0;
                }
                else {
                    nw *= w;
                }
                np -= nw * 0.5;
                it->second.width = nw;
                it->second.position = np;
            }
            else if (it->second.notetype == HOLD &&
                it->second.time >= st && it->second.time <= ed) {//for hold notes
                nw = it->second.width;
                np = it->second.position;
                np += nw * 0.5;
                //randomized width mode 1
                if (random == 1) {
                    w = (rand() % 501 + 1) / 100.0;
                }
                //randomized width mode 2
                if (random == 2) {
                    nw = (rand() % 601 + 1) / 100.0;
                }
                else {
                    nw *= w;
                }
                np -= nw * 0.5;
                it->second.width = nw;
                it->second.position = np;

                int temp_sub = it->second.subid;//find its sub
                map<int, note>::iterator sub_it = cs.m_right.find(temp_sub);//its sub note

                //sub not found
                if (sub_it == cs.m_right.end() || sub_it->second.notetype != SUB) {
                    //cout << "Hold-sub mismatch!" << endl;
                    return 2;
                }
                else {//sub found
                    sub_it->second.width = it->second.width;
                    sub_it->second.position = it->second.position;
                }
            }
        }
    }

    return 0;
}
