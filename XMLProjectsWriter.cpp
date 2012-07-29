#include "XMLProjectsWriter.h"
#include "Project.h"

#include <QXmlStreamWriter>
#include <QFile>
#include <QDebug>

XMLProjectsWriter::XMLProjectsWriter(const QString &xmlfileName) :
    xmlFileName(xmlfileName),
    root(0),
    xmlStreamWriter(0),
    xmlFile(0)
{
    InitStreamWriter();
}

XMLProjectsWriter::~XMLProjectsWriter()
{
    delete xmlStreamWriter; xmlStreamWriter = 0;
    delete xmlFile; xmlFile = 0;
}

void XMLProjectsWriter::SetRoot(Project *root)
{
    this->root = root;
}

bool XMLProjectsWriter::Write() const
{
    if(!root)
    {
        return false;
    }

    xmlStreamWriter->writeStartDocument();
    xmlStreamWriter->writeStartElement("Projects");

    WriteProject(root);

    xmlStreamWriter->writeEndElement(); // Projects
    xmlStreamWriter->writeEndDocument();

    return true;
}

void XMLProjectsWriter::InitStreamWriter()
{
    xmlFile = new QFile(xmlFileName);

    if(!xmlFile->open(QFile::WriteOnly))
    {
        qDebug() << "Could not open file '" << xmlFileName << "' for writing.";
        return;
    }

    xmlStreamWriter = new QXmlStreamWriter(xmlFile);

    xmlStreamWriter->setAutoFormatting(true);
}

void XMLProjectsWriter::WriteProject(Project *p) const
{
    // we don't need an artificial project named 'root'
    if(p!=root)
    {
        xmlStreamWriter->writeStartElement("Project");

        xmlStreamWriter->writeAttribute("name", p->Name());
        xmlStreamWriter->writeAttribute("totalHours", QString::number(p->TotalHours()));
        xmlStreamWriter->writeAttribute("plannedHours", QString::number(p->PlannedHours()));
        xmlStreamWriter->writeAttribute("workedHours", QString::number(p->WorkedHours()));
    }

    const QVector<Project*> &allSubprojects = p->AllSubprojects();

    foreach(Project *sp, allSubprojects)
    {
        WriteProject(sp);
    }

    xmlStreamWriter->writeEndElement(); // Project
}
