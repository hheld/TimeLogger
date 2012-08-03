#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDir>
#include <QTimer>
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
    isCurrentlyWorking(false),
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
    connect(projectModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateLabelCurrentProject(QModelIndex)));

    initSettingsFolder();

    pdb = new ProjectDatabase();

    updateLabelCurrentProject();

    ui->toolButton_stopWorking->setEnabled(false);

    // automatically save project XML every 10 minutes
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_actionSave_triggered()));
    timer->start(60*10*1000);

    on_actionOpen_triggered();
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

    ui->treeView_projects->setExpanded(index, false);
    ui->treeView_projects->expand(index);
    ui->treeView_projects->setCurrentIndex(indexOfNewProject);
    ui->treeView_projects->edit(indexOfNewProject);

    updateLabelCurrentProject(indexOfNewProject);
}

void MainWindow::on_toolButton_removeProject_clicked()
{
    QModelIndex index = ui->treeView_projects->currentIndex();

    projectModel->removeRow(index.row(), index.parent());
}

void MainWindow::on_actionSave_triggered()
{
    if(projectModel->Root()->NumOfSubprojects() > 0)
    {
        XMLProjectsWriter projectsWriter(Project::pathToProjectXML);
        projectsWriter.SetRoot(projectModel->Root());
        projectsWriter.Write();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    XMLProjectsReader projectsReader(Project::pathToProjectXML);
    projectsReader.SetRoot(projectModel->Root());
    projectsReader.Read();

    ui->treeView_projects->expandAll();
}

void MainWindow::updateLabelCurrentProject(const QModelIndex &index)
{
    if(isCurrentlyWorking)
    {
        return;
    }

    Project *p = projectModel->GetProject(index);

    QString currentProjectName;

    if(p == projectModel->Root())
    {
        currentProjectName = "None";
        currentlySelectedProject = 0;
    }
    else
    {
        currentProjectName = p->Name();
        currentlySelectedProject = p;

        currentProjectIndex = index.sibling(index.row(), 3);
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

void MainWindow::closeEvent(QCloseEvent *)
{
    on_actionSave_triggered();
}

void MainWindow::on_toolButton_startWorking_clicked()
{
    if(currentlySelectedProject)
    {
        pdb->LogWorkingStart(currentlySelectedProject, QDateTime::currentDateTime());
    }
    else
    {
        return;
    }

    isCurrentlyWorking = true;

    ui->toolButton_startWorking->setEnabled(false);
    ui->toolButton_stopWorking->setEnabled(true);

    ui->statusBar->showMessage(tr("Currently working on project '%1'.").arg(currentlySelectedProject->Name()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(addSecondToCurrentProject()));
    connect(this, SIGNAL(stoppedWorking()), timer, SLOT(stop()));
    timer->start(1000);
}

void MainWindow::on_toolButton_stopWorking_clicked()
{
    if(currentlySelectedProject)
    {
        pdb->LogWorkingEnd(currentlySelectedProject, QDateTime::currentDateTime());
    }

    isCurrentlyWorking = false;

    ui->toolButton_startWorking->setEnabled(true);
    ui->toolButton_stopWorking->setEnabled(false);

    updateLabelCurrentProject(ui->treeView_projects->currentIndex());

    ui->statusBar->clearMessage();

    emit stoppedWorking();
}

void MainWindow::addSecondToCurrentProject()
{
    currentlySelectedProject->AddWorkedHours(1./3600.);

//    ui->treeView_projects->update(ProjectModel::TopLevelIndex(currentProjectIndex));
    ui->treeView_projects->dataChanged(ProjectModel::TopLevelIndex(currentProjectIndex), currentProjectIndex);
}
