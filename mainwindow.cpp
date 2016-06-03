#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->rawDataMonitor->hide();
    ui->portMonitor->hide();
    ui->statusBar->hide();
    connect(ui->communicationPanel,SIGNAL(rawData(QByteArray)),this,SLOT(printData(QByteArray)));
    connect(ui->communicationPanel,
            SIGNAL(newData(double,double,double,int,int,int,int,int,int,double,double,double,double,double,double,double,double)),
            this,SLOT(newData(double,double,double,int,int,int,int,int,int,double,double,double,double,double,double,double,double)));
    connect(ui->communicationPanel,
            SIGNAL(newData2(double,double,double)),
            this,SLOT(newData2(double,double,double)));

    connect(ui->communicationPanel,
            SIGNAL(newData3(double,double,double,double)),
            this,SLOT(newData3(double,double,double,double)));

    QCustomPlot *customPlot = ui->plotPitch;

    customPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    customPlot->legend->setFont(legendFont);
    customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
    customPlot->addGraph(); // blue line
    customPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
    customPlot->graph(0)->setAntialiasedFill(false);
    customPlot->graph(0)->setName("real Theta");
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(Qt::red));
    customPlot->graph(1)->setName("requested Theta");
    customPlot->addGraph(); // yellow line
    customPlot->graph(2)->setPen(QPen(Qt::yellow));
    customPlot->graph(2)->setName("requested velocity");
    customPlot->addGraph(); // green line
    customPlot->graph(3)->setPen(QPen(Qt::green));
    customPlot->graph(3)->setName("position");
    customPlot->addGraph(); // magenta line
    customPlot->graph(4)->setPen(QPen(Qt::magenta));
   // customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));

//    customPlot->addGraph(); // blue dot
//    customPlot->graph(2)->setPen(QPen(Qt::blue));
//    customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
//    customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
//    customPlot->addGraph(); // red dot
//    customPlot->graph(3)->setPen(QPen(Qt::red));
//    customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
//    customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);
    gyroYangle =0;
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    double dt = key - lastTime;
    lastTime = key;
    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(2);
    customPlot->axisRect()->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
 //   connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
 //   dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printData(QByteArray data)
{
    ui->portMonitor->append(QString(data));
   // ui->portMonitor->append("\n");
 //   ui->portMonitor->append("\n");
//    ui->rawDataMonitor->append(QString(data.toHex()));
//    ui->rawDataMonitor->append("\n");
//    ui->rawDataMonitor->append("\n");
}

void MainWindow::newData(double dmpYaw, double dmpPitch, double dmpRoll, int accX, int accY, int accZ, int gyroX, int gyroY, int gyroZ, double accRoll, double gyroRoll, double compRoll, double kalRoll, double accPitch, double gyroPitch, double compPitch, double kalPitch)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    double dt = key - lastTime;
    lastTime = key;

   // static double lastPointKey = 0;
//    if (key-lastPointKey > 0.01) // at most add point every 10 ms
//    {
      double value0 = dmpPitch;// niebieski
      double value1 = kalPitch; // czerwony
      double value2 = compPitch; // żółty
      double value3 = accPitch; //zielony
       //-gyroY/16.4; //fioletowy

      gyroYangle += (-gyroY/16.4) * dt;
      double value4 =gyroYangle;
      qDebug() << gyroYangle;
      // add data to lines:
      ui->plotPitch->graph(0)->addData(key, value0);
      ui->plotPitch->graph(1)->addData(key, value1);
      ui->plotPitch->graph(2)->addData(key, value2);
      ui->plotPitch->graph(3)->addData(key, value3);
      ui->plotPitch->graph(4)->addData(key, value4);
      // set data of dots:
