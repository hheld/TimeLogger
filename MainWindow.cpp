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

    projectModel = new ProjectModel(&projects);
    ui->treeView_projects->setModel(projectModel);

    connect(projectModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), ui->treeView_projects, SLOT(update()));

    initTestProjects();
}

MainWindow::~MainWindow()
{
    delete ui;

    qDeleteAll(projects);
    projects.clear();

    delete projectModel;
}

void MainWindow::initTestProjects()
{
    projectModel->AddProject(new Project("P1"));
    projectModel->AddProject(new Project("P2"));
    projectModel->AddProject(new Project("P3"));
    projectModel->AddProject(new Project("Child of P1", projects[0]));
    projectModel->AddProject(new Project("Child of child of P1", projects[3]));
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
}
