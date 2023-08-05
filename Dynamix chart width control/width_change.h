/**
* Creator:AXIS5
*/
// width change.h

#ifndef wid_R_H
#define wid_R_H

#include <iostream>
#include<fstream>
#include<cstdlib>
#include<sstream>
#include<vector>
#include<map>
#include<ctime>
#include<iomanip>

#include<sys/stat.h>
#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
#include<direct.h>
#include <io.h>
#else
#include<unistd.h>
#endif

#include<cstdio>
#define MID_CHANGE 0x4
#define LEFT_CHANGE 0x2
#define RIGHT_CHANGE 0x1

#include<string>
using std::string;
int width_change(string fn, double w, string _outf, double st, double ed, int side_mask);
#endif

