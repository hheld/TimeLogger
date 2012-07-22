#include "Project.h"

Project::Project(const QString &name, Project *parent) :
    name(name),
    parent(parent)
{
}

const QString &Project::Name() const
{
    return name;
}

void Project::Name(const QString &name)
{
    this->name = name;
}

Project *Project::Parent() const
{
    return parent;
}

void Project::Parent(Project *const parent)
{
    this->parent = parent;
}

QVariant Project::col(int column) const
{
    Q_ASSERT(column < 1);

    switch(column)
    {
    case 0:
        return name;
        break;
    default:
        return QVariant();
    }
}

Project *Project::SubProject(int row) const
{
    if(row<0)
    {
        return 0;
    }

    if(row < subProjects.size())
    {
        return subProjects[row];
    }

    return 0;
}

int Project::NumOfSubprojects() const
{
    return subProjects.size();
}

void Project::AddSubProject(Project *subProject)
{
    subProjects.append(subProject);
}
