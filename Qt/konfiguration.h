#ifndef KONFIGURATION_H
#define KONFIGURATION_H
#include <QtSerialPort/QSerialPort>
#include <QDialog>
#include <QMessageBox>


//#include "mainwindow.cpp"


namespace Ui {
class konfiguration;
}

class konfiguration : public QDialog
{
    Q_OBJECT

public:
    explicit konfiguration(QSerialPort *Serial=0, QWidget *parent = nullptr);
    ~konfiguration();

private slots:
    //void on_pushButton_clicked();

    void on_pushButton_SendCBC_clicked();

private:
    Ui::konfiguration *ui;
    void VerifySerialPort();
};

#endif // KONFIGURATION_H
