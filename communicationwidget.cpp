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
            ui->statusLabel->setText("Port otwarty");
            ui->portListComboBox->setEnabled(false);
            //connect(serial,SIGNAL(readyRead()),this,SLOT(readData()));
        } else {
            ui->statusLabel->setText("Nie można otworzyć portu" + port->errorString());
            ui->OpenButton->setChecked(false);
        }
    }
    else
    {
        if (port->isOpen())
        {
            port->close();
        }
        //disconnect(serial,SIGNAL(readyRead()),this,SLOT(readData()));
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
