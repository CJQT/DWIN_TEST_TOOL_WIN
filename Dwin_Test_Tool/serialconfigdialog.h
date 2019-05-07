#ifndef SERIALCONFIGDIALOG_H
#define SERIALCONFIGDIALOG_H

#include <QDialog>
#include "win_qextserialport.h"

struct configPara
{
    QString portName;
    PortSettings porSetPara;
};

namespace Ui {
class SerialConfigDialog;
}

class SerialConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialConfigDialog(QWidget *parent = 0);
    ~SerialConfigDialog();

signals:
    void signal_comPara(configPara para);
    void signal_closeComPort();

private slots:
    void slot_setCloseComPortWidgetStatus();

    void on_openComPortBtn_clicked();

    void on_updateComPortBtn_clicked();

    void on_closeComPortBtn_clicked();

private:
    void updateComPort();
    QStringList getEnableComPortList();
private:
    Ui::SerialConfigDialog *ui;

};

#endif // SERIALCONFIGDIALOG_H
