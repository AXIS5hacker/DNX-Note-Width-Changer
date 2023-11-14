#include "hintdlg.h"


HintDlg::HintDlg(Icon icon, const QString& title, const QString& text,
                           StandardButtons buttons, QWidget* parent) :
    QMessageBox(icon, title, text, buttons,parent) {
}

HintDlg::~HintDlg() {

}
