#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = 0);
    ~DialogSettings();

    QTime GetStartOfDay() const;
    int GetNumOfWorkingHours() const;
    int GetHoursBeforeBudgetWarning() const;
    int GetIntervalsForReminder() const;

    void SetStartOfDay(const QTime &time);
    void SetNumOfWorkingHours(const int &hours);
    void SetHoursBeforeBudgetWarning(const int &hours);
    void SetIntervalsForReminder(const int &minutes);

private:
    Ui::DialogSettings *ui;
};

#endif // DIALOGSETTINGS_H
