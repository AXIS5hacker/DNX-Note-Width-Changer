/**
  *This project is compiled with QT 6.2.4
  *C++ standard: c++17
  *You need to add argument /Zc:__cplusplus to compile this project using Visual Studio 2019
  *Use MFC static libraries,and use /MT in running libraries in Visual 2019
  *In Qt Creator, the qmake command needed is "QMAKE_CXXFLAGS += -std:c++17 -Zc:__cplusplus"
  */
#include "maingui.h"

#include <QApplication>
#include <QFont>
#include <QFontDatabase>
QString customfont;
string qstr2str_utf8(const QString qstr) {
	/**
	 * Author:AXIS5
	 * QString->string
	 * input:QString
	 * output:string(utf8)
	 */
	QByteArray cdata = qstr.toLocal8Bit();
	return string(cdata);
}

QString str2qstr_utf8(const string str) {
	/**
	 * Author:AXIS5
	 * string->QString
	 * input:string(utf8)
	 * output:QString
	 */
	return QString::fromLocal8Bit(str.data());
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	int fid = QFontDatabase::addApplicationFont(":/font/Dynamix.ttf");

	if (fid != -1)customfont = QFontDatabase::applicationFontFamilies(fid).at(0);
	MainGUI w;

	w.show();
	return a.exec();
}
