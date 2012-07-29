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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toolButton_addProject_clicked();

    void on_toolButton_removeProject_clicked();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void updateLabelCurrentProject(const QModelIndex &index = QModelIndex());

private:
    Ui::MainWindow *ui;

    ProjectModel *projectModel;
    LineEditDelegate *lineEditDelegate;
};

#endif // MAINWINDOW_H
