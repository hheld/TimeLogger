#include "Report.h"
#include "ui_Report.h"

Report::Report(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Report)
{
    ui->setupUi(this);

    QDate currentDate = QDate::currentDate();

    ui->dateEdit_from->setDate(QDate(currentDate.year(), currentDate.month(), 1));
    ui->dateEdit_to->setDate(QDate(currentDate.year(), currentDate.month(), currentDate.daysInMonth()));
}

Report::~Report()
{
    delete ui;
}
