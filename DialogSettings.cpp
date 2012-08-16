#include "DialogSettings.h"
#include "ui_DialogSettings.h"

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

QTime DialogSettings::GetStartOfDay() const
{
    return ui->timeEdit_start->time();
}

int DialogSettings::GetNumOfWorkingHours() const
{
    return ui->spinBox_numOfWorkingHours->value();
}

int DialogSettings::GetHoursBeforeBudgetWarning() const
{
    return ui->spinBox_hoursToBudget->value();
}

int DialogSettings::GetIntervalsForReminder() const
{
    return ui->spinBox_intervalsReminder->value();
}

void DialogSettings::SetStartOfDay(const QTime &time)
{
    ui->timeEdit_start->setTime(time);
}

void DialogSettings::SetNumOfWorkingHours(const int &hours)
{
    ui->spinBox_numOfWorkingHours->setValue(hours);
}

void DialogSettings::SetHoursBeforeBudgetWarning(const int &hours)
{
    ui->spinBox_hoursToBudget->setValue(hours);
}

void DialogSettings::SetIntervalsForReminder(const int &minutes)
{
    ui->spinBox_intervalsReminder->setValue(minutes);
}
