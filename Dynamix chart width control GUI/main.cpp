#include "mainGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainGUI w;
    w.show();
    return a.exec();
}
