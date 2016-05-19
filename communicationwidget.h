#ifndef COMMUNICATIONWIDGET_H
#define COMMUNICATIONWIDGET_H

#include <QWidget>

namespace Ui {
class CommunicationWidget;
}

class CommunicationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommunicationWidget(QWidget *parent = 0);
    ~CommunicationWidget();

private:
    Ui::CommunicationWidget *ui;
};

#endif // COMMUNICATIONWIDGET_H
