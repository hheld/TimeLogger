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
    void itemChanged();

    void on_dateEdit_selectDay_dateChanged(const QDate &date);

    void on_toolButton_commitToDb_clicked();

private:
    Ui::DayView *ui;

    DayGraphicsScene *dayScene;
    ProjectDatabase *db;

    void paintEvent(QPaintEvent *);

    bool initialSettingOfDateInPaintEventDone;
};

#endif // DAYVIEW_H
