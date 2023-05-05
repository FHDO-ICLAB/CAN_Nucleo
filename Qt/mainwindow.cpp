#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
//#include "QtSerialPort/qserialport.h"


quint8 DLC;
//bool Test=false;
quint8 DLC_Old;
QString description;
QString manufacturer;
QString serialNumber;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,m_serial(new QSerialPort(this)),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("QSerialport");
    // this->m_serial = new QSerialPort(this);
       connect(this->m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
        //ui->comboBox_port_name->event
       //allows only Hex in the LineEdit

       ui->lineEdit_ID->setValidator(new QRegExpValidator(QRegExp("0x[0-9a-fA-F][0-9a-fA-F]"),this));
       ui->lineEdit_DLC->setValidator(new QRegExpValidator(QRegExp("[1-8]"),this));
       ui->lineEdit_D0->setValidator(new QRegExpValidator(QRegExp("0x[0-9a-fA-F][0-9a-fA-F]"),this));
       ui->lineEdit_D1->setValidator(new QRegExpValidator(QRegExp("0x[0-9a-fA-F][0-9a-fA-F]"),this));
       ui->lineEdit_D2->setValidator(new QRegExpValidator(QRegExp("0x[0-9a-fA-F][0-9a-fA-F]"),this));
       ui->lineEdit_D3->setValidator(new QRegExpValidator(QRegExp("0x[0-9a-fA-F][0-9a-fA-F]"),this));
       ui->lineEdit_D4->setValidator(new QRegExpValidator(QRegExp("0x[0-9a-fA-F][0-9a-fA-F]"),this));
       ui->lineEdit_D5->setValidator(new QRegExpValidator(QRegExp("0x[0-9a-fA-F][0-9a-fA-F]"),this));
       ui->lineEdit_D6->setValidator(new QRegExpValidator(QRegExp("0x[0-9a-fA-F][0-9a-fA-F]"),this));
       ui->lineEdit_D7->setValidator(new QRegExpValidator(QRegExp("0x[0-9a-fA-F][0-9a-fA-F]"),this));

       //ui->comboBox_port_name->addItem(QStringLiteral("COM_Test"));
       ui->comboBox_baud_rate->addItem(QStringLiteral("9600"));
       ui->comboBox_baud_rate->itemData(ui->comboBox_baud_rate->count()-1, 0) = QSerialPort::Baud9600;
       ui->comboBox_baud_rate->addItem(QStringLiteral("19200"));
       ui->comboBox_baud_rate->itemData(ui->comboBox_baud_rate->count()-1, 0) = QSerialPort::Baud19200;
       ui->comboBox_baud_rate->addItem(QStringLiteral("38400"));
       ui->comboBox_baud_rate->itemData(ui->comboBox_baud_rate->count()-1, 0) = QSerialPort::Baud38400;
       ui->comboBox_baud_rate->addItem(QStringLiteral("115200"));
       ui->comboBox_baud_rate->itemData(ui->comboBox_baud_rate->count()-1, 0) = QSerialPort::Baud115200;
       ui->comboBox_baud_rate->setCurrentIndex(3);


       ui->comboBox_data_bits->addItem(QStringLiteral("5"));
       ui->comboBox_data_bits->itemData(ui->comboBox_data_bits->count()-1, 0) = QSerialPort::Data5;
       ui->comboBox_data_bits->addItem(QStringLiteral("6"));
       ui->comboBox_data_bits->itemData(ui->comboBox_data_bits->count()-1, 0) = QSerialPort::Data6;
       ui->comboBox_data_bits->addItem(QStringLiteral("7"));
       ui->comboBox_data_bits->itemData(ui->comboBox_data_bits->count()-1, 0) = QSerialPort::Data7;
       ui->comboBox_data_bits->addItem(QStringLiteral("8"));
       ui->comboBox_data_bits->itemData(ui->comboBox_data_bits->count()-1, 0) = QSerialPort::Data8;
       ui->comboBox_data_bits->setCurrentIndex(3);


       ui->comboBox_parity_bit->addItem(tr("None"));
       ui->comboBox_parity_bit->itemData(ui->comboBox_parity_bit->count()-1, 0) = QSerialPort::NoParity;
       ui->comboBox_parity_bit->addItem(tr("Even"));
       ui->comboBox_parity_bit->itemData(ui->comboBox_parity_bit->count()-1, 0) = QSerialPort::EvenParity;
       ui->comboBox_parity_bit->addItem(tr("Odd"));
       ui->comboBox_parity_bit->itemData(ui->comboBox_parity_bit->count()-1, 0) = QSerialPort::OddParity;
       ui->comboBox_parity_bit->addItem(tr("Mark"));
       ui->comboBox_parity_bit->itemData(ui->comboBox_parity_bit->count()-1, 0) = QSerialPort::MarkParity;
       ui->comboBox_parity_bit->addItem(tr("Space"));
       ui->comboBox_parity_bit->itemData(ui->comboBox_parity_bit->count()-1, 0) = QSerialPort::SpaceParity;
       ui->comboBox_parity_bit->setCurrentIndex(0);


       ui->comboBox_stop_bit->addItem(QStringLiteral("1"));
       ui->comboBox_stop_bit->itemData(ui->comboBox_stop_bit->count()-1, 0) = QSerialPort::OneStop;
       ui->comboBox_stop_bit->addItem(tr("1.5"));
       ui->comboBox_stop_bit->itemData(ui->comboBox_stop_bit->count()-1, 0) = QSerialPort::OneAndHalfStop;
       ui->comboBox_stop_bit->addItem(QStringLiteral("2"));
       ui->comboBox_stop_bit->itemData(ui->comboBox_stop_bit->count()-1, 0) = QSerialPort::TwoStop;
       ui->comboBox_stop_bit->setCurrentIndex(0);

       ui->comboBox_flow_control->addItem(tr("None"));
       ui->comboBox_flow_control->itemData(ui->comboBox_flow_control->count()-1, 0) = QSerialPort::NoFlowControl;
       ui->comboBox_flow_control->addItem(tr("RTS/CTS"));
       ui->comboBox_flow_control->itemData(ui->comboBox_flow_control->count()-1, 0) = QSerialPort::HardwareControl;
       ui->comboBox_flow_control->addItem(tr("XON/XOFF"));
       ui->comboBox_flow_control->itemData(ui->comboBox_flow_control->count()-1, 0) = QSerialPort::SoftwareControl;
       ui->comboBox_flow_control->setCurrentIndex(0);

       ui->rtr->addItem(QString("CAN_RTR_DATA"));
       ui->rtr->itemData(ui->rtr->count()-1, 0) ;
       ui->rtr->addItem(QString("CAN_RTR_REMOTE"));
       ui->rtr->itemData(ui->rtr->count()-1, 0) ;

       ui->TGT->addItem(tr("ENABLE"));
       ui->TGT->itemData(ui->TGT->count()-1, 0) ;
       ui->TGT->addItem(tr("DISABLE"));
       ui->TGT->itemData(ui->TGT->count()-1, 0) ;

       ui->ide->addItem(tr("CAN_ID_STD"));
       ui->ide->itemData(ui->ide->count()-1, 0) ;
       ui->ide->addItem(tr("CAN_ID_EXT"));
       ui->ide->itemData(ui->ide->count()-1, 0) ;


       fillPortsInfo();

       //updateSettings();

       QFuture<void> serial_update_future = QtConcurrent::run(this, &MainWindow::availableSerialUpdate);
       //QFuture<void> connection_update_future = QtConcurrent::run(this, &MainWindow::connectionStatusUpdate);



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openSerialPort()
{
    //closeSerialPort();
       updateSettings();
       this->m_serial->setPortName(this->m_currentSettings.name);
       this->m_serial->setBaudRate(this->m_currentSettings.baudRate);
       this->m_serial->setDataBits(this->m_currentSettings.dataBits);
       this->m_serial->setParity(this->m_currentSettings.parity);
       this->m_serial->setStopBits(this->m_currentSettings.stopBits);
       this->m_serial->setFlowControl(this->m_currentSettings.flowControl);

       //qDebug() << "port name: " << this->m_currentSettings.name;
       //qDebug() << "baud rate: " << this->m_currentSettings.baudRate;
       //qDebug() << "data bits: " << this->m_currentSettings.dataBits;
       //qDebug() << "parity: " <<  this->m_currentSettings.parity;
       //qDebug() << "stop bits: " << this->m_currentSettings.stopBits;
       //qDebug() << "flow control: " << this->m_currentSettings.flowControl;
       ui->plainTextEdit_sent_commands->insertPlainText(QDate::currentDate().toString()+" "+QTime::currentTime().toString()+ " " + m_serial->portName() + " Connected"+"\n");
       ui->plainTextEdit_sent_commands->insertPlainText("Manufacturer: "+ description + " "+serialNumber+"\n");

       if(!m_serial->open(QIODevice::ReadWrite))
       {
           //QMessageBox::critical(this, tr("Error"), m_serial->errorString());
           if (m_serial->isOpen()){
               ui->label_connection_status_value->setText(m_serial->portName() + " is already connected. ");

           } else {
               QMessageBox::critical(this, tr("Error"), m_serial->errorString());
           }
           //qDebug() << "Could not open serial port...";
           //ui->label_connection_status_value->setText(" Could not open serial port... ");
       }
       else
       {
           m_serial->flush();
           ui->label_connection_status_value->setText(m_serial->portName() + " Connected ");
           //qDebug() << "Opened Serial port!";
       }
}

void MainWindow::connectionStatusUpdate()
{
    while(1)
       {
           if(m_serial->isOpen() && m_serial->isReadable())
           {
               ui->label_connection_status_value->setText(m_serial->portName() + " Connected");
               qDebug() << "Serial port open!";
           }
           else
           {
               ui->label_connection_status_value->setText(m_serial->portName() + " Not Connected");
               qDebug() << "Serial port appears to not have opened.";
           }
       }
    }
void MainWindow::updateSettings()
{
    this->m_currentSettings.name = ui->comboBox_port_name->itemData(ui->comboBox_port_name->currentIndex(), 0).toString();

    m_currentSettings.baudRate = ui->comboBox_baud_rate->itemData(ui->comboBox_baud_rate->currentIndex(), 0).toInt();
    m_currentSettings.stringBaudRate = QString::number(m_currentSettings.baudRate);

    m_currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->comboBox_data_bits->itemData(ui->comboBox_data_bits->currentIndex(), 0).toInt());
    m_currentSettings.stringDataBits =  ui->comboBox_data_bits->itemData(ui->comboBox_data_bits->currentIndex(), 0).toString();

    m_currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->comboBox_parity_bit->itemData((ui->comboBox_parity_bit->currentIndex(),0)).toInt());
    m_currentSettings.stringParity = ui->comboBox_parity_bit->itemData(ui->comboBox_parity_bit->currentIndex(), 0).toString();

    m_currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->comboBox_stop_bit->itemData((ui->comboBox_stop_bit->currentIndex(),0)).toInt());
    m_currentSettings.stringStopBits =  ui->comboBox_stop_bit->itemData(ui->comboBox_stop_bit->currentIndex(), 0).toString();

    m_currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->comboBox_flow_control->itemData((ui->comboBox_flow_control->currentIndex(),0)).toInt());
    m_currentSettings.stringFlowControl = ui->comboBox_flow_control->itemData(ui->comboBox_flow_control->currentIndex(), 0).toString();
}

