#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QModelIndex>

namespace Ui {
class MainWindow;
}

class Project;
class ProjectModel;
class LineEditDelegate;
class ProjectDatabase;
class Report;

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

private:
    Ui::MainWindow *ui;

    ProjectModel *projectModel;
    LineEditDelegate *lineEditDelegate;

    Report *report;

    QModelIndex currentProjectIndex;
    Project *currentlySelectedProject;
    bool isCurrentlyWorking;

    ProjectDatabase *pdb;

    void initSettingsFolder() const;

    void OpenProjectXMLFile();

    void closeEvent(QCloseEvent *);
};

#endif // MAINWINDOW_H
