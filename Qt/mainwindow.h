#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "QtSerialPort/qserialport.h"
#include "QtSerialPort/qserialportinfo.h"
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QFuture>
#include <QtConcurrent/QtConcurrent>
#include <QList>
#include "konfiguration.h"
#include <QTime>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QSerialPort *m_serial = nullptr;
    struct Settings
       {
           QString name;
           qint32 baudRate;
           QString stringBaudRate;
           QSerialPort::DataBits dataBits;
           QString stringDataBits;
           QSerialPort::Parity parity;
           QString stringParity;
           QSerialPort::StopBits stopBits;
           QString stringStopBits;
           QSerialPort::FlowControl flowControl;
           QString stringFlowControl;
           bool localEchoEnabled;

       };
    Settings m_currentSettings;
    explicit MainWindow (QWidget *parent=0);
     ~MainWindow();

private:
    Ui::MainWindow *ui;

    konfiguration *can_kfg;
    void updateSettings();
    void availableSerialUpdate();
    void connectionStatusUpdate();
private slots:
    void openSerialPort();
    void closeSerialPort();
    void readData();
    void fillPortsInfo();

    void on_pushButton_disconnect_clicked();
    void on_pushButton_connect_clicked();
    void on_pushButton_clear_console_clicked();
    void on_pushButton_send_command_clicked();
    void on_CAN_Konfig_clicked();
    void on_lineEdit_DLC_textChanged();
    //void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
