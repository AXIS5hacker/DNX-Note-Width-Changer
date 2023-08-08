#include "mainGUI.h"

mainGUI::mainGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::mainGUIClass())
{
    ui->setupUi(this);
}

mainGUI::~mainGUI()
{
    delete ui;
}
