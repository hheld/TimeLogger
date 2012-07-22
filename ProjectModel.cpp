#include "ProjectModel.h"
#include "Project.h"

#include <QDebug>

ProjectModel::ProjectModel(QVector<Project *> *projects, QObject *parent) :
    QAbstractItemModel(parent),
    projects(projects),
    root(new Project("root"))
{
}

ProjectModel::~ProjectModel()
{
    delete root;
}

void ProjectModel::AddProject(Project *p, Project *parent)
{
    if(parent==0 && p->Parent()==0)
    {
        p->Parent(root);
    }

    projects->append(p);
    p->Parent()->AddSubProject(p);
}

QModelIndex ProjectModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    Project *parentProject;

    if(!parent.isValid())
    {
        parentProject = root;
    }
    else
    {
        parentProject = static_cast<Project*>(parent.internalPointer());
    }

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

    Project *childProject = static_cast<Project*>(child.internalPointer());
    Project *parentProject = childProject->Parent();

    if(parentProject == root)
    {
        return QModelIndex();
    }

    return createIndex(projects->indexOf(parentProject), 0, parentProject);
}

int ProjectModel::rowCount(const QModelIndex &parent) const
{
    Project *parentProject;

    if(!parent.isValid())
    {
        parentProject = root;
    }
    else
    {
        parentProject = static_cast<Project*>(parent.internalPointer());
    }

    return parentProject->NumOfSubprojects();
}

int ProjectModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}

QVariant ProjectModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || role!=Qt::DisplayRole)
    {
        return QVariant();
    }

    return static_cast<Project*>(index.internalPointer())->Name();
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

    Project *currentProject = static_cast<Project*>(index.internalPointer());
    currentProject->Name(value.toString());

    emit(dataChanged(index, index));

    return true;
}

bool ProjectModel::insertRow(int row, const QModelIndex &parent)
{
    Project *parentProject;

    if(!parent.isValid())
    {
        beginInsertRows(index(row, 0), row, 1);

        parentProject = root;
    }
    else
    {
        beginInsertRows(parent, row, 1);

        parentProject = static_cast<Project*>(parent.internalPointer());
    }

    Project *newProject = new Project(tr("Enter project title here"), parentProject);

    AddProject(newProject, parentProject);

    endInsertRows();

    return true;
}