//      ui->plotPitch->graph(2)->clearData();
//      ui->plotPitch->graph(2)->addData(key, value0);
//      ui->plotPitch->graph(3)->clearData();
//      ui->plotPitch->graph(3)->addData(key, value1);
      // remove data of lines that's outside visible range:
      ui->plotPitch->graph(0)->removeDataBefore(key-8);
      ui->plotPitch->graph(1)->removeDataBefore(key-8);
      ui->plotPitch->graph(2)->removeDataBefore(key-8);
      ui->plotPitch->graph(3)->removeDataBefore(key-8);
      ui->plotPitch->graph(4)->removeDataBefore(key-8);
      // rescale value (vertical) axis to fit the current data:
      ui->plotPitch->graph(0)->rescaleValueAxis(true);
      ui->plotPitch->graph(1)->rescaleValueAxis(true);
      ui->plotPitch->graph(2)->rescaleValueAxis(true);
      ui->plotPitch->graph(3)->rescaleValueAxis(true);
     // ui->plotPitch->graph(4)->rescaleValueAxis(true);
  //    lastPointKey = key;
  //  }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotPitch->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->plotPitch->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->plotPitch->graph(0)->data()->count()+ui->plotPitch->graph(1)->data()->count())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }



    ui->portMonitor->append(QString::number(gyroPitch)+"\t"+QString::number(accPitch)+"\t"+QString::number(dmpPitch)+
                            "\t"+QString::number(compPitch)+"\t"+QString::number(kalPitch)+"\n");
    ui->rawDataMonitor->append(QString::number(accY)+"\t"+QString::number(gyroY)+"\n");
}

void MainWindow::newData2(double dmpYaw, double dmpPitch, double dmpRoll)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
   // static double lastPointKey = 0;
//    if (key-lastPointKey > 0.01) // at most add point every 10 ms
//    {
      double value0 = dmpPitch;// niebieski
//      double value1 = kalPitch; // czerwony
//      double value2 = compPitch; // żółty
//      double value3 = accPitch; //zielony
      // add data to lines:
      ui->plotPitch->graph(0)->addData(key, value0);
//      ui->plotPitch->graph(1)->addData(key, value1);
//      ui->plotPitch->graph(2)->addData(key, value2);
//      ui->plotPitch->graph(3)->addData(key, value3);
      // set data of dots:
//      ui->plotPitch->graph(2)->clearData();
//      ui->plotPitch->graph(2)->addData(key, value0);
//      ui->plotPitch->graph(3)->clearData();
//      ui->plotPitch->graph(3)->addData(key, value1);
      // remove data of lines that's outside visible range:
      ui->plotPitch->graph(0)->removeDataBefore(key-8);
//      ui->plotPitch->graph(1)->removeDataBefore(key-8);
//      ui->plotPitch->graph(2)->removeDataBefore(key-8);
//      ui->plotPitch->graph(3)->removeDataBefore(key-8);
      // rescale value (vertical) axis to fit the current data:
      ui->plotPitch->graph(0)->rescaleValueAxis(true);
//      ui->plotPitch->graph(1)->rescaleValueAxis(true);
//      ui->plotPitch->graph(2)->rescaleValueAxis(true);
//      ui->plotPitch->graph(3)->rescaleValueAxis(true);
  //    lastPointKey = key;
  //  }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotPitch->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->plotPitch->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->plotPitch->graph(0)->data()->count()+ui->plotPitch->graph(1)->data()->count())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}

void MainWindow::newData3(double dmpPitch, double setpoint, double input, double output)
{
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    double dt = key - lastTime;
    lastTime = key;

   // static double lastPointKey = 0;
//    if (key-lastPointKey > 0.01) // at most add point every 10 ms
//    {
      double value0 = dmpPitch;// niebieski
      double value1 = setpoint; // czerwony
      double value2 = input; // żółty
      double value3 = output; //zielony
       //-gyroY/16.4; //fioletowy

//      gyroYangle += (-gyroY/16.4) * dt;
//      double value4 =gyroYangle;
//      qDebug() << gyroYangle;
      // add data to lines:
      ui->plotPitch->graph(0)->addData(key, value0);
      ui->plotPitch->graph(1)->addData(key, value1);
      ui->plotPitch->graph(2)->addData(key, value2);
      ui->plotPitch->graph(3)->addData(key, value3);
//      ui->plotPitch->graph(4)->addData(key, value4);
      // set data of dots:
//      ui->plotPitch->graph(2)->clearData();
//      ui->plotPitch->graph(2)->addData(key, value0);
//      ui->plotPitch->graph(3)->clearData();
//      ui->plotPitch->graph(3)->addData(key, value1);
      // remove data of lines that's outside visible range:
      ui->plotPitch->graph(0)->removeDataBefore(key-8);
      ui->plotPitch->graph(1)->removeDataBefore(key-8);
      ui->plotPitch->graph(2)->removeDataBefore(key-8);
      ui->plotPitch->graph(3)->removeDataBefore(key-8);
  //    ui->plotPitch->graph(4)->removeDataBefore(key-8);
      // rescale value (vertical) axis to fit the current data:
      ui->plotPitch->graph(0)->rescaleValueAxis(true);
      ui->plotPitch->graph(1)->rescaleValueAxis(true);
      ui->plotPitch->graph(2)->rescaleValueAxis(true);
      ui->plotPitch->graph(3)->rescaleValueAxis(true);
     // ui->plotPitch->graph(4)->rescaleValueAxis(true);
  //    lastPointKey = key;
  //  }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotPitch->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->plotPitch->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->plotPitch->graph(0)->data()->count()+ui->plotPitch->graph(1)->data()->count())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }



    ui->portMonitor->append(QString::number(dmpPitch)+"\t"+QString::number(setpoint)+"\t"+QString::number(input)+
                            "\t"+QString::number(output)+"\n");
    //ui->rawDataMonitor->append(QString::number(accY)+"\t"+QString::number(gyroY)+"\n");
}

