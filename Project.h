#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QVector>
#include <QVariant>

class Project
{
public:
    explicit Project(const QString &name, Project *parent = 0);
    ~Project();

    const QString& Name() const;
    void Name(const QString &name);

    Project* Parent() const;
    void Parent(Project* const parent);

    QVariant col(int column) const;

    Project* SubProject(int row) const;

    int NumOfSubprojects() const;
    int SubprojectNumber() const;

    void AddSubProject(Project *subProject);
    void RemoveSubProject(Project *subProject);

    double TotalHours() const;
    void TotalHours(const double &hours);

    double PlannedHours() const;
    void PlannedHours(const double &hours);

    double WorkedHours() const;
    void WorkedHours(const double &workedHours);

    const QVector<Project*>& AllSubprojects() const;

private:
    QString name;
    QVector<Project*> subProjects;
    Project *parent;

    double totalBudgetHours;
    double myPlannedHours;
    double workedHours;
};

#endif // PROJECT_H
