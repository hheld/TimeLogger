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

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QString projectName;
    QDateTime start;
    QDateTime end;

    int numOfProjectsThisDay;
    int indexOfThisProject;

    bool isLeftEdge;
    bool doMove;
    bool doStretch;

    double originalXPos;

    void UpdateToolTip();
};

#endif // PROJECTGRAPHICSITEM_H
