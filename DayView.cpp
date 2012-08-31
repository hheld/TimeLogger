#include "DayView.h"
#include "ui_DayView.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

#include "DayGraphicsScene.h"
#include "ProjectDatabase.h"
#include "ProjectGraphicsItem.h"
#include "DialogAddHours.h"
#include "Project.h"

DayView::DayView(Project *root, QTime *startOfWorkDay, int *numOfWorkingHours, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DayView),
    dayScene(0),
    db(0),
    root(root),
    initialSettingOfDateInPaintEventDone(false),
    startOfWorkDay(startOfWorkDay),
    numOfWorkingHours(numOfWorkingHours)
{
    ui->setupUi(this);

    dayScene = new DayGraphicsScene(this);
    dayScene->SetStartWorkDay(startOfWorkDay);
    dayScene->SetWorkingHoursPerDay(numOfWorkingHours);

    ui->dateEdit_selectDay->setDate(QDate::currentDate());

    ui->graphicsView->setScene(dayScene);

    connect(dayScene, SIGNAL(itemChanged()), this, SLOT(itemChanged()));
    connect(dayScene, SIGNAL(itemRemoved(QDate)), this, SLOT(on_dateEdit_selectDay_dateChanged(QDate)));
    connect(dayScene, SIGNAL(doubleClickedOnBackground(QTime)), this, SLOT(toolButton_addProject_clicked(QTime)));
    connect(ui->toolButton_addProject, SIGNAL(clicked()), this, SLOT(toolButton_addProject_clicked()));
}

DayView::~DayView()
{
    delete ui;
}

void DayView::SetProjectDatabase(ProjectDatabase *db)
{
    this->db = db;
}

void DayView::ViewOnly(bool viewOnly)
{
    ui->graphicsView->setInteractive(!viewOnly);
}

void DayView::itemChanged()
{
    ui->toolButton_commitToDb->setEnabled(true);
    ui->toolButton_ResetChanges->setEnabled(true);
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

            ProjectGraphicsItem *pgi = new ProjectGraphicsItem(db);

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
    if(!initialSettingOfDateInPaintEventDone)
    {
        on_dateEdit_selectDay_dateChanged(ui->dateEdit_selectDay->date());
        initialSettingOfDateInPaintEventDone = true;
    }
}

void DayView::on_toolButton_commitToDb_clicked()
{
    QList<QGraphicsItem*> allItems = dayScene->items();

    foreach(QGraphicsItem *i, allItems)
    {
        ProjectGraphicsItem *p = dynamic_cast<ProjectGraphicsItem*>(i);

        p->UpdateDatabaseEntry();
    }

    ui->toolButton_commitToDb->setEnabled(false);
    ui->toolButton_ResetChanges->setEnabled(false);
}

void DayView::toolButton_addProject_clicked(const QTime &time)
{
    DialogAddHours diag(this);

    if(!time.isNull())
    {
        diag.SetStart(time);
    }

    QStringList listOfWorkableProjects = root->GetAllWorkableProjectNames();

    if(listOfWorkableProjects.isEmpty())
    {
        QMessageBox::information(this, tr("Setup projects"), tr("To manually add worked hours, you need to define workable projects in the main window first."));

        return;
    }

    diag.SetListOfWorkableProjects(listOfWorkableProjects);

    if(diag.exec() == QDialog::Accepted)
    {
        QString projectName = diag.Name();
        QTime start = diag.Start();
        QTime end = diag.End();

        QDateTime dtStart(ui->dateEdit_selectDay->date(), start);
        QDateTime dtEnd(ui->dateEdit_selectDay->date(), end);

        QString sql = "INSERT OR IGNORE INTO Projects (Name, Start, End) VALUES ('" + projectName + "', '" + dtStart.toString(Qt::ISODate) + "', '" + dtEnd.toString(Qt::ISODate) + "')";

        QSqlQuery query(*db->Db());

        bool query_ok = query.exec(sql);

        if(!query_ok)
        {
            qDebug() << query.lastError().text();
        }

        // without this, already made changes are reset by the next function call, so we should automatically commit all changes to the DB - most likely, this is what the user expects anyway
        on_toolButton_commitToDb_clicked();

        on_dateEdit_selectDay_dateChanged(ui->dateEdit_selectDay->date());
    }
}

void DayView::on_toolButton_ResetChanges_clicked()
{
    on_dateEdit_selectDay_dateChanged(ui->dateEdit_selectDay->date());

    ui->toolButton_commitToDb->setEnabled(false);
    ui->toolButton_ResetChanges->setEnabled(false);
}
