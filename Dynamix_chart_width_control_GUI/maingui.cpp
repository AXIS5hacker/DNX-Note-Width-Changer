#include "maingui.h"
#include "ui_maingui.h"
#include<QFileDialog>
#include"chart_store.h"
#include"qdefs.h"
#include<QMessageBox>
#include<QInputDialog>
#include"../version.h"
#include"hintdlg.h"
#include<sstream>
#include<ctime>
using std::exception;
using std::ofstream;
using std::ostringstream;
using std::endl;
extern QString customfont;
MainGUI::MainGUI(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainGUI)
{
	ui->setupUi(this);
	translator_loaded = false;
	//load translators
	cn_trans.load(":/translations/DNX_widthGUI_zh_CN.qm");
	en_trans.load(":/translations/DNX_widthGUI_en_en.qm");
	translate_en();
	//connect the slider to the spinbox
	connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(change_multiplier(int)));
	connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(change_multiplier(int)));
	//Translations in menu
	connect(ui->actionChinese, SIGNAL(triggered()), this, SLOT(translate_cn()));
	connect(ui->actionEnglish_2, SIGNAL(triggered()), this, SLOT(translate_en()));
	//connect the radio buttons to the width multiplier sections
	connect(ui->active_multiplier, SIGNAL(clicked()), this, SLOT(activate_multiplier()));
	connect(ui->active_randomizer1, SIGNAL(clicked()), this, SLOT(activate_multiplier()));
	connect(ui->active_randomizer2, SIGNAL(clicked()), this, SLOT(activate_multiplier()));
	ui->label_7->setFont(QFont(customfont, 18, 300));
	
	
}

//retranslate texts
void MainGUI::retranslate_text() {
	trans_1 = qApp->translate("MainGUI", "Hint", nullptr);
	trans_2 = qApp->translate("MainGUI", "This chart has Hold-Sub mismatch problems, and they have been automatically fixed.\nFor more info, see %1 for details.", nullptr);
	trans_3 = qApp->translate("MainGUI", "Chart auto repair complete, press save button to save it or use the width changing options to make further changes.", nullptr);
	trans_ftype = qApp->translate("MainGUI", "XML Chart files (*.xml);;All files (*.*)", nullptr);
	trans_choose = tr("Choose an XML chart file");
}

//translate to Chinese
void MainGUI::translate_cn() {
	QString tmptext = "";
	//need to uninstall the current translator before loading a new one
	if (translator_loaded) {
		qApp->removeTranslator(currentTranslator);
	}
	if (!qApp->installTranslator(&cn_trans)) {
		QMessageBox::critical(this, "Error", "Translation not found");
	}
	currentTranslator = &cn_trans;
	translator_loaded = true;
	ui->retranslateUi(this);
	if (cs.chart_filename != "") {
		tmptext = tr("Current Chart File: %1").arg(QString::fromStdString(cs.chart_filename));
	}
	//save translation text
	retranslate_text();
	//qDebug() << qApp->translate("MainGUI", "Chart auto repair complete, press save button to save it or use the width changing options to make further changes.", nullptr) << "cccc";
	ui->loaded_file->setText(tmptext);
	ui->version->setText(QString(VERSION_H));
}

//translate to English
void MainGUI::translate_en() {
	QString tmptext = "";
	//need to uninstall the current translator before loading a new one
	if (translator_loaded) {
		qApp->removeTranslator(currentTranslator);
	}
	if (!qApp->installTranslator(&en_trans)) {
		QMessageBox::critical(this, "Error", "Translation not found");
	}
	currentTranslator = &en_trans;
	translator_loaded = true;
	ui->retranslateUi(this);
	if (cs.chart_filename != "") {
		tmptext = (tr("Current Chart File: ")+str2qstr_utf8(cs.chart_filename));
	}
	//save translation text
	retranslate_text();
	ui->loaded_file->setText(tmptext);
	ui->version->setText(QString(VERSION_H));
}

void MainGUI::on_exitButton_clicked() {
	this->close();
}

//The browse button
void MainGUI::on_browse_clicked() {
	QString filename = QFileDialog::getOpenFileName(this,
		trans_choose,
		QDir::currentPath(), trans_ftype);
	ui->open_file_name->setText(filename);
}

