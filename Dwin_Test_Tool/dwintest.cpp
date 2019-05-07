#include "dwintest.h"
#include "ui_dwintest.h"
#include <QMessageBox>
#include <QDebug>

DwinTest::DwinTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DwinTest)
{
    ui->setupUi(this);

    createWidget();
    setComboBoxData();
}

DwinTest::~DwinTest()
{
    delete ui;
}

void DwinTest::slot_openComport(configPara para)
{
    //QString portName = ui->portNameComboBox->currentText();//获取串口名
    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    //if(!myCom)
    //{
        myCom = new Win_QextSerialPort(para.portName,QextSerialBase::EventDriven);
        //connect(myCom,SIGNAL(readyRead()),this,SLOT(slot_readMyCom()));
    //}

    if(!myCom->open(QIODevice::ReadWrite))  //以可读写方式打开串口
    {
        QMessageBox::warning(this,"warning","open serial failed!",QMessageBox::Ok);
        return;
    }
    //根据组合框内容对串口进行设置
    //设置波特率
    myCom->setBaudRate(para.porSetPara.BaudRate);

    //设置数据位
    myCom->setDataBits(para.porSetPara.DataBits);

    //设置奇偶校验
    myCom->setParity(para.porSetPara.Parity);

    //设置停止位
    myCom->setStopBits(para.porSetPara.StopBits);

    //设置数据流控制
    myCom->setFlowControl(FLOW_OFF);//设置数据流控制，我们使用无数据流控制的默认设置
    myCom->setTimeout(500);//设置延时

    //信号和槽函数关联，当串口缓冲区有数据时，进行读串口操作
    connect(myCom,SIGNAL(readyRead()),this,SLOT(slot_readMyCom()));
}

void DwinTest::slot_closeComPort()
{
    on_closeComPortBtn_clicked();
}

void DwinTest::createWidget()
{
    myCom = NULL;
    serialConDlg = new SerialConfigDialog(this);
    serialConDlg->close();
    connect(serialConDlg,SIGNAL(signal_comPara(configPara)),this,SLOT(slot_openComport(configPara)));
    connect(serialConDlg,SIGNAL(signal_closeComPort()),this,SLOT(slot_closeComPort()));
    connect(this,SIGNAL(signal_setWidgetStatus()),serialConDlg,SLOT(slot_setCloseComPortWidgetStatus()));
}

void DwinTest::setComboBoxData()
{
    ui->variableComboBox->setCurrentIndex(-1);
    ui->select_A_Z_ComboBox->setCurrentIndex(-1);
    ui->select_R_M_F_ComboBox->setCurrentIndex(-1);
}

QByteArray DwinTest::stringToHex(QString sendStr)
{
    for(int i=0; i<sendStr.length(); ++i)
    {
        if(QString(sendStr[i]) == " ")
            sendStr.remove(i,1);
    }
    QByteArray buf;
    if(sendStr.length()%2 != 0)
    {
        QMessageBox::warning(this,"warning","发送数据长度错误!",QMessageBox::Ok);
        return buf;     //此时buf为空，可用于调用函数判断
    }

    QString data;
    for(int i=0; i<sendStr.length()-1; i+=2)
    {
        data = sendStr.mid(i,2);
        buf.append(data.toInt(0,16));
    }
    return buf;
}

void DwinTest::on_openComPortBtn_clicked()
{
    serialConDlg->show();
}

void DwinTest::on_writeValueBtn_clicked()
{
    QString PKTHeadStr = "5AA5 05 82";
    QString addrStr = ui->select_R_M_F_ComboBox->currentData().toString();
    QString valueStr = ui->writeValueLineEdit->text();
    valueStr = QString("%1").arg(valueStr.toInt(),4,16,QChar('0'));
    QString sendStr = PKTHeadStr + addrStr + valueStr;

    QByteArray buf = stringToHex(sendStr);
    if(buf.isEmpty())
    {
        return ;
    }
    /*for(int i=0; i<sendStr.length(); ++i)
    {
        if(QString(sendStr[i]) == " ")
            sendStr.remove(i,1);
    }
    if(sendStr.length()%2 != 0)
    {
        QMessageBox::warning(this,"warning","发送数据长度错误!",QMessageBox::Ok);
        return;
    }
    QByteArray buf;
    QString data;
    for(int i=0; i<sendStr.length()-1; i+=2)
    {
        data = sendStr.mid(i,2);
        buf.append(data.toInt(0,16));
    }*/

    //以16进制发送
    myCom->write(buf);

    //将发送数据显示到“已发送数据”显示区
    ui->sendTextBrowser->insertPlainText(sendStr);
    ui->sendTextBrowser->insertPlainText("\r\n");
}