void MainWindow::realtimeDataSlot()
{
    // calculate two new data points:
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.01) // at most add point every 10 ms
    {
      double value0 = qSin(key); //qSin(key*1.6+qCos(key*1.7)*2)*10 + qSin(key*1.2+0.56)*20 + 26;
      double value1 = qCos(key); //qSin(key*1.3+qCos(key*1.2)*1.2)*7 + qSin(key*0.9+0.26)*24 + 26;
      // add data to lines:
      ui->plotPitch->graph(0)->addData(key, value0);
      ui->plotPitch->graph(1)->addData(key, value1);
      // set data of dots:
      ui->plotPitch->graph(2)->clearData();
      ui->plotPitch->graph(2)->addData(key, value0);
      ui->plotPitch->graph(3)->clearData();
      ui->plotPitch->graph(3)->addData(key, value1);
      // remove data of lines that's outside visible range:
      ui->plotPitch->graph(0)->removeDataBefore(key-8);
      ui->plotPitch->graph(1)->removeDataBefore(key-8);
      // rescale value (vertical) axis to fit the current data:
      ui->plotPitch->graph(0)->rescaleValueAxis();
      ui->plotPitch->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotPitch->xAxis->setRange(key+0.25, 8, Qt::AlignRight);
    ui->plotPitch->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->plotPitch->graph(0)->data()->count()+ui->plotPitch->graph(1)->data()->count())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}

void MainWindow::on_SpinBox_Kp_editingFinished()
{
    ui->communicationPanel->writeData("p",ui->SpinBox_Kp->value());
}

void MainWindow::on_SpinBox_Ki_editingFinished()
{
    ui->communicationPanel->writeData("i",ui->SpinBox_Ki->value());
}

void MainWindow::on_SpinBox_Kd_editingFinished()
{
    ui->communicationPanel->writeData("d",ui->SpinBox_Kd->value());
}

void MainWindow::on_SpinBox_Setpoint_editingFinished()
{
    ui->communicationPanel->writeData("s",ui->SpinBox_Setpoint->value());
}

void MainWindow::on_doubleSpinBox_editingFinished()
{
    ui->communicationPanel->writeData("m",ui->doubleSpinBox->value());
}

void MainWindow::on_RescaleButton_clicked()
{
    ui->plotPitch->graph(0)->rescaleValueAxis();
}

void MainWindow::on_filterSpinBox_editingFinished()
{
    ui->communicationPanel->writeData("f",ui->filterSpinBox->value());
}

void MainWindow::on_balansKd_editingFinished()
{
    ui->communicationPanel->writeData("e",ui->balansKd->value());
}

void MainWindow::on_balansKi_editingFinished()
{
    ui->communicationPanel->writeData("w",ui->balansKi->value());
}

void MainWindow::on_balansKp_editingFinished()
{
    ui->communicationPanel->writeData("q",ui->balansKp->value());
}

void MainWindow::on_positionKp_editingFinished()
{
    ui->communicationPanel->writeData("r",ui->positionKp->value());
}

void MainWindow::on_positionKi_editingFinished()
{
    ui->communicationPanel->writeData("t",ui->positionKi->value());
}

void MainWindow::on_positionKd_editingFinished()
{
    ui->communicationPanel->writeData("y",ui->positionKd->value());
}
