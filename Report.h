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

private slots:
    void on_pushButton_generateReport_clicked();

private:
    Ui::Report *ui;

    ProjectDatabase *db;
};

#endif // REPORT_H
