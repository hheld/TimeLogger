#include "Project.h"

#include <QDir>

QString Project::pathToProjectXML = QDir::homePath() + "/.timelogger/projects.xml";

Project::Project(const QString &name, Project *parent) :
    name(name),
    parent(parent),
    totalBudgetHours(0.),
    myPlannedHours(0.),
    workedHours(0.)
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
    subProject->Parent(this);
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

double Project::WorkedHours() const
{
    return workedHours;
}

void Project::WorkedHours(const double &workedHours)
{
    this->workedHours = workedHours;
}

const QVector<Project *> &Project::AllSubprojects() const
{
    return subProjects;
}

bool Project::CheckConsistency() const
{
    bool isConsistent = true;

    if(parent)
    {
        // 1. planned hours always <= total hours
        if(myPlannedHours > totalBudgetHours)
        {
            return false;
        }

        // 2. sum of subprojects' hours must be <= this project's hours
        double sumTotalHoursSubprojects = 0.;
        double sumPlannedHoursSubprojects = 0.;

        foreach(Project *sp, subProjects)
        {
            sumTotalHoursSubprojects += sp->TotalHours();
            sumPlannedHoursSubprojects += sp->PlannedHours();
        }

        if(sumTotalHoursSubprojects > totalBudgetHours || sumPlannedHoursSubprojects > myPlannedHours)
        {
            return false;
        }
    }

    return isConsistent;
}
