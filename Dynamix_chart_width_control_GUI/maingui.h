#ifndef MAINGUI_H
#define MAINGUI_H

#include <QMainWindow>
#include"chart_store.h"
#include<qtranslator.h>
namespace Ui {
	class MainGUI;
}

class MainGUI : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainGUI(QWidget* parent = nullptr);
	~MainGUI();
private slots:

	void on_exitButton_clicked();//!<The exit button

	void on_browse_clicked();//!<The browse button

	void on_loadFile_clicked();//!<The load file button

	void change_multiplier(int);
	void on_startCheck_clicked(bool checked);

	void on_endCheck_clicked(bool checked);

	void on_widthApply_clicked();

	void on_saveChart_clicked();

	void on_limitCheck_clicked(bool checked);

	void activate_multiplier();

	void translate_cn();//!<apply Chinese translation

	void translate_en();//!<apply English translation
private:
	chart_store cs;
	Ui::MainGUI* ui;
};

#endif // MAINGUI_H
