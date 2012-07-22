#ifndef PROJECTTREEVIEW_H
#define PROJECTTREEVIEW_H

#include <QTreeView>

class ProjectTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit ProjectTreeView(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *event);

signals:

public slots:

};

#endif // PROJECTTREEVIEW_H
