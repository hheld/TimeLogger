#include "ProjectModel.h"
#include "Project.h"

#include <QDebug>

ProjectModel::ProjectModel(QObject *parent) :
    QAbstractItemModel(parent),
    root(new Project("root"))
{
//    Project *P1 = AddProject("P1");
//    Project *P2 = AddProject("P2");
//    Project *P3 = AddProject("P3");
//    Project *P4 = AddProject("P4", P1);
//    Project *P5 = AddProject("P5", P2);
//    Project *P6 = AddProject("P6", P5);
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

    Project *newProject = new Project(name);

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

    return 3;
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

    return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
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
        break;
    case 2:
        currentProject->PlannedHours(value.toDouble());
        break;
    }

    emit(dataChanged(index, index));

    return true;
}

bool ProjectModel::insertRow(int row, const QModelIndex &parent)
{
    Project *parentProject = GetProject(parent);

    beginInsertRows(parent, row, row);

    Project *newProject = new Project(tr("Enter project title here"), parentProject);

    AddProject(newProject);

    endInsertRows();

    return true;
}

bool ProjectModel::removeRow(int row, const QModelIndex &parent)
{
    Project *p = GetProject(parent);

    beginRemoveRows(parent, row, row);

    Project *projectToBeRemoved = p->SubProject(row);

    p->RemoveSubProject(projectToBeRemoved);
    RemoveProject(projectToBeRemoved);

    endRemoveRows();

    return true;
}
