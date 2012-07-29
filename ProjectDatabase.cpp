#include "ProjectDatabase.h"
#include "Project.h"

#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

QString ProjectDatabase::pathToDb = QDir::homePath() + "/.timelogger/projects.db";

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
    AddProjectTable(p);

    QString startAsString = start.toString(Qt::ISODate);

    QString sql = "INSERT INTO " + p->Name() + " (Start) VALUES ('" + startAsString + "')";

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
    AddProjectTable(p);

    QString startAsString = end.toString(Qt::ISODate);

    QString sql = "UPDATE " + p->Name() + " SET End = '" + startAsString + "' WHERE Start='" + lastStartTime + "'";

    QSqlQuery query(db);

    bool query_ok = query.exec(sql);

    if(!query_ok)
    {
        qDebug() << query.lastError().text();
    }
}

void ProjectDatabase::AddProjectTable(Project *p)
{
    QString sql = "CREATE TABLE IF NOT EXISTS " + p->Name() + " (";

    sql += "Start TEXT, ";
    sql += "End TEXT";
    sql += ")";

    QSqlQuery query(db);
    query.exec(sql);
}
