#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "communicationwidget.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer dataTimer;
    double lastTime;
    double gyroYangle;

private slots:
    void printData(QByteArray data);
    void newData(double dmpYaw, double dmpPitch, double dmpRoll,
                 int accX, int accY, int accZ, int gyroX, int gyroY, int gyroZ,
                 double accRoll, double gyroRoll, double compRoll, double kalRoll,
                 double accPitch, double gyroPitch, double compPitch, double kalPitch);
    void newData2(double dmpYaw, double dmpPitch, double dmpRoll);
    void newData3(double dmpPitch, double setpoint, double input, double output);
    void realtimeDataSlot();
    void on_SpinBox_Kp_editingFinished();
    void on_SpinBox_Ki_editingFinished();
    void on_SpinBox_Kd_editingFinished();
    void on_SpinBox_Setpoint_editingFinished();
    void on_doubleSpinBox_editingFinished();
    void on_RescaleButton_clicked();
};

#endif // MAINWINDOW_H
