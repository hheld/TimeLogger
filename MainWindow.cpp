#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

#include "Project.h"
#include "ProjectModel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    projectModel = new ProjectModel();
    ui->treeView_projects->setModel(projectModel);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete projectModel;
}

void MainWindow::on_toolButton_addProject_clicked()
{
    QModelIndex index = ui->treeView_projects->currentIndex();

    int row = projectModel->rowCount(index);

    projectModel->insertRow(row, index);

    QModelIndex indexOfNewProject = projectModel->index(row, 0, index);

    ui->treeView_projects->setExpanded(index, true);
    ui->treeView_projects->setCurrentIndex(indexOfNewProject);
    ui->treeView_projects->edit(indexOfNewProject);
    ui->treeView_projects->expand(indexOfNewProject);
}

void MainWindow::on_toolButton_removeProject_clicked()
{
    QModelIndex index = ui->treeView_projects->currentIndex();

    projectModel->removeRow(index.row(), index.parent());
}
