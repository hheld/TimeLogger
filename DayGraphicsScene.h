#ifndef DAYGRAPHICSSCENE_H
#define DAYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QTime>

class QDateTime;

class DayGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit DayGraphicsScene(QObject *parent = 0);

    double MapTimeToXCoord(const QDateTime &time) const;
    QTime MapXCoordToTime(const double &x) const;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    double minX;
    double maxX;

    double minY;
    double maxY;

    void ItemModified();

    void SetStartWorkDay(QTime *time);
    void SetWorkingHoursPerDay(int *hours);

signals:
    void itemChanged();
    void itemRemoved(const QDate &date);
    void doubleClickedOnBackground(const QTime &time);

public slots:

private:
    void drawBackground(QPainter *painter, const QRectF &rect);

    QTime *startWorkDay;
    int *workingHoursPerDay;
};

#endif // DAYGRAPHICSSCENE_H
