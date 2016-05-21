#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->communicationPanel,SIGNAL(rawData(QByteArray)),this,SLOT(printData(QByteArray)));
    connect(ui->communicationPanel,
            SIGNAL(newData(double,double,double,int,int,int,int,int,int,double,double,double,double,double,double,double,double)),
            this,SLOT(newData(double,double,double,int,int,int,int,int,int,double,double,double,double,double,double,double,double)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printData(QByteArray data)
{
    ui->portMonitor->append(QString(data));
    ui->portMonitor->append("\n");
    ui->portMonitor->append("\n");
    ui->rawDataMonitor->append(QString(data.toHex()));
    ui->rawDataMonitor->append("\n");
    ui->rawDataMonitor->append("\n");
}

void MainWindow::newData(double dmpYaw, double dmpPitch, double dmpRoll, int accX, int accY, int accZ, int gyroX, int gyroY, int gyroZ, double accRoll, double gyroRoll, double compRoll, double kalRoll, double accPitch, double gyroPitch, double compPitch, double kalPitch)
{
    ui->portMonitor->append(QString::number(gyroPitch)+"\t"+QString::number(accPitch)+"\t"+QString::number(dmpPitch)+
                            "\t"+QString::number(compPitch)+"\t"+QString::number(kalPitch)+"\n");
    ui->rawDataMonitor->append(QString::number(accY)+"\t"+QString::number(gyroY)+"\n");
}
