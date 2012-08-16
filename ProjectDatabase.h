#ifndef PROJECTDATABASE_H
#define PROJECTDATABASE_H

#include <QtSql/QSqlDatabase>
#include <QDateTime>
#include <QMap>
#include <QPair>

class Project;

class ProjectDatabase
{
public:
    ProjectDatabase();
    ~ProjectDatabase();

    void LogWorkingStart(Project *p, const QDateTime &start);
    void LogWorkingEnd(Project *p, const QDateTime &end);

    /** The returned hours already are rounded to half hours.
     */
    QMap<QString, double> GetProjectsTotalWorkedHoursInRange(const QDate &from, const QDate &to) const;

    /** The returned hours already are rounded to half hours.
     */
    QMap<QDate, QList<QPair<QString, double> > > GetProjectsDailyWorkedHoursInRange(const QDate &from, const QDate &to) const;

    QMap<QString, QList<QPair<QDateTime, QDateTime> > > GetProjectDetailsForDay(const QDate &day) const;

    static double RoundHours(const double &hours);

    inline QSqlDatabase* Db() { return &db; }

private:
    QSqlDatabase db;
    QString lastStartTime;

    /** Only does it if it doesn't exist yet.
     */
    void AddProjectTable();

    static void UpdateWorkedHoursInList(QList<QPair<QString, double> > &list, const QString &name, const double &workedHours);

    static QString pathToDb;
};

#endif // PROJECTDATABASE_H