void MainWindow::fillPortsInfo()
{
    //ui->comboBox_port_name->clear();

    //QString description;
    //QString manufacturer;
    //QString serialNumber;
    QList<QString> port_name_list;
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
    {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : "")
             << (!manufacturer.isEmpty() ? manufacturer : "")
             << (!serialNumber.isEmpty() ? serialNumber : "")
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : "")
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : "");

        port_name_list.append(info.systemLocation());
    }
    int row;
       QString currentSelection;
       currentSelection = ui->comboBox_port_name->currentText();

       ui->comboBox_port_name->clear();
       for(int i = 0; i < port_name_list.count(); i++)
       {
           row = ui->comboBox_port_name->findText(port_name_list.at(i));
           if(row < 0)
           {
               ui->comboBox_port_name->addItem(port_name_list.at(i));
           }
       }

       row = ui->comboBox_port_name->findText(currentSelection);
       if(row >= 0)
       {
           ui->comboBox_port_name->setCurrentIndex(row);
       }

   }
void MainWindow:: availableSerialUpdate()
{
    while(1)
    {
        fillPortsInfo();
        QThread::sleep(1);
    }
}
void MainWindow::readData()
{
     const QByteArray RData = m_serial->readAll();
     ui->plainTextEdit_sent_commands->insertPlainText(QDate::currentDate().toString()+" "+QTime::currentTime().toString()+" received data: "+RData.toHex(' ')+"\n");//QString::fromLocal8Bit(data.data()
}
void MainWindow::closeSerialPort()
{
    if(m_serial->isOpen())
    {
        m_serial->close();
        ui->label_connection_status_value->setText("Disconnected");
        ui->plainTextEdit_sent_commands->insertPlainText(QDate::currentDate().toString()+" "+QTime::currentTime().toString()+ " " + m_serial->portName() + " Disconnected"+"\n");
        //qDebug() << "closeSerialPort(): Closed opened connection...";
    }
    else
    {
        //qDebug() << "closeSerialPort(): No connection to close...";
        ui->label_connection_status_value->setText("No connection to close...");
    }
}
void MainWindow::on_pushButton_connect_clicked()
{
    openSerialPort();
    //ui->label_connection_status_value->setText("Connected");
}