//The load file button
void MainGUI::on_loadFile_clicked() {
	ui->listWidget->clear();
	//qDebug() << qApp->translate("MainGUI", "Chart auto repair complete, press save button to save it or use the width changing options to make further changes.", nullptr) << "cccc";
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
		ui->modeSelect->setEnabled(false);
		QMessageBox::critical(this, "Error", "Empty filename.");
		return;
	}
	try {
		int success = cs.readfile(qstr2str_utf8(ui->open_file_name->text()));
		//fixing stage
        bool fix_stage=false;
        if(success!=0){
            fix_stage=true;
        }
		//missing barpm
		if (success & BARPM_MISSING) {
			double new_barpm = 0;
			string ll;

			new_barpm = QInputDialog::getDouble(this,
				tr("Barpm fix"),
				tr("Illegal Barpm! Please enter a valid Barpm:"),
				1, 1, 2147483647.0, 6);



			cs.set_barpm(new_barpm);
			QMessageBox::warning(this, trans_1, tr("Barpm is set to %1").arg(new_barpm));
			success &= (~(int)BARPM_MISSING);
		}
		//missing left side
		if (success & LEFT_SIDE_MISSING) {
			sides s = sides::UNKNOWN;
			QString side_string;
			side_string = QInputDialog::getText(this,
				tr("Left side fix"),
				tr("Left side type is not specified! Please enter a valid side type")
			);
			//cout << "Left side type is not specified! Please enter a valid side type:" << endl;
			while (s == sides::UNKNOWN) {

				//lowercase
				side_string = side_string.toLower();
				if (side_string == "pad") {
					s = sides::PAD;
				}
				else if (side_string == "mixer") {
					s = sides::MIXER;
				}
				else if (side_string == "multi") {
					s = sides::MULTI;
				}
				else {
					side_string = QInputDialog::getText(this,
						tr("Left side fix"),
						tr("Invalid side type! Please enter again!")
					);

				}
			}
			cs.set_lside(s);
			success &= (~(int)LEFT_SIDE_MISSING);
		}
		//missing right side
		if (success & RIGHT_SIDE_MISSING) {
			sides s = sides::UNKNOWN;
			QString side_string;
			side_string = QInputDialog::getText(this,
				tr("Right side fix"),
				tr("Right side type is not specified! Please enter a valid side type")
			);
			//cout << "Left side type is not specified! Please enter a valid side type:" << endl;
			while (s == sides::UNKNOWN) {

				//lowercase
				side_string = side_string.toLower();
				if (side_string == "pad") {
					s = sides::PAD;
				}
				else if (side_string == "mixer") {
					s = sides::MIXER;
				}
				else if (side_string == "multi") {
					s = sides::MULTI;
				}
				else {
					side_string = QInputDialog::getText(this,
						tr("Right side fix"),
						tr("Invalid side type! Please enter again!")
					);

				}
			}
			cs.set_rside(s);
			success &= (~(int)RIGHT_SIDE_MISSING);
		}
		//Hold-sub mismatch autofix
		if (success & HOLD_SUB_MISMATCH) {
			//generate log
			QDir d = QDir::currentPath();
			if (!d.exists("logs")) {
				d.mkdir("logs");
				d.refresh();
			}
			d.cd("logs");
			time_t timer = time(NULL);
			struct tm *tblock = localtime(&timer);
			ostringstream ostr;
			string tarpath_string = qstr2str_utf8(d.path());
			ostr << tarpath_string<< "/" << "_" << tblock->tm_hour << "_" << tblock->tm_min << "_" << tblock->tm_sec << ".log";
			string fname = ostr.str();
			ofstream flog;
			bool log_avail = true;
			//create log
			flog.open(fname);
			if (flog.fail()) {
				log_avail = false;
				//QMessageBox::information(this, "info", str2qstr_utf8(fname));
			}
			if (log_avail) {
				flog << "mismatched notes found:\n\n";
				flog << "note_id\tside\ttime" << endl;
			}
			for (auto& ii : cs.mismatched_notes) {
				//fix
				if (ii.second == "middle") {
					if (log_avail) {
						flog << ii.first << '\t' << ii.second << '\t' << cs.m_notes[ii.first].time << endl;
					}
					cs.m_notes.erase(ii.first);
				}
				else if (ii.second == "left") {
					if (log_avail) {
						flog << ii.first << '\t' << ii.second << '\t' << cs.m_left[ii.first].time << endl;
					}
					cs.m_left.erase(ii.first);
				}
				else if (ii.second == "right") {
					if (log_avail) {
						flog << ii.first << '\t' << ii.second << '\t' << cs.m_right[ii.first].time << endl;
					}
					cs.m_right.erase(ii.first);
				}
			}
			flog << "The note(s) above is(are) automatically deleted." << endl;
			flog.close();
            dlg=new HintDlg(
				HintDlg::Warning,
				trans_1,
				trans_2.arg(str2qstr_utf8(fname)),
                HintDlg::Ok,this);
            dlg->setAttribute(Qt::WA_DeleteOnClose); //关掉消息框后删除指针

			dlg->exec();
            //QMessageBox::warning(this, "Hint", "This chart has Hold-Sub mismatch problems, and they have been automatically fixed.");
			//set status to success
			success &= (~(int)HOLD_SUB_MISMATCH);
		}
        if(fix_stage){
            dlg=new HintDlg(HintDlg::Warning,
				trans_1,
				trans_3,
                HintDlg::Ok,this);
			
			
            dlg->setAttribute(Qt::WA_DeleteOnClose); //关掉消息框后删除指针
			//qDebug() << qApp->translate("MainGUI", "Chart auto repair complete, press save button to save it or use the width changing options to make further changes.", nullptr) <<"cccc";
			dlg->exec();
        }
		if (success != 0) {
			throw std::logic_error("Unknown error");
		}
		ui->loaded_file->setText(tr("Current Chart File: ") + str2qstr_utf8(cs.chart_filename));
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
		ui->modeSelect->setEnabled(false);
		QMessageBox::critical(this, "Error", ex.what());
		return;
	}

	ui->TimeSpecify->setEnabled(true);
	ui->saveChart->setEnabled(true);
	ui->widthApply->setEnabled(true);
	ui->sideChoose->setEnabled(true);

	ui->modeSelect->setEnabled(true);

	//check if random mode or not
	if (ui->active_multiplier->isChecked()) {
		ui->widthMultiply->setEnabled(true);
		ui->horizontalSlider->setEnabled(true);
		ui->widthSpinBox->setEnabled(true);
	}
	else {
		ui->widthMultiply->setEnabled(false);
		ui->horizontalSlider->setEnabled(false);
		ui->widthSpinBox->setEnabled(false);
	}
	//ui->loaded_file->adjustSize();
}

