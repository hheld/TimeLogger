#ifndef REPORT_H
#define REPORT_H

#include <QWidget>

namespace Ui {
class Report;
}

class ProjectDatabase;

class Report : public QWidget
{
    Q_OBJECT

public:
    explicit Report(ProjectDatabase *db, QWidget *parent = 0);
    ~Report();

public slots:
    void clearHtml();

private slots:
    void on_pushButton_generateReport_clicked();

private:
    Ui::Report *ui;

    ProjectDatabase *db;

    QString html;

    void AddHtmlHeader();
    void AddHtmlFooter();

    void GetMapProjectName2RowIndex(QMap<QString, int> &projectName2RowIndex, const QList<QList<QPair<QString, double> > > &listOfProjects);
};

#endif // REPORT_H
