/**
  *This project is compiled with QT 6.2.4
  *C++ standard: c++17
  *You need to add argument /Zc:__cplusplus to compile this project using Visual Studio 2019
  */
#include "maingui.h"

#include <QApplication>

string qstr2str_utf8(const QString qstr) {
	/**
	 * Author:AXIS5
	 * QString->string
	 * input:QString
	 * output:string(utf8)
	 */
	QByteArray cdata = qstr.toUtf8();
	return string(cdata);
}

QString str2qstr_utf8(const string str) {
	/**
	 * Author:AXIS5
	 * string->QString
	 * input:string(utf8)
	 * output:QString
	 */
	return QString::fromUtf8(str.data());
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	MainGUI w;
	w.show();
	return a.exec();
}
