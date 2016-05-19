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

private:
    Ui::CommunicationWidget *ui;
    QSerialPort *port;
};

#endif // COMMUNICATIONWIDGET_H
