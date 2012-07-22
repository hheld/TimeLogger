#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QVector>
#include <QVariant>

class Project
{
public:
    explicit Project(const QString &name, Project *parent = 0);

    const QString& Name() const;
    void Name(const QString &name);

    Project* Parent() const;
    void Parent(Project* const parent);

    QVariant col(int column) const;

    Project* SubProject(int row) const;

    int NumOfSubprojects() const;

    void AddSubProject(Project *subProject);

private:
    QString name;
    QVector<Project*> subProjects;
    Project *parent;
};

#endif // PROJECT_H
