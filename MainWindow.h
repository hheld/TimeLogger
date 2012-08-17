#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QModelIndex>
#include <QSystemTrayIcon>
#include <QTime>

namespace Ui {
class MainWindow;
}

class Project;
class ProjectModel;
class LineEditDelegate;
class ProjectDatabase;
class Report;
class QMenu;
class QAction;
class DayView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void stoppedWorking();

public slots:
    void SaveProjectXMLFile();
    void on_toolButton_stopWorking_clicked();

private slots:
    void on_toolButton_addProject_clicked();

    void on_toolButton_removeProject_clicked();

    void updateLabelCurrentProject(const QModelIndex &index = QModelIndex());

    void on_toolButton_startWorking_clicked();

    void addSecondToCurrentProject();

    void on_actionReport_triggered();

    void systemTrayIconClicked(const QSystemTrayIcon::ActivationReason &reason);

    void trayAction_quit_triggered();

    void askUserIfStillWorking();

    void on_actionDay_view_triggered();

    void on_actionSettings_triggered();

    void on_actionWorked_hours_from_DB_triggered();

private:
    Ui::MainWindow *ui;

    ProjectModel *projectModel;
    LineEditDelegate *lineEditDelegate;

    Report *report;
    DayView *dayView;

    QModelIndex currentProjectIndex;
    Project *currentlySelectedProject;
    bool isCurrentlyWorking;

    ProjectDatabase *pdb;

    QSystemTrayIcon *sysTrayIcon;
    QMenu *systemTrayMenu;
    QAction *trayAction_quit;

    // settings
    int numOfWorkingHoursPerDay;
    QTime startOfWorkingDay;
    int hoursBeforeEndOfBudgetWarning;
    int intervalsOfRemindersInMinutes;

    // private member functions
    void initSettingsFolder() const;

    void OpenProjectXMLFile();
    void SetupSystemTrayIcon();

    void closeEvent(QCloseEvent *);

    void ReadSettings();
    void WriteSettings() const;
};

#endif // MAINWINDOW_H
