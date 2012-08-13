#include "DayView.h"
#include "ui_DayView.h"

#include <QDebug>

#include "DayGraphicsScene.h"
#include "ProjectDatabase.h"

DayView::DayView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DayView),
    dayScene(0),
    db(0)
{
    ui->setupUi(this);

    ui->dateEdit_selectDay->setDate(QDate::currentDate());

    dayScene = new DayGraphicsScene(this);

    ui->graphicsView->setScene(dayScene);
}

DayView::~DayView()
{
    delete ui;
    delete dayScene;
}

void DayView::SetProjectDatabase(ProjectDatabase *db)
{
    this->db = db;

    on_dateEdit_selectDay_dateChanged(QDate::currentDate());
}

void DayView::on_dateEdit_selectDay_dateChanged(const QDate &date)
{
    if(!db)
    {
        return;
    }

    QMap<QString, QList<QPair<QDateTime, QDateTime> > > details = db->GetProjectDetailsForDay(date);

    QMap<QString, QList<QPair<QDateTime, QDateTime> > >::const_iterator cit = details.constBegin();

    while(cit != details.constEnd())
    {
        qDebug() << cit.key() << cit.value();

        ++cit;
    }
}
