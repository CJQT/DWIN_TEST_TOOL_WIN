#include "serialconfigdialog.h"
#include "ui_serialconfigdialog.h"
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>

SerialConfigDialog::SerialConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialConfigDialog)
{
    ui->setupUi(this);

    updateComPort();
}

SerialConfigDialog::~SerialConfigDialog()
{
    delete ui;
}

void SerialConfigDialog::slot_setCloseComPortWidgetStatus()
{
    ui->portNameComboBox->setEnabled(true);
    ui->updateComPortBtn->setEnabled(true);
    ui->BaudRateComboBox->setEnabled(true);
    ui->dataBitsComboBox->setEnabled(true);
    ui->parityComboBox->setEnabled(true);
    ui->stopBitsComboBox->setEnabled(true);
    ui->openComPortBtn->setEnabled(true);
    ui->closeComPortBtn->setEnabled(false);
}

void SerialConfigDialog::on_openComPortBtn_clicked()
{
    if(ui->portNameComboBox->count() == 0)
    {
        QMessageBox::warning(this,"warning","没有可用的串口!请连接设备并点击“刷新串口”按钮",QMessageBox::Ok);
        return;
    }

    configPara configpara;
    configpara.portName = ui->portNameComboBox->currentText();

    //设置波特率
    if(ui->BaudRateComboBox->currentText() == tr("9600"))
        configpara.porSetPara.BaudRate = BAUD9600;
    else if(ui->BaudRateComboBox->currentText() == tr("115200"))
        configpara.porSetPara.BaudRate = BAUD115200;
    //设置数据位
    QString dataStr = ui->dataBitsComboBox->currentText();
    if(dataStr == tr("8"))
        configpara.porSetPara.DataBits = DATA_8;
    else if(dataStr == tr("7"))
        configpara.porSetPara.DataBits = DATA_7;
    //设置奇偶校验
    QString parStr = ui->parityComboBox->currentText();
    if(parStr == tr("无"))
        configpara.porSetPara.Parity = PAR_NONE;
    else if(parStr == tr("奇"))
        configpara.porSetPara.Parity = PAR_ODD;
    else if(parStr == tr("偶"))
        configpara.porSetPara.Parity = PAR_EVEN;
    //设置停止位
    QString stopStr = ui->stopBitsComboBox->currentText();
    if(stopStr == tr("1"))
        configpara.porSetPara.StopBits = STOP_1;
    else if(stopStr == tr("2"))
        configpara.porSetPara.StopBits = STOP_2;

    emit signal_comPara(configpara);

    this->close();

    //设置相关部件状态
    ui->portNameComboBox->setEnabled(false);
    ui->BaudRateComboBox->setEnabled(false);
    ui->updateComPortBtn->setEnabled(false);
    ui->dataBitsComboBox->setEnabled(false);
    ui->parityComboBox->setEnabled(false);
    ui->stopBitsComboBox->setEnabled(false);
    ui->openComPortBtn->setEnabled(false);
    ui->closeComPortBtn->setEnabled(true);
}

void SerialConfigDialog::on_updateComPortBtn_clicked()
{
    updateComPort();
}

void SerialConfigDialog::updateComPort()
{
    QStringList comPortList = getEnableComPortList();
    ui->portNameComboBox->clear();
    ui->portNameComboBox->addItems(comPortList);
}

QStringList SerialConfigDialog::getEnableComPortList()
{
    QStringList comPortList;

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            comPortList.append(serial.portName());
            serial.close();
        }
    }

    return comPortList;
}

void SerialConfigDialog::on_closeComPortBtn_clicked()
{
    emit signal_closeComPort();
}
