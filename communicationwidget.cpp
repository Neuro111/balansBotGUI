#include "communicationwidget.h"
#include "ui_communicationwidget.h"

CommunicationWidget::CommunicationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicationWidget)
{
    ui->setupUi(this);
}

CommunicationWidget::~CommunicationWidget()
{
    delete ui;
}
