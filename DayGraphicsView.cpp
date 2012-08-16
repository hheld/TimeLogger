#include "DayGraphicsView.h"

DayGraphicsView::DayGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}
