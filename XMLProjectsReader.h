#ifndef XMLPROJECTSREADER_H
#define XMLPROJECTSREADER_H

#include <QString>

class Project;

class XMLProjectsReader
{
public:
    explicit XMLProjectsReader(const QString &xmlFileName);
    ~XMLProjectsReader();

    void SetRoot(Project *root);

    bool Read() const;

private:
    QString xmlFileName;

    Project *root;
};

#endif // XMLPROJECTSREADER_H
