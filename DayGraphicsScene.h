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

    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    double minX;
    double maxX;

    double minY;
    double maxY;

    void ItemModified();

signals:
    void itemChanged();
    void itemRemoved(const QDate &date);

public slots:

private:
    void drawBackground(QPainter *painter, const QRectF &rect);

    QTime startWorkDay;
    double workingHoursPerDay;
};

#endif // DAYGRAPHICSSCENE_H
