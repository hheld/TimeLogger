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