void MainWindow::on_pushButton_disconnect_clicked()
{
    closeSerialPort();
}

void MainWindow::on_pushButton_clear_console_clicked()
{
    ui->plainTextEdit_sent_commands->clear();
}

void MainWindow::on_pushButton_send_command_clicked()
{

    QString ID = ui->lineEdit_ID->text().mid(2);//.toLatin1().toHex();
    QString D0 = ui->lineEdit_D0->text().mid(2);//.toLatin1().toHex();
    QString D1 = ui->lineEdit_D1->text().mid(2);//.toLatin1().toHex();
    QString D2 = ui->lineEdit_D2->text().mid(2);//.toLatin1().toHex();
    QString D3 = ui->lineEdit_D3->text().mid(2);//.toLatin1().toHex();
    QString D4 = ui->lineEdit_D4->text().mid(2);//.toLatin1().toHex();
    QString D5 = ui->lineEdit_D5->text().mid(2);//.toLatin1().toHex();
    QString D6 = ui->lineEdit_D6->text().mid(2);//.toLatin1().toHex();
    QString D7 = ui->lineEdit_D7->text().mid(2);//.toLatin1().toHex();
    QString RTR=ui->rtr->currentText();//=="CAN_RTR_DATA")?RTR=1:RTR=0;//QString RTR = ui->rtr->currentText() ;
    QString TransGlobalTime=ui->TGT->currentText();//=="ENABLE")?TransGlobalTime=1:TransGlobalTime=0;//QString TransGlobalTime = ui->TGT->currentText() ;
    QString IDE = ui->ide->currentText();//=="CAN_ID_STD")?IDE=1:IDE=0; ;//QString IDE = ui->ide->currentText() ;
    quint8 RTR1=(RTR=="CAN_RTR_DATA")?1:0;
    quint8 TRT1=(TransGlobalTime=="ENABLE")?1:0;
    quint8 IDE1=(IDE=="CAN_ID_STD")?1:0;
    QByteArray Data_Frame;
    //Test_Data.resize(DLC);
    //char cmd[8];
    //cmd[0]=ui->lineEdit_D0->text().mid(2);
    Data_Frame.append(2);//Data Frame Flag
    Data_Frame.append(QByteArray::fromHex(ID.toLatin1()));
    Data_Frame.append(DLC);
    Data_Frame.append(QByteArray::fromHex(D0.toLatin1()));
    Data_Frame.append(QByteArray::fromHex(D1.toLatin1()));
    Data_Frame.append(QByteArray::fromHex(D2.toLatin1()));
    Data_Frame.append(QByteArray::fromHex(D3.toLatin1()));
    Data_Frame.append(QByteArray::fromHex(D4.toLatin1()));
    Data_Frame.append(QByteArray::fromHex(D5.toLatin1()));
    Data_Frame.append(QByteArray::fromHex(D6.toLatin1()));
    Data_Frame.append(QByteArray::fromHex(D7.toLatin1()));
    //int k=(8-DLC)+Test_Data.size();
    //Test_Data.resize(k);
    //for (int i=DLC+2;i<k;i++)
    //    Test_Data[i]=0x00;
    if (Data_Frame.size()!=11)
        Data_Frame.resize(11);
    for (int i=DLC+3;i<11;i++)
        Data_Frame[i]=0x00;
    QByteArray DataField;
    DataField.resize(8);
    for (int j=0;j<8;j++)
        DataField[j]=Data_Frame[j+3];
    Data_Frame.append(RTR1);
    Data_Frame.append(TRT1);
    Data_Frame.append(IDE1);
    if (m_serial->isOpen()){
        m_serial->write(Data_Frame.toStdString().data(),Data_Frame.size());
        ui->plainTextEdit_sent_commands->insertPlainText(QDate::currentDate().toString()+" "+QTime::currentTime().toString()+" transmited Data "+DataField.toHex(' ')+"\n"); //Data_Frame.toHex(' ') D0+" "+D1+" "+D2+" "+D3+" "+D4+" "+D5+" "+D6+" "+D7
    }else{
        //QMessageBox::critical(this, tr("Error"), m_serial->errorString());
        QMessageBox::critical(this, tr("Error"),"Serial Port is not open!!");
    }
    //m_serial->write(Test_Data.toStdString().data(),Test_Data.size());
    //ui->plainTextEdit_sent_commands->insertPlainText(QDate::currentDate().toString()+" "+QTime::currentTime().toString()+" transmited Data "+D0+D1+D2+D3+D4+D5+D6+D7+"\n");
    //qDebug() << "Writing the following data: " << QDate::currentDate().toString()+" "+QTime::currentTime().toString()+" transmited Data "+D0+D1+D2+D3+D4+D5+D6+D7;
}


