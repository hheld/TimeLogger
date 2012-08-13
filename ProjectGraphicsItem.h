#ifndef PROJECTGRAPHICSITEM_H
#define PROJECTGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QDateTime>

class ProjectGraphicsItem : public QGraphicsItem
{
public:
    explicit ProjectGraphicsItem(ProjectGraphicsItem *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void SetProjectInfo(const QString &name, const QDateTime &start, const QDateTime &end, const int &numOfProjectsThisDay, const int &indexOfThisProject);

private:
    QString projectName;
    QDateTime start;
    QDateTime end;

    int numOfProjectsThisDay;
    int indexOfThisProject;
};

#endif // PROJECTGRAPHICSITEM_H
