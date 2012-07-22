#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QVector>

class Project
{
public:
    explicit Project(const QString &name, Project *parent = 0);

    const QString& Name() const;
    void Name(const QString &name);

    Project* Parent() const;
    void Parent(Project* const parent);

private:
    QString name;
    QVector<Project*> subProjects;
    Project *parent;
};

#endif // PROJECT_H
