#include "ProjectModel.h"
#include "Project.h"

#include <QDebug>

ProjectModel::ProjectModel(int *hoursBeforeEndOfBudgetWarning, QObject *parent) :
    QAbstractItemModel(parent),
    root(new Project(hoursBeforeEndOfBudgetWarning, "root")),
    hoursBeforeEndOfBudgetWarning(hoursBeforeEndOfBudgetWarning)
{
    connect(this, SIGNAL(totalOrPlannedHoursEdited(QModelIndex)), this, SLOT(updateHoursOfAllParents(QModelIndex)));
}

ProjectModel::~ProjectModel()
{
    delete root;
}

Project *ProjectModel::AddProject(const QString &name, Project *parent)
{
    if(parent == 0)
    {
        parent = root;
    }

    Project *newProject = new Project(hoursBeforeEndOfBudgetWarning, name);

    parent->AddSubProject(newProject);

    return newProject;
}

Project *ProjectModel::GetProject(const QModelIndex &index) const
{
    if(index.isValid())
    {
        Project *p = static_cast<Project*>(index.internalPointer());

        if(p)
        {
            return p;
        }
    }

    return root;
}

Project *ProjectModel::Root() const
{
    return root;
}

QModelIndex ProjectModel::TopLevelIndex(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return QModelIndex();
    }

    QModelIndex topLevelParent = QModelIndex();
    QModelIndex parent = index.parent();

    while(parent.isValid())
    {
        topLevelParent = parent;

        parent = parent.parent();
    }

    return topLevelParent.sibling(topLevelParent.row(), 3);
}

void ProjectModel::AddProject(Project *p)
{
    p->Parent()->AddSubProject(p);
}

void ProjectModel::RemoveProject(Project *p)
{
    if(p==root || p==0)
    {
        return;
    }

    delete p;
}

QModelIndex ProjectModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    Project *parentProject = GetProject(parent);
    Project *childProject = parentProject->SubProject(row);

    if(childProject)
    {
        return createIndex(row, column, childProject);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex ProjectModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
    {
        return QModelIndex();
    }

    Project *childProject = GetProject(child);
    Project *parentProject = childProject->Parent();

    if(parentProject == root)
    {
        return QModelIndex();
    }

    return createIndex(parentProject->SubprojectNumber(), 0, parentProject);
}

int ProjectModel::rowCount(const QModelIndex &parent) const
{
    Project *parentProject = GetProject(parent);

    return parentProject->NumOfSubprojects();
}

int ProjectModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 4;
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role!=Qt::DisplayRole)
    {
        return QVariant();
    }

    QVariant actualData;

    Project *currentProject = GetProject(index);

    switch(index.column())
    {
    case 0:
        actualData = currentProject->Name();
        break;
    case 1:
        actualData = currentProject->TotalHours();
        break;
    case 2:
        actualData = currentProject->PlannedHours();
        break;
    case 3:
        actualData = currentProject->WorkedHours();
        break;
    default:
        break;
    }

    return actualData;
}

QVariant ProjectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole or orientation != Qt::Horizontal)
    {
        return QVariant();
    }

    switch(section)
    {
    case 0:
        return tr("Project");
        break;
    case 1:
        return tr("Total hours");
        break;
    case 2:
        return tr("Planned for me");
        break;
    case 3:
        return tr("Worked hours");
        break;
    default:
        return QVariant();
    }
}

Qt::ItemFlags ProjectModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    Project *p = GetProject(index);

    if(!p->HasChild())
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

bool ProjectModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != Qt::EditRole)
    {
        return false;
    }

    Project *currentProject = GetProject(index);

    switch(index.column())
    {
    case 0:
        currentProject->Name(value.toString());
        break;
    case 1:
        currentProject->TotalHours(value.toDouble());
        emit totalOrPlannedHoursEdited(index);
        break;
    case 2:
        currentProject->PlannedHours(value.toDouble());
        emit totalOrPlannedHoursEdited(index);
        break;
    case 3:
        currentProject->WorkedHours(value.toDouble());
        emit totalOrPlannedHoursEdited(index);
        break;
    }

    emit(dataChanged(index, index));

    return true;
}

bool ProjectModel::insertRow(int row, const QModelIndex &parent)
{
    Project *parentProject = GetProject(parent);

    beginInsertRows(parent, row, row);

    Project *newProject = new Project(hoursBeforeEndOfBudgetWarning, tr("Enter project title here"), parentProject);

    AddProject(newProject);

    endInsertRows();

    return true;
}

bool ProjectModel::removeRow(int row, const QModelIndex &parent)
{
    if(row<0)
    {
        return false;
    }

    Project *p = GetProject(parent);

    beginRemoveRows(parent, row, row);

    Project *projectToBeRemoved = p->SubProject(row);

    p->RemoveSubProject(projectToBeRemoved);
    RemoveProject(projectToBeRemoved);

    endRemoveRows();

    return true;
}


void ProjectModel::updateHoursOfAllParents(const QModelIndex &index)
{
    QModelIndex topParent = TopLevelIndex(index);

    Project *p = GetProject(topParent);

    p->MakeHoursConsistent();

    emit dataChanged(topParent.sibling(topParent.row(), 1), index.sibling(index.row(), 3));
}
