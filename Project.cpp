#include "Project.h"

Project::Project(const QString &name, Project *parent) :
    name(name),
    parent(parent),
    totalBudgetHours(0.),
    myPlannedHours(0.)
{
}

Project::~Project()
{
    qDeleteAll(subProjects);

    subProjects.clear();
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
    case 1:
        return totalBudgetHours;
        break;
    case 2:
        return myPlannedHours;
    default:
        return QVariant();
    }
}

Project *Project::SubProject(int row) const
{
    if(row>=0 && row < subProjects.size())
    {
        return subProjects[row];
    }

    return 0;
}

int Project::NumOfSubprojects() const
{
    return subProjects.size();
}

int Project::SubprojectNumber() const
{
    if (parent)
    {
        return parent->subProjects.indexOf(const_cast<Project*>(this));
    }

    return 0;
}

void Project::AddSubProject(Project *subProject)
{
    subProjects.append(subProject);
}

void Project::RemoveSubProject(Project *subProject)
{
    subProjects.remove(subProjects.indexOf(subProject));
}

double Project::TotalHours() const
{
    return totalBudgetHours;
}

void Project::TotalHours(const double &hours)
{
    totalBudgetHours = hours;
}

double Project::PlannedHours() const
{
    return myPlannedHours;
}

void Project::PlannedHours(const double &hours)
{
    myPlannedHours = hours;
}
