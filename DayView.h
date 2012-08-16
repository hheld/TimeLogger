#ifndef DAYVIEW_H
#define DAYVIEW_H

#include <QWidget>
#include <QTime>

namespace Ui {
class DayView;
}

class DayGraphicsScene;
class ProjectDatabase;
class QDate;
class Project;

class DayView : public QWidget
{
    Q_OBJECT

public:
    DayView(Project *root, QTime *startOfWorkDay, int *numOfWorkingHours, QWidget *parent = 0);
    ~DayView();

    void SetProjectDatabase(ProjectDatabase *db);

private slots:
    void itemChanged();

    void on_dateEdit_selectDay_dateChanged(const QDate &date);

    void on_toolButton_commitToDb_clicked();

    void toolButton_addProject_clicked(const QTime &time = QTime());

private:
    Ui::DayView *ui;

    DayGraphicsScene *dayScene;
    ProjectDatabase *db;

    /** this is needed for the list of all workable projects.
     */
    Project *root;

    void paintEvent(QPaintEvent *);

    bool initialSettingOfDateInPaintEventDone;

    QTime *startOfWorkDay;
    int *numOfWorkingHours;
};

#endif // DAYVIEW_H
