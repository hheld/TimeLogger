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

QMap<QString, double> ProjectDatabase::GetProjectsWorkedHoursInRange(const QDate &from, const QDate &to) const
{
    QMap<QString, double> project2hours;

    if(to > from)
    {
        QString sql;
        sql  = "SELECT name, Start, End FROM Projects ";
        sql += "WHERE Start >= '" + from.toString(Qt::ISODate) + "' AND End <= '" + to.toString(Qt::ISODate) + "' ";
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
            double workedHours = projectStart.secsTo(projectEnd) / 3600.;

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
