#include "DayView.h"
#include "ui_DayView.h"

#include <QDebug>

#include "DayGraphicsScene.h"
#include "ProjectDatabase.h"
#include "ProjectGraphicsItem.h"

DayView::DayView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DayView),
    dayScene(0),
    db(0)
{
    ui->setupUi(this);

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
}

void DayView::on_dateEdit_selectDay_dateChanged(const QDate &date)
{
    if(!db)
    {
        return;
    }

    // clear everything currently on the scene
    dayScene->clear();

    QMap<QString, QList<QPair<QDateTime, QDateTime> > > details = db->GetProjectDetailsForDay(date);

    QMap<QString, QList<QPair<QDateTime, QDateTime> > >::const_iterator cit = details.constBegin();

    int numOfProjects = details.size();
    int index = 0;

    while(cit != details.constEnd())
    {
        QString projectName = cit.key();

        QList<QPair<QDateTime, QDateTime> >::const_iterator lcit = cit.value().constBegin();

        while(lcit != cit.value().constEnd())
        {
            QDateTime start = lcit->first;
            QDateTime end = lcit->second;

            ProjectGraphicsItem *pgi = new ProjectGraphicsItem;

            pgi->SetProjectInfo(projectName, start, end, numOfProjects, index);

            QString toolTip = tr("Project name: %1\nStart: %2\nEnd: %3")
                    .arg(projectName)
                    .arg(start.toString(Qt::SystemLocaleShortDate))
                    .arg(end.toString(Qt::SystemLocaleShortDate));

            pgi->setToolTip(toolTip);

            dayScene->addItem(pgi);

            ++lcit;
        }

        ++cit;
        ++index;
    }

    if(numOfProjects)
    {
        dayScene->setSceneRect(dayScene->itemsBoundingRect());
    }
}

void DayView::paintEvent(QPaintEvent *)
{
    ui->dateEdit_selectDay->setDate(QDate::currentDate());
}
