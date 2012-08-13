#ifndef DAYVIEW_H
#define DAYVIEW_H

#include <QWidget>

namespace Ui {
class DayView;
}

class DayGraphicsScene;
class ProjectDatabase;
class QDate;

class DayView : public QWidget
{
    Q_OBJECT

public:
    explicit DayView(QWidget *parent = 0);
    ~DayView();

    void SetProjectDatabase(ProjectDatabase *db);

private slots:
    void on_dateEdit_selectDay_dateChanged(const QDate &date);

private:
    Ui::DayView *ui;

    DayGraphicsScene *dayScene;
    ProjectDatabase *db;
};

#endif // DAYVIEW_H
