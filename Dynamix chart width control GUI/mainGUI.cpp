#include "maingui.h"
#include "ui_maingui.h"

MainGUI::MainGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainGUI)
{
    ui->setupUi(this);
}
void MainGUI::on_exitButton_clicked(){
    this->close();
}

MainGUI::~MainGUI()
{
    delete ui;
}
