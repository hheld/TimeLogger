#include "XMLProjectsReader.h"
#include "Project.h"

#include <QXmlStreamReader>
#include <QFile>
#include <QList>
#include <QDebug>

XMLProjectsReader::XMLProjectsReader(const QString &xmlFileName) :
    xmlFileName(xmlFileName)
{
}

XMLProjectsReader::~XMLProjectsReader()
{
}

void XMLProjectsReader::SetRoot(Project *root)
{
    this->root = root;
}

bool XMLProjectsReader::Read() const
{
    QFile xmlFile(xmlFileName);

    if(!xmlFile.open(QFile::ReadOnly))
    {
        qDebug() << "Cannot open file '" << xmlFileName << "' for reading.";
        return false;
    }

    QXmlStreamReader xmlStreamReader(&xmlFile);

    QList<Project*> currentParent;
    currentParent << root;

    while(!xmlStreamReader.atEnd())
    {
        xmlStreamReader.readNext();

        if(xmlStreamReader.isStartElement())
        {
            if(xmlStreamReader.name() == "Project")
            {
                QXmlStreamAttributes attr = xmlStreamReader.attributes();

                QString projectName = attr.value("name").toString();
                double totalHours = attr.value("totalHours").toString().toDouble();
                double plannedHours = attr.value("plannedHours").toString().toDouble();

                Project *newProject = new Project(projectName);
                newProject->TotalHours(totalHours);
                newProject->PlannedHours(plannedHours);

                currentParent.last()->AddSubProject(newProject);
                currentParent.append(newProject);
            }
        }
        else if(xmlStreamReader.isEndElement())
        {
            if(xmlStreamReader.name() == "Project")
            {
                currentParent.removeLast();
            }
        }
    }

    return true;
}
