#ifndef PROJECTDATABASE_H
#define PROJECTDATABASE_H

#include <QtSql/QSqlDatabase>
#include <QDateTime>

class Project;

class ProjectDatabase
{
public:
    ProjectDatabase();
    ~ProjectDatabase();

    void LogWorkingStart(Project *p, const QDateTime &start);
    void LogWorkingEnd(Project *p, const QDateTime &end);

private:
    QSqlDatabase db;
    QString lastStartTime;

    /** Only does it if it doesn't exist yet.
     */
    void AddProjectTable();

    static QString pathToDb;
};

#endif // PROJECTDATABASE_H
