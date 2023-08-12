﻿#include "maingui.h"
#include "ui_maingui.h"
#include<QFileDialog>
#include"chart_store.h"
#include"defs.h"
#include<QMessageBox>
#include"../version.h"
using std::exception;
extern QString customfont;
MainGUI::MainGUI(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainGUI)
{
	ui->setupUi(this);
	translate_en();
	//connect the slider to the spinbox
	connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(change_multiplier(int)));
	connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(change_multiplier(int)));
	//Translations in menu
	connect(ui->actionChinese, SIGNAL(triggered()), this, SLOT(translate_cn()));
	connect(ui->actionEnglish_2, SIGNAL(triggered()), this, SLOT(translate_en()));
	ui->label_7->setFont(QFont(customfont, 18, 300));
}

//translate to Chinese
void MainGUI::translate_cn() {
	QTranslator cn_trans;
	cn_trans.load(":/translations/DNX_widthGUI_zh_CN.qm");
	QString tmptext = ui->loaded_file->text();
	if (!qApp->installTranslator(&cn_trans)) {
		QMessageBox::critical(this, "Error", "Translation not found");
	}
	ui->retranslateUi(this);
	ui->loaded_file->setText(tmptext);
	ui->version->setText(QString(VERSION_H));
}

//translate to English
void MainGUI::translate_en() {
	QTranslator en_trans;
	en_trans.load(":/translations/DNX_widthGUI_en_en.qm");
	QString tmptext = ui->loaded_file->text();
	if (!qApp->installTranslator(&en_trans)) {
		QMessageBox::critical(this, "Error", "Translation not found");
	}
	ui->retranslateUi(this);
	ui->loaded_file->setText(tmptext);
	ui->version->setText(QString(VERSION_H));
}

void MainGUI::on_exitButton_clicked() {
	this->close();
}

//The browse button
void MainGUI::on_browse_clicked() {
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Choose an XML chart file"),
		QDir::currentPath(), tr("XML Chart files (*.xml);;All files (*.*)"));
	ui->open_file_name->setText(filename);
}

//The load file button
void MainGUI::on_loadFile_clicked() {
	ui->listWidget->clear();
	if (ui->open_file_name->text() == "") {
		ui->loaded_file->setText("");
		//no file loaded or load error, lock all objects
		ui->TimeSpecify->setEnabled(false);
		ui->saveChart->setEnabled(false);
		ui->widthApply->setEnabled(false);
		ui->sideChoose->setEnabled(false);
		ui->horizontalSlider->setEnabled(false);
		ui->widthSpinBox->setEnabled(false);
		ui->widthMultiply->setEnabled(false);
		QMessageBox::critical(this, "Error", "Empty filename.");
		return;
	}
	try {
		cs.readfile(qstr2str_utf8(ui->open_file_name->text()));
		ui->loaded_file->setText(QString("Current Chart File: ") + ui->open_file_name->text());
	}
	catch (exception& ex) {
		ui->loaded_file->setText("");
		//no file loaded or load error, lock all objects
		ui->TimeSpecify->setEnabled(false);
		ui->saveChart->setEnabled(false);
		ui->widthApply->setEnabled(false);
		ui->sideChoose->setEnabled(false);
		ui->horizontalSlider->setEnabled(false);
		ui->widthSpinBox->setEnabled(false);
		ui->widthMultiply->setEnabled(false);
		QMessageBox::critical(this, "Error", ex.what());
		return;
	}

	ui->TimeSpecify->setEnabled(true);
	ui->saveChart->setEnabled(true);
	ui->widthApply->setEnabled(true);
	ui->sideChoose->setEnabled(true);
	ui->widthMultiply->setEnabled(true);
	ui->horizontalSlider->setEnabled(true);
	ui->widthSpinBox->setEnabled(true);
	//ui->loaded_file->adjustSize();
}

//slide bar: width change
void MainGUI::change_multiplier(int v) {
	ui->widthSpinBox->setValue(v / 100.0);
}

MainGUI::~MainGUI()
{
	delete ui;
}

//enable start time checkbox
void MainGUI::on_startCheck_clicked(bool checked)
{
	if (checked) {
		ui->stime->setEnabled(true);
	}
	else {
		ui->stime->setEnabled(false);
	}
}

//enable end time checkbox
void MainGUI::on_endCheck_clicked(bool checked)
{
	if (checked) {
		ui->etime->setEnabled(true);
	}
	else {
		ui->etime->setEnabled(false);
	}
}

void MainGUI::on_limitCheck_clicked(bool checked) {
	if (checked) {
		ui->widthSpinBox->setValue(1);
		ui->widthSpinBox->setMinimum(0.01);
		ui->widthSpinBox->setMaximum(30);
	}
	else {
		ui->widthSpinBox->setValue(1);
		ui->widthSpinBox->setMinimum(-999);
		ui->widthSpinBox->setMaximum(9999);
	}
};

void MainGUI::on_widthApply_clicked()
{
	double start_time = -1e9, end_time = 1e9;
	//get multiplier value
	double multiplier = ui->widthSpinBox->value();
	//get sides
	int sides = 0;
	if (ui->checkMiddle->isChecked()) {
		sides = sides | MID_CHANGE;
	}
	if (ui->checkLeft->isChecked()) {
		sides = sides | LEFT_CHANGE;
	}
	if (ui->checkRight->isChecked()) {
		sides = sides | RIGHT_CHANGE;
	}
	//get start time
	if (ui->startCheck->isChecked()) {
		start_time = ui->stime->value();
	}
	//get end time
	if (ui->endCheck->isChecked()) {
		end_time = ui->etime->value();
	}
	if (start_time >= end_time) {
		QMessageBox::critical(this, "Error", tr("Start time must be less than end time!"));
		return;
	}
	//apply change
	if (width_change(cs, multiplier, start_time, end_time, sides) != 0) {
		QMessageBox::critical(this, "Error", tr("Hold-Sub mismatch!"));
		return;
	}
	else {
		QMessageBox::information(this, "Done", tr("Width change applied!"));
	}
	//append the edit queue
	QString logtext = "";
	logtext += "width ";
	logtext = logtext + QString::number(multiplier * 100) + "%,";
	logtext = logtext + " sides: ";
	if (sides & MID_CHANGE) {
		logtext += "middle ";
	}
	if (sides & LEFT_CHANGE) {
		logtext += "left ";
	}
	if (sides & RIGHT_CHANGE) {
		logtext += "right ";
	}
	logtext += ", ";
	if (ui->startCheck->isChecked() || ui->endCheck->isChecked()) {
		logtext += "From ";
		if (ui->startCheck->isChecked()) {
			logtext += QString::number(start_time);
			logtext += " bar ";
		}
		else {
			logtext += "start ";
		}
		logtext += "to ";
		if (ui->endCheck->isChecked()) {
			logtext += QString::number(end_time);
			logtext += " bar ";
		}
		else {
			logtext += "end";
		}
	}
	else {
		logtext += "Changed Entire Chart";
	}
	ui->listWidget->addItem(logtext);
}

void MainGUI::on_saveChart_clicked()
{
	//set default output path
	QString out_filename = str2qstr_utf8(cs.chart_filename.substr(0, cs.chart_filename.length() - 4) + "_out.xml");
	QString savename = QFileDialog::getSaveFileName(this, tr("Save chart to"),
		out_filename, tr("XML Chart files (*.xml)"));
	cs.to_file(qstr2str_utf8(savename));
}