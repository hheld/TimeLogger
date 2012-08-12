#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDir>
#include <QTimer>
#include <QMenu>
#include <QApplication>
#include <QDebug>

#include "Project.h"
#include "ProjectModel.h"
#include "XMLProjectsWriter.h"
#include "XMLProjectsReader.h"
#include "LineEditDelegate.h"
#include "ProjectDatabase.h"
#include "Report.h"
#include "DayView.h"

void MainWindow::SetupSystemTrayIcon()
{
    sysTrayIcon->setIcon(QIcon(":/icons/Resources/AppIcon.svg"));

    systemTrayMenu = new QMenu(this);

    trayAction_quit = new QAction(tr("Quit"), systemTrayMenu);

    systemTrayMenu->addAction(trayAction_quit);

    // make sure everything is saved before we quit
    connect(trayAction_quit, SIGNAL(triggered()), this, SLOT(trayAction_quit_triggered()));

    // finally, just quit
    connect(trayAction_quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    // open window if the message bubble is clicked
    connect(sysTrayIcon, SIGNAL(messageClicked()), this, SLOT(show()));

    sysTrayIcon->setContextMenu(systemTrayMenu);

    // ask the user every 30 minutes if he/she is still working on the same project
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(askUserIfStillWorking()));
    timer->start(1000*60*30);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    report(0),
    dayView(0),
    currentlySelectedProject(0),
    isCurrentlyWorking(false),
    pdb(0),
    sysTrayIcon(0),
    systemTrayMenu(0),
    trayAction_quit(0)
{
    ui->setupUi(this);

    pdb = new ProjectDatabase();
    sysTrayIcon = new QSystemTrayIcon(this);

    report = new Report(pdb);
    dayView = new DayView;

    projectModel = new ProjectModel();
    ui->treeView_projects->setModel(projectModel);

    lineEditDelegate = new LineEditDelegate(this);
    ui->treeView_projects->setItemDelegateForColumn(0, lineEditDelegate);
    ui->treeView_projects->setItemDelegateForColumn(1, lineEditDelegate);
    ui->treeView_projects->setItemDelegateForColumn(2, lineEditDelegate);
    ui->treeView_projects->setItemDelegateForColumn(3, lineEditDelegate);

    connect(ui->treeView_projects, SIGNAL(clicked(QModelIndex)), this, SLOT(updateLabelCurrentProject(QModelIndex)));
    connect(ui->treeView_projects, SIGNAL(clickedOutsideOfAnyRow()), this, SLOT(updateLabelCurrentProject()));
    connect(projectModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateLabelCurrentProject(QModelIndex)));

    initSettingsFolder();

    updateLabelCurrentProject();

    ui->toolButton_startWorking->setEnabled(false);
    ui->toolButton_stopWorking->setEnabled(false);

    // automatically save project XML every 10 minutes
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(SaveProjectXMLFile()));
    timer->start(60*10*1000);

    OpenProjectXMLFile();

    SetupSystemTrayIcon();
    sysTrayIcon->show();

    connect(sysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(systemTrayIconClicked(QSystemTrayIcon::ActivationReason)));

    // disable close button of window
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete projectModel;
    delete lineEditDelegate;
    delete pdb;
    delete report;
    delete dayView;
    delete sysTrayIcon;
    delete systemTrayMenu;
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

    bool success = projectModel->removeRow(index.row(), index.parent());

    if(success)
    {
        currentlySelectedProject = 0;
        currentProjectIndex = QModelIndex();
    }
}

void MainWindow::SaveProjectXMLFile()
{
    if(projectModel->Root()->NumOfSubprojects() >= 0)
    {
        XMLProjectsWriter projectsWriter(Project::pathToProjectXML);
        projectsWriter.SetRoot(projectModel->Root());
        projectsWriter.Write();
    }
}

void MainWindow::OpenProjectXMLFile()
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

        ui->toolButton_startWorking->setEnabled(false);
    }
    else
    {
        currentProjectName = p->Name() + " (" + p->Parent()->Name() + ")";
        currentlySelectedProject = p;

        currentProjectIndex = index.sibling(index.row(), 3);

        if(!p->HasChild())
        {
            ui->toolButton_startWorking->setEnabled(true);
        }
        else
        {
            ui->toolButton_startWorking->setEnabled(false);
        }
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
    on_toolButton_stopWorking_clicked();
    SaveProjectXMLFile();

    report->close();
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

    ui->statusBar->showMessage(tr("Currently working on project '%1'.").arg(currentlySelectedProject->Name() + " (" + currentlySelectedProject->Parent()->Name() + ")"));

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

    ui->treeView_projects->dataChanged(ProjectModel::TopLevelIndex(currentProjectIndex), currentProjectIndex);
}

void MainWindow::on_actionReport_triggered()
{
    report->show();
}

void MainWindow::systemTrayIconClicked(const QSystemTrayIcon::ActivationReason &reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::Trigger:
        if(isVisible())
        {
            hide();
            report->hide();
        }
        else
        {
            show();
        }

        break;

    case QSystemTrayIcon::Context:
        systemTrayMenu->show();
        break;

    default:
        break;
    }
}

void MainWindow::trayAction_quit_triggered()
{
    SaveProjectXMLFile();
    on_toolButton_stopWorking_clicked();
}

void MainWindow::askUserIfStillWorking()
{
    if(currentlySelectedProject && isCurrentlyWorking)
    {
        sysTrayIcon->showMessage(tr("TimeLogger"), tr("Still working on %1 (%2)?").arg(currentlySelectedProject->Name()).arg(currentlySelectedProject->Parent()->Name()));
    }
    else
    {
        sysTrayIcon->showMessage(tr("TimeLogger"), tr("Are you really working on nothing currently??"));
    }
}

void MainWindow::on_actionDay_view_triggered()
{
    dayView->show();
}
