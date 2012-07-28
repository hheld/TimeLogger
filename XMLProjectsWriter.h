#ifndef XMLPROJECTSWRITER_H
#define XMLPROJECTSWRITER_H

#include <QString>

class Project;
class QXmlStreamWriter;
class QFile;

class XMLProjectsWriter
{
public:
    explicit XMLProjectsWriter(const QString &xmlfileName);
    ~XMLProjectsWriter();

    void SetRoot(Project *root);

    bool Write() const;

private:
    QString xmlFileName;

    Project *root;

    QXmlStreamWriter *xmlStreamWriter;

    QFile *xmlFile;

    void InitStreamWriter();
    void WriteProject(Project *p) const;
};

#endif // XMLPROJECTSWRITER_H
