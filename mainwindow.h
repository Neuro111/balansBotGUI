#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "communicationwidget.h"

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

private slots:
    void printData(QByteArray data);
    void newData(double dmpYaw, double dmpPitch, double dmpRoll,
                 int accX, int accY, int accZ, int gyroX, int gyroY, int gyroZ,
                 double accRoll, double gyroRoll, double compRoll, double kalRoll,
                 double accPitch, double gyroPitch, double compPitch, double kalPitch);
};

#endif // MAINWINDOW_H
