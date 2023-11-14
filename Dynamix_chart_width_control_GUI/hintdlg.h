#ifndef HINTDLG_H
#define HINTDLG_H

#include <QMessageBox>
#include <QObject>
#include <QWidget>

class HintDlg : public QMessageBox
{
    Q_OBJECT
public:
    HintDlg(Icon icon, const QString& title, const QString& text,
                     StandardButtons buttons, QWidget* parent = 0);
    ~HintDlg();

};

#endif // HINTDLG_H
