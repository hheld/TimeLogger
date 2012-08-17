#include "ProjectDatabase.h"
#include "Project.h"

#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

QString ProjectDatabase::pathToDb = QDir::homePath() + "/.timelogger/projects.sqlite";

ProjectDatabase::ProjectDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(pathToDb);

    bool db_ok = db.open();

    if (!db_ok)
    {
        qDebug() << "Could not open the sqlite3 database '" << pathToDb << "'.";
        return;
    }
}

ProjectDatabase::~ProjectDatabase()
{
    db.close();
}

void ProjectDatabase::LogWorkingStart(Project *p, const QDateTime &start)
{
    AddProjectTable();

    QString startAsString = start.toString(Qt::ISODate);

    QString sql = "INSERT INTO Projects (Name, Start) VALUES ('" + p->DbName() + "', '" + startAsString + "')";

    QSqlQuery query(db);

    bool query_ok = query.exec(sql);

    if(!query_ok)
    {
        qDebug() << query.lastError().text();
        lastStartTime.clear();
    }

    lastStartTime = startAsString;
}

void ProjectDatabase::LogWorkingEnd(Project *p, const QDateTime &end)
{
    AddProjectTable();

    QString startAsString = end.toString(Qt::ISODate);

    QString sql = "UPDATE Projects SET End = '" + startAsString + "' WHERE Start='" + lastStartTime + "' AND Name='" + p->DbName() + "'";

    QSqlQuery query(db);

    bool query_ok = query.exec(sql);

    if(!query_ok)
    {
        qDebug() << query.lastError().text();
    }
}

QMap<QString, double> ProjectDatabase::GetProjectsTotalWorkedHoursInRange(const QDate &from, const QDate &to) const
{
    QMap<QString, double> project2hours;

    if(to > from)
    {
        QString sql;
        sql  = "SELECT name, Start, End FROM Projects ";
        sql += "WHERE Start >= '" + from.toString(Qt::ISODate) + "' AND End < '" + to.addDays(1).toString(Qt::ISODate) + "' ";
        sql += "ORDER BY Start";

        QSqlQuery query(db);

        bool query_ok = query.exec(sql);

        if(!query_ok)
        {
            qDebug() << query.lastError().text();
        }

        while(query.next())
        {
            QString projectName = query.value(0).toString();
            QDateTime projectStart = query.value(1).toDateTime();
            QDateTime projectEnd = query.value(2).toDateTime();
            double workedHours = RoundHours(projectStart.secsTo(projectEnd) / 3600.);

            if(project2hours.contains(projectName))
            {
                project2hours[projectName] += workedHours;
            }
            else
            {
                project2hours[projectName] = workedHours;
            }
        }
    }

    return project2hours;
}

QMap<QDate, QList<QPair<QString, double> > > ProjectDatabase::GetProjectsDailyWorkedHoursInRange(const QDate &from, const QDate &to) const
{
    QMap<QDate, QList<QPair<QString, double> > > dates2Projects;

    if(to > from)
    {
        int numOfDays = from.daysTo(to) + 1;

        for(int i=0; i<numOfDays; ++i)
        {
            QString sql;
            sql  = "SELECT name, Start, End FROM Projects ";
            sql += "WHERE Start >= '" + from.addDays(i).toString(Qt::ISODate) + "' AND End < '" + from.addDays(i+1).toString(Qt::ISODate) + "' ";
            sql += "ORDER BY Start";

            QSqlQuery query(db);

            bool query_ok = query.exec(sql);

            if(!query_ok)
            {
                qDebug() << query.lastError().text();
            }

            while(query.next())
            {
                QString projectName = query.value(0).toString();
                QDateTime projectStart = query.value(1).toDateTime();
                QDateTime projectEnd = query.value(2).toDateTime();
                double workedHours = RoundHours(projectStart.secsTo(projectEnd) / 3600.);

                QList<QPair<QString, double> > &listOfProjects = dates2Projects[from.addDays(i)];

                UpdateWorkedHoursInList(listOfProjects, projectName, workedHours);
            }
        }
    }

    return dates2Projects;
}

QMap<QString, QList<QPair<QDateTime, QDateTime> > > ProjectDatabase::GetProjectDetailsForDay(const QDate &day) const
{
    QMap<QString, QList<QPair<QDateTime, QDateTime> > > projects2Details;

    QString sql;

    sql  = "SELECT name, Start, End FROM Projects ";
    sql += "WHERE Start >= '" + day.toString(Qt::ISODate) + "' AND (End < '" + day.addDays(1).toString(Qt::ISODate) + "' OR End IS NULL)";
    sql += "ORDER BY name";

    QSqlQuery query(db);

    bool query_ok = query.exec(sql);

    if(!query_ok)
    {
        qDebug() << query.lastError().text();
    }

    while(query.next())
    {
        QString projectName = query.value(0).toString();
        QDateTime projectStart = query.value(1).toDateTime();
        QDateTime projectEnd = query.value(2).toDateTime();

        if(projectEnd.isNull())
        {
            projectEnd = QDateTime::currentDateTime();
        }

        QList<QPair<QDateTime, QDateTime > > &listOfDetails = projects2Details[projectName];

        listOfDetails.append(qMakePair(projectStart, projectEnd));
    }

    return projects2Details;
}

void ProjectDatabase::AddProjectTable()
{
    QString sql = "CREATE TABLE IF NOT EXISTS Projects (";

    sql += "Name TEXT, ";
    sql += "Start TEXT, ";
    sql += "End TEXT, ";
    sql += "PRIMARY KEY (Start, End)";
    sql += ")";

    QSqlQuery query(db);
    query.exec(sql);
}

void ProjectDatabase::UpdateWorkedHoursInList(QList<QPair<QString, double> > &list, const QString &name, const double &workedHours)
{
    QList<QPair<QString, double> >::iterator it = list.begin();

    while(it != list.end())
    {
        if(it->first == name)
        {
            break;
        }

        ++it;
    }

    if(it != list.end())
    {
        it->second += workedHours;
    }
    else
    {
        list.append(qMakePair(name, workedHours));
    }
}

double ProjectDatabase::RoundHours(const double &hours)
{
    int trunc = 10.*hours;

    int modFive = trunc%5;

    if(modFive<3)
    {
        trunc -= modFive;
    }
    else
    {
        trunc += 5 - modFive;
    }

    return static_cast<double>(trunc / 10.);
}
