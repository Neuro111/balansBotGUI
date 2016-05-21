#ifndef COMMUNICATIONWIDGET_H
#define COMMUNICATIONWIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QDebug>

namespace Ui {
class CommunicationWidget;
}

class CommunicationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommunicationWidget(QWidget *parent = 0);
    ~CommunicationWidget();

private slots:

    void on_OpenButton_toggled(bool checked);

    void on_portListComboBox_activated(const QString &arg1);

    void readData();

private:
    Ui::CommunicationWidget *ui;
    QSerialPort *port;
    QString dataBuffer;
    QStringList buffer;
    QStringList frame;
    int index;


signals:
    void rawData(QByteArray);
    void newData(double dmpYaw, double dmpPitch, double dmpRoll,
                 int accX, int accY, int accZ, int gyroX, int gyroY, int gyroZ,
                 double accRoll, double gyroRoll, double compRoll, double kalRoll,
                 double accPitch, double gyroPitch, double compPitch, double kalPitch);
    void newData2(double dmpYaw, double dmpPitch, double dmpRoll);
};

#endif // COMMUNICATIONWIDGET_H
