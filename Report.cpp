#include "Report.h"
#include "ui_Report.h"
#include "ProjectDatabase.h"

#include <QDebug>

Report::Report(ProjectDatabase *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Report),
    db(db)
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

void Report::on_pushButton_generateReport_clicked()
{
    QMap<QString, double> workedHours = db->GetProjectsWorkedHoursInRange(ui->dateEdit_from->date(), ui->dateEdit_to->date());

    QMap<QString, double>::const_iterator cit = workedHours.constBegin();

    while(cit != workedHours.constEnd())
    {
        qDebug() << cit.key() << cit.value();

        ++cit;
    }
}
