#ifndef DEFS_H
#define DEFS_H
/**
  * Some global definitions of the entire project (CLI & GUI program)
  * Author: AXIS5
  *
*/

#include<iomanip>
#include<fstream>
#include<algorithm>
#include<sys/stat.h>
#if defined(_WIN64)||defined(WIN32)||defined(_WIN32)
#include<direct.h>
#include <io.h>
#else
#include<unistd.h>
#endif

#include<cstdio>
//side change flags
#define MID_CHANGE 0x4
#define LEFT_CHANGE 0x2
#define RIGHT_CHANGE 0x1

//chart read flags
#define BARPM_MISSING 0x1
#define LEFT_SIDE_MISSING 0x2
#define RIGHT_SIDE_MISSING 0x4
#define HOLD_SUB_MISMATCH 0x8

#include "chart_store.h"
#include<string>
using std::string;
int width_change(chart_store& cs, double w, double st, double ed, int side_mask, int random);
#endif // DEFS_H
