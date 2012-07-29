#include "ProjectTreeView.h"

#include <QMouseEvent>
#include <QDebug>

ProjectTreeView::ProjectTreeView(QWidget *parent) :
    QTreeView(parent)
{
}

void ProjectTreeView::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();

    QModelIndex index = indexAt(pos);

    if(!index.isValid())
    {
        clearSelection();
        setCurrentIndex(index);

        emit clickedOutsideOfAnyRow();
    }

    QTreeView::mousePressEvent(event);
}
