#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Project.h"
#include "ProjectModel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    projectModel = new ProjectModel(&projects);
    ui->treeView_projects->setModel(projectModel);

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
