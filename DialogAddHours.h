#ifndef DIALOGADDHOURS_H
#define DIALOGADDHOURS_H

#include <QDialog>

namespace Ui {
class DialogAddHours;
}

class DialogAddHours : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddHours(QWidget *parent = 0);
    ~DialogAddHours();

    void SetListOfWorkableProjects(const QStringList &projects);

    QTime Start() const;
    QTime End() const;
    QString Name() const;

private:
    Ui::DialogAddHours *ui;
};

#endif // DIALOGADDHOURS_H
