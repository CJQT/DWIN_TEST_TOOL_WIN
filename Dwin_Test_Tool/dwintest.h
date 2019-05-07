#ifndef DWINTEST_H
#define DWINTEST_H

#include <QMainWindow>
#include "serialconfigdialog.h"
#include <QMap>

struct UIData
{
    QStringList UList{"Ua","Ub","Uc","Ux","Uy","Uz"};
    QStringList IList{"Ia","Ib","Ic","Ix","Iy","Iz"};
    QMap<QString,QString> Ua{{"Ua_rms","1000"},{"Ua_ang","1002"},{"Ua_fre","1004"}};
    QMap<QString,QString> Ub{{"Ub_rms","1006"},{"Ub_ang","1008"},{"Ub_fre","100A"}};
    QMap<QString,QString> Uc{{"Uc_rms","100C"},{"Uc_ang","100E"},{"Uc_fre","1010"}};
    QMap<QString,QString> Ux{{"Ux_rms","1012"},{"Ux_ang","1014"},{"Ux_fre","1016"}};
    QMap<QString,QString> Uy{{"Uy_rms","1018"},{"Uy_ang","101A"},{"Uy_fre","101C"}};
    QMap<QString,QString> Uz{{"Uz_rms","101E"},{"Uz_ang","1020"},{"Uz_fre","1022"}};
    QMap<QString,QString> Ia{{"Ia_rms","1024"},{"Ia_ang","1026"},{"Ia_fre","1028"}};
    QMap<QString,QString> Ib{{"Ib_rms","102A"},{"Ib_ang","102C"},{"Ib_fre","102E"}};
    QMap<QString,QString> Ic{{"Ic_rms","1030"},{"Ic_ang","1032"},{"Ic_fre","1034"}};
    QMap<QString,QString> Ix{{"Ix_rms","1036"},{"Ix_ang","1038"},{"Ix_fre","103A"}};
    QMap<QString,QString> Iy{{"Iy_rms","103C"},{"Iy_ang","103E"},{"Iy_fre","1040"}};
    QMap<QString,QString> Iz{{"Iz_rms","1042"},{"Iz_ang","1044"},{"Iz_fre","1046"}};
};

namespace Ui {
class DwinTest;
}

class DwinTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit DwinTest(QWidget *parent = 0);
    ~DwinTest();

signals:
    void signal_setWidgetStatus();
private slots:
    void slot_openComport(configPara para);
    void slot_closeComPort();
    void slot_readMyCom();
    void on_openComPortBtn_clicked();

    void on_writeValueBtn_clicked();

    void on_switchPageIdBtn_clicked();


    void on_closeComPortBtn_clicked();

    void on_variableComboBox_currentTextChanged(const QString &arg1);

    void on_select_A_Z_ComboBox_currentTextChanged(const QString &arg1);

private:
    void createWidget();
    void setComboBoxData();

    QByteArray stringToHex(QString sendStr);
private:
    Ui::DwinTest *ui;

    SerialConfigDialog *serialConDlg;
    Win_QextSerialPort *myCom;
};

#endif // DWINTEST_H
