#ifndef REPORT_H
#define REPORT_H

#include <QWidget>

namespace Ui {
class Report;
}

class Report : public QWidget
{
    Q_OBJECT
    
public:
    explicit Report(QWidget *parent = 0);
    ~Report();
    
private:
    Ui::Report *ui;
};

#endif // REPORT_H
