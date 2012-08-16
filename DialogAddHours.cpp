#include "DialogAddHours.h"
#include "ui_DialogAddHours.h"

DialogAddHours::DialogAddHours(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddHours)
{
    ui->setupUi(this);
}

DialogAddHours::~DialogAddHours()
{
    delete ui;
}

void DialogAddHours::SetListOfWorkableProjects(const QStringList &projects)
{
    ui->comboBox_project->addItems(projects);
}

QTime DialogAddHours::Start() const
{
    return ui->timeEdit_start->time();
}

QTime DialogAddHours::End() const
{
    return ui->timeEdit_end->time();
}

QString DialogAddHours::Name() const
{
    return ui->comboBox_project->currentText();
}

void DialogAddHours::SetStart(const QTime &time)
{
    ui->timeEdit_start->setTime(time);
    ui->timeEdit_end->setTime(time.addSecs(3600));
}