//check if width multiplier mode
void MainGUI::activate_multiplier() {
	if (ui->active_multiplier->isChecked()) {
		ui->widthMultiply->setEnabled(true);
		ui->horizontalSlider->setEnabled(true);
		ui->widthSpinBox->setEnabled(true);
	}
	else {
		ui->widthMultiply->setEnabled(false);
		ui->horizontalSlider->setEnabled(false);
		ui->widthSpinBox->setEnabled(false);
	}
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
	//get random mode
	int random_mode = 0;
	//judge change mode
	if (ui->active_multiplier->isChecked()) {
		random_mode = 0;//width multiplier mode
	}
	else if (ui->active_randomizer1->isChecked()) {
		random_mode = 1;
	}
	else if (ui->active_randomizer2->isChecked()) {
		random_mode = 2;
	}
	//judge checked sides
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
	if (width_change(cs, multiplier, start_time, end_time, sides, random_mode) != 0) {
		QMessageBox::critical(this, "Error", tr("Hold-Sub mismatch!"));
		return;
	}
	else {
		QMessageBox::information(this, "Done", tr("Width change applied!"));
	}
	//append the edit queue
	QString logtext = "";
	//width change mode
	if (random_mode == 0) {
		logtext += "width ";
		logtext = logtext + QString::number(multiplier * 100) + "%,";
	}
	else if (random_mode == 1) {
		logtext += "randomized width mode 1,";
	}
	else if (random_mode == 2) {
		logtext += "randomized width mode 2,";
	}
	//sides
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
	//time
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
	//append the list
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