void DwinTest::on_switchPageIdBtn_clicked()
{
    QString PKTHeadStr = "5AA5 07 82 0084 5A01";
    QString pageIdStr = ui->targetPageIdLineEdit->text();
    pageIdStr = QString("%1").arg(pageIdStr.toInt(),4,16,QChar('0'));
    QString sendStr = PKTHeadStr + pageIdStr;

    QByteArray buf = stringToHex(sendStr);
    if(buf.isEmpty())
    {
        return ;
    }
    myCom->write(buf);
}

void DwinTest::slot_readMyCom()
{
    QByteArray received = myCom->readAll();
    received = received.toHex().toUpper();
    ui->receivedTextBrowser->insertPlainText(received);
    //ui->receivedTextBrowser->insertPlainText("\r\n");

    /*QByteArray temp = myCom->readAll();
    temp = temp.toHex().toUpper();
    ui->receivedTextBrowser->insertPlainText(temp);
    ui->receivedTextBrowser->insertPlainText("\n");*/

}

void DwinTest::on_closeComPortBtn_clicked()
{
    myCom->close();
    //发信号告知串口配置界面，设置相关部件可用或不可用
    emit signal_setWidgetStatus();
}

void DwinTest::on_variableComboBox_currentTextChanged(const QString &arg1)
{
    UIData data;
    if(arg1 == "U")
    {
        ui->select_A_Z_ComboBox->clear();
        ui->select_A_Z_ComboBox->addItems(data.UList);
        //ui->select_A_Z_ComboBox->addItem(data.UList.at(0),QVariant::fromValue(data.Ua));

        ui->select_A_Z_ComboBox->setItemData(0,QVariant::fromValue(data.Ua));
        ui->select_A_Z_ComboBox->setItemData(1,QVariant::fromValue(data.Ub));
        ui->select_A_Z_ComboBox->setItemData(2,QVariant::fromValue(data.Uc));
        ui->select_A_Z_ComboBox->setItemData(3,QVariant::fromValue(data.Ux));
        ui->select_A_Z_ComboBox->setItemData(4,QVariant::fromValue(data.Uy));
        ui->select_A_Z_ComboBox->setItemData(5,QVariant::fromValue(data.Uz));
        //qDebug()<<"data.Ua.size : "<<data.Ua.size();
    }
    else if(arg1 == "I")
    {
        ui->select_A_Z_ComboBox->clear();
        ui->select_A_Z_ComboBox->addItems(data.IList);
        ui->select_A_Z_ComboBox->setItemData(0,QVariant::fromValue(data.Ia));
        ui->select_A_Z_ComboBox->setItemData(1,QVariant::fromValue(data.Ib));
        ui->select_A_Z_ComboBox->setItemData(2,QVariant::fromValue(data.Ic));
        ui->select_A_Z_ComboBox->setItemData(3,QVariant::fromValue(data.Ix));
        ui->select_A_Z_ComboBox->setItemData(4,QVariant::fromValue(data.Iy));
        ui->select_A_Z_ComboBox->setItemData(5,QVariant::fromValue(data.Iz));
    }
}

void DwinTest::on_select_A_Z_ComboBox_currentTextChanged(const QString &arg1)
{
    //qDebug()<<"set rms ang fre addr";
    QString headStr = arg1;
    QMap<QString,QString> addrMap = ui->select_A_Z_ComboBox->currentData().value<QMap<QString,QString>>();
    QString rms = headStr + "_" + "rms";
    QString ang = headStr + "_" + "ang";
    QString fre = headStr + "_" + "fre";
    ui->select_R_M_F_ComboBox->setItemData(0,addrMap.value(rms));
    ui->select_R_M_F_ComboBox->setItemData(1,addrMap.value(ang));
    ui->select_R_M_F_ComboBox->setItemData(2,addrMap.value(fre));

    //qDebug()<<"set : "<<ui->select_R_M_F_ComboBox->currentData(Qt::UserRole).toString();
    //qDebug()<<"set : "<<ui->select_R_M_F_ComboBox->itemData(0,Qt::UserRole).toString();
    //qDebug()<<"map size : "<<addrMap.size();
    //qDebug()<<rms << "  "<<ang<<"  "<<fre<<endl;
}
