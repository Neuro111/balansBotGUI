#include "communicationwidget.h"
#include "ui_communicationwidget.h"

CommunicationWidget::CommunicationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicationWidget)
{
    ui->setupUi(this);

    port = new QSerialPort(this);
    port->setBaudRate(QSerialPort::Baud115200);
    port->setDataBits(QSerialPort::Data8);
    port->setParity(QSerialPort::NoParity);
    port->setStopBits(QSerialPort::OneStop);


    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    for(int i=0; i<availablePorts.count(); i++)
    {
        ui->portListComboBox->addItem(availablePorts.at(i).portName());
    }
    ui->portListComboBox->addItem("<odśwież>");
}

CommunicationWidget::~CommunicationWidget()
{
    delete ui;
}

void CommunicationWidget::on_OpenButton_toggled(bool checked)
{
    if(checked)
    {
        port->setPortName(ui->portListComboBox->currentText());
        if (port->open(QIODevice::ReadWrite)) {
            //ui->statusLabel->setText("Port otwarty");
            ui->portListComboBox->setEnabled(false);
            connect(port,SIGNAL(readyRead()),this,SLOT(readData()));
        } else {
         //   ui->statusLabel->setText("Nie można otworzyć portu" + port->errorString());
            ui->OpenButton->setChecked(false);
        }
    }
    else
    {
        if (port->isOpen())
        {
            port->close();
        }
        disconnect(port,SIGNAL(readyRead()),this,SLOT(readData()));
        ui->portListComboBox->setEnabled(true);
    }
}

void CommunicationWidget::on_portListComboBox_activated(const QString &arg1)
{
    if(arg1 == "<odśwież>")
    {
        ui->portListComboBox->clear();
        QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
        for(int i=0; i<availablePorts.count(); i++)
        {
            ui->portListComboBox->addItem(availablePorts.at(i).portName());
        }
        ui->portListComboBox->addItem("<odśwież>");
        ui->portListComboBox->setCurrentIndex(0);
    }

}

void CommunicationWidget::readData()
{
    QByteArray rawDataBuffer = port->readAll();
    dataBuffer.append(rawDataBuffer);
    emit rawData(rawDataBuffer);
    //QStringList frame;
    buffer = dataBuffer.split("\t");
    dataBuffer = buffer.takeLast();
    for(int i=0; i<buffer.count();i++)
    {
        if(buffer.at(i) == "\r\nypr")
        {
            if(index == 17)
                emit newData(frame.at(0).toDouble(),frame.at(1).toDouble(),frame.at(2).toDouble(),frame.at(3).toInt(),frame.at(4).toInt(),
                             frame.at(5).toInt(),frame.at(6).toInt(),frame.at(7).toInt(),frame.at(8).toInt(),frame.at(9).toDouble(),
                             frame.at(10).toDouble(),frame.at(11).toDouble(),frame.at(12).toDouble(),frame.at(13).toDouble(),
                             frame.at(14).toDouble(),frame.at(15).toDouble(),frame.at(16).toDouble());
            if(index == 9)
                emit newData(frame.at(0).toDouble(),frame.at(1).toDouble(),frame.at(2).toDouble(),frame.at(3).toInt(),frame.at(4).toInt(),
                             frame.at(5).toInt(),frame.at(6).toInt(),frame.at(7).toInt(),frame.at(8).toInt(),0,0,0,0,0,0,0,0);
            if(index == 3)
                emit newData2(frame.at(0).toDouble(),frame.at(1).toDouble(),frame.at(2).toDouble());
            if(index == 4)
                emit newData3(frame.at(0).toDouble(),frame.at(1).toDouble(),frame.at(2).toDouble(), frame.at(3).toDouble());
            frame.clear();
            index=0;
        }
        else
        {
            frame.append(buffer.at(i));
            index++;
        }
    }
    buffer.clear();
}

void CommunicationWidget::writeData(const char* dataID, double data)
{
    QByteArray dataToSend;
    dataToSend.append(dataID);
    dataToSend.append("\t");
    dataToSend.append(QByteArray::number(data));
    dataToSend.append("\n");

    qDebug() << dataToSend;
    //qDebug() << dataToSend.toStdString();
    port->write(QByteArray(dataToSend));
  //  port->writeData(datastr);
}
