#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDir>
#include <QDebug>

#include "Project.h"
#include "ProjectModel.h"
#include "XMLProjectsWriter.h"
#include "XMLProjectsReader.h"
#include "LineEditDelegate.h"
#include "ProjectDatabase.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentlySelectedProject(0),
    pdb(0)
{
    ui->setupUi(this);

    projectModel = new ProjectModel();
    ui->treeView_projects->setModel(projectModel);

    lineEditDelegate = new LineEditDelegate(this);
    ui->treeView_projects->setItemDelegateForColumn(0, lineEditDelegate);
    ui->treeView_projects->setItemDelegateForColumn(1, lineEditDelegate);
    ui->treeView_projects->setItemDelegateForColumn(2, lineEditDelegate);

    connect(ui->treeView_projects, SIGNAL(clicked(QModelIndex)), this, SLOT(updateLabelCurrentProject(QModelIndex)));
    connect(ui->treeView_projects, SIGNAL(clickedOutsideOfAnyRow()), this, SLOT(updateLabelCurrentProject()));

    initSettingsFolder();

    pdb = new ProjectDatabase();

    updateLabelCurrentProject();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete projectModel;
    delete lineEditDelegate;
    delete pdb;
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

void MainWindow::on_actionSave_triggered()
{
    XMLProjectsWriter projectsWriter("../projects.xml");
    projectsWriter.SetRoot(projectModel->Root());
    projectsWriter.Write();
}

void MainWindow::on_actionOpen_triggered()
{
    XMLProjectsReader projectsReader("../projects.xml");
    projectsReader.SetRoot(projectModel->Root());
    projectsReader.Read();

    ui->treeView_projects->expandAll();
}

void MainWindow::updateLabelCurrentProject(const QModelIndex &index)
{
    Project *p = projectModel->GetProject(index);

    QString currentProjectName;

    if(p == projectModel->Root())
    {
        if(currentlySelectedProject)
        {
            pdb->LogWorkingEnd(currentlySelectedProject, QDateTime::currentDateTime());
        }

        currentProjectName = "None";
        currentlySelectedProject = 0;
    }
    else
    {
        if(currentlySelectedProject)
        {
            pdb->LogWorkingEnd(currentlySelectedProject, QDateTime::currentDateTime());
        }

        if(p)
        {
            pdb->LogWorkingStart(p, QDateTime::currentDateTime());
        }

        currentProjectName = p->Name();
        currentlySelectedProject = p;
    }

    ui->label_selectedProject->setText(currentProjectName);
}

void MainWindow::initSettingsFolder() const
{
    QString hiddenFolderName = QDir::homePath() + "/.timelogger";

    QDir hiddenFolder(hiddenFolderName);

    if(!hiddenFolder.exists())
    {
        hiddenFolder.mkdir(hiddenFolderName);
    }
}
