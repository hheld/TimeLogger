#include "Project.h"

#include <QDir>
#include <QStringList>

QString Project::pathToProjectXML = QDir::homePath() + "/.timelogger/projects.xml";

Project::Project(int *hoursBeforeEndOfBudgetWarning, const QString &name, Project *parent) :
    name(name),
    parent(parent),
    totalBudgetHours(0.),
    myPlannedHours(0.),
    workedHours(0.),
    hoursBeforeEndOfBudgetWarning(hoursBeforeEndOfBudgetWarning)
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

QString Project::DbName() const
{
    QString dbName = name;

    // replace whitespaces with underscores
    dbName.replace(" ", "_");

    // to get a unique database name, we prepend all parents' names delimited by '_' to the name
    Project *p = parent;

    while(p != 0 && p->Name() != "root")
    {
        dbName.prepend(p->Name() + "_");

        p = p->parent;
    }

    return dbName;
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

void Project::AddWorkedHours(const double &seconds)
{
    workedHours += seconds;

    // also update parent project for consistency if there is any
    if(parent)
    {
        parent->AddWorkedHours(seconds);
    }
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

Project::State Project::GetState() const
{
    if(workedHours < myPlannedHours - *hoursBeforeEndOfBudgetWarning)
    {
        return InBudet;
    }
    else if(workedHours >= myPlannedHours - *hoursBeforeEndOfBudgetWarning && workedHours <= myPlannedHours)
    {
        return ApproachingEndOfBudget;
    }
    else
    {
        return ExceededBudget;
    }
}

bool Project::HasChild() const
{
    return subProjects.size() > 0;
}

void Project::MakeHoursConsistent()
{
    if(HasChild())
    {
        myPlannedHours = 0.;
        totalBudgetHours = 0.;
        workedHours = 0.;

        foreach(Project *sp, subProjects)
        {
            sp->MakeHoursConsistent();

            myPlannedHours += sp->myPlannedHours;
            totalBudgetHours += sp->totalBudgetHours;
            workedHours += sp->workedHours;
        }
    }
}

QStringList Project::GetAllWorkableProjects() const
{
    QStringList allProjectNames;

    const QVector<Project*> &sp = subProjects;

    if(sp.isEmpty())
    {
        allProjectNames << DbName();
        return allProjectNames;
    }

    foreach(Project *p, sp)
    {
        allProjectNames += p->GetAllWorkableProjects();
    }

    return allProjectNames;
}

int *Project::PtrHoursBeforeEndOfBudget() const
{
    return hoursBeforeEndOfBudgetWarning;
}
