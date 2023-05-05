#include "konfiguration.h"
//#include "mainwindow.h"
#include "ui_konfiguration.h"

QSerialPort *SPort;
konfiguration::konfiguration(QSerialPort *Serial, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::konfiguration)
{
    ui->setupUi(this);

    this->setWindowTitle("CAN Configuration");

    ui->Pre->setValidator(new QIntValidator(1,1024,this));
    ui->mod->addItem(tr("CAN_MODE_NORMAL"));
    ui->mod->itemData(ui->mod->count()-1, 0) ;
    ui->mod->addItem(tr("CAN_MODE_LOOPBACK"));
    ui->mod->itemData(ui->mod->count()-1, 0) ;
    ui->mod->addItem(tr("CAN_MODE_SILENT"));
    ui->mod->itemData(ui->mod->count()-1, 0) ;
    ui->mod->addItem(tr("CAN_MODE_SILENT_LOOPBACK"));
    ui->mod->itemData(ui->mod->count()-1, 0) ;

    for (int i=1;i<5;i++){
        ui->sjw->addItem(QString::number(i)+tr(" Time Quantum"));
        ui->sjw->itemData(ui->sjw->count()-1, 0) ;
    }
    for (int i=1;i<17;i++){
        ui->TS1->addItem(QString::number(i)+tr(" Time Quantum"));
        ui->TS1->itemData(ui->TS1->count()-1, 0);
    }
    for (int i=1;i<9;i++){
        ui->TS2->addItem(QString::number(i)+tr(" Time Quantum"));
        ui->TS2->itemData(ui->TS2->count()-1, 0);
    }
    SPort=Serial;
    VerifySerialPort();
}

konfiguration::~konfiguration()
{
    delete ui;
}
void konfiguration::VerifySerialPort()
{
    if (SPort->isOpen()){
        ui->Label_Conn->setText(SPort->portName() + " is connected. ");
    } else {
        //QMessageBox::critical(this, tr("Error"), SPort->errorString());
        QMessageBox::critical(this, tr("Error"),"Serial Port is not open!!");
        this->hide();
    }
}

void konfiguration::on_pushButton_SendCBC_clicked()
{
    quint16 Prescaler=ui->Pre->text().toInt();
    //QString Mode=ui->mod->currentText();
    quint8 Mode=ui->mod->currentIndex();
    quint8 SJW=ui->sjw->currentText().leftRef(1).toUInt();
    quint8 TS1=ui->TS1->currentText().leftRef(1).toUInt();
    quint8 TS2=ui->TS2->currentText().leftRef(1).toUInt();
    QByteArray Config_Data;
    //Config_Data.resize(14);
    Config_Data.append(1); //Config flag
    Config_Data.append(Prescaler);
    Config_Data.append(Mode);
    Config_Data.append(SJW);
    Config_Data.append(TS1);
    Config_Data.append(TS2);
    Config_Data.resize(14);
    for (int i=6;i<14;i++)
        Config_Data[i]=0x00;
//d
    if (SPort->isOpen()){
        SPort->write(Config_Data.toStdString().data(),Config_Data.size());
        //ui->plainTextEdit_sent_commands->insertPlainText(QDate::currentDate().toString()+" "+QTime::currentTime().toString()+" transmited Data "+Test_Data.toHex(' ')+"\n");
    }else{
        //QMessageBox::critical(this, tr("Error"), m_serial->errorString());
        QMessageBox::critical(this, tr("Error"),"Serial Port is not open!!");
    }
    //
}

