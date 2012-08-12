#ifndef DAYVIEW_H
#define DAYVIEW_H

#include <QWidget>

namespace Ui {
class DayView;
}

class DayGraphicsScene;

class DayView : public QWidget
{
    Q_OBJECT

public:
    explicit DayView(QWidget *parent = 0);
    ~DayView();

private:
    Ui::DayView *ui;

    DayGraphicsScene *dayScene;
};

#endif // DAYVIEW_H
