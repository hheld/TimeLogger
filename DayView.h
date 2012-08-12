#ifndef DAYVIEW_H
#define DAYVIEW_H

#include <QWidget>

namespace Ui {
class DayView;
}

class DayView : public QWidget
{
    Q_OBJECT
    
public:
    explicit DayView(QWidget *parent = 0);
    ~DayView();
    
private:
    Ui::DayView *ui;
};

#endif // DAYVIEW_H
