#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QVector>

namespace Ui {
class MainWindow;
}

class Project;
class ProjectModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toolButton_addProject_clicked();

private:
    Ui::MainWindow *ui;

    QVector<Project*> projects;
    ProjectModel *projectModel;

    void initTestProjects();
};

#endif // MAINWINDOW_H