void MainWindow::on_CAN_Konfig_clicked()
{
   can_kfg = new konfiguration(m_serial,this);
   can_kfg->show();

}





void MainWindow::on_lineEdit_DLC_textChanged()
{
     DLC=ui->lineEdit_DLC->text().toInt();


     if (DLC!=DLC_Old) {
         ui->lineEdit_D0->setEnabled(1);
         ui->lineEdit_D1->setEnabled(1);
         ui->lineEdit_D2->setEnabled(1);
         ui->lineEdit_D3->setEnabled(1);
         ui->lineEdit_D4->setEnabled(1);
         ui->lineEdit_D5->setEnabled(1);
         ui->lineEdit_D6->setEnabled(1);
         ui->lineEdit_D7->setEnabled(1);

         //QObjectList Objects=ui->centralwidget->children();
         QList<QLineEdit *> list = ui->centralwidget->findChildren<QLineEdit *>(QString(), Qt::FindDirectChildrenOnly);
         for (int j = DLC; j < 8; j++) {
             for (int i=0; i<list.length(); i++)
             {
                 if (list[i]->objectName().contains("lineEdit_D"+QString::number(j))){
                     dynamic_cast<QWidget*>(list[i])->setDisabled(true);
                 }
             }
         }
         DLC_Old=DLC;
     }
    }
