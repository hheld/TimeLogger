#ifndef PROJECTGRAPHICSITEM_H
#define PROJECTGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QDateTime>

class ProjectDatabase;

class ProjectGraphicsItem : public QGraphicsItem
{
public:
    explicit ProjectGraphicsItem(ProjectDatabase *db, ProjectGraphicsItem *parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void SetProjectInfo(const QString &name, const QDateTime &start, const QDateTime &end, const int &numOfProjectsThisDay, const int &indexOfThisProject);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    /** It's OK to call this function on any item. It only commits something to the database if there has been an actual change.
     */
    void UpdateDatabaseEntry();

private:
    QString originalProjectName;
    QDateTime originalStart;
    QDateTime originalEnd;

    QString projectName;
    QDateTime start;
    QDateTime end;

    int numOfProjectsThisDay;
    int indexOfThisProject;

    bool isLeftEdge;
    bool doMove;
    bool doStretch;
    bool hasChanged;

    double originalXPos;

    ProjectDatabase *db;

    void UpdateToolTip();
};

#endif // PROJECTGRAPHICSITEM_H
