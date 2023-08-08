#ifndef MAINGUI_H
#define MAINGUI_H

#include <QMainWindow>

namespace Ui {
class MainGUI;
}

class MainGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainGUI(QWidget *parent = nullptr);
    ~MainGUI();
private slots:
    void on_exitButton_clicked();
private:
    Ui::MainGUI *ui;
};

#endif // MAINGUI_H
