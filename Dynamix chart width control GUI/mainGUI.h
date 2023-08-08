#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainGUI.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainGUIClass; };
QT_END_NAMESPACE

class mainGUI : public QMainWindow
{
    Q_OBJECT

public:
    mainGUI(QWidget *parent = nullptr);
    ~mainGUI();

private:
    Ui::mainGUIClass *ui;
};
