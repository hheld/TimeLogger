#include "DayGraphicsScene.h"
#include "ProjectGraphicsItem.h"

#include <QPainter>
#include <QTime>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QDebug>

DayGraphicsScene::DayGraphicsScene(QObject *parent) :
    QGraphicsScene(parent),
    minX(0.),
    maxX(0.),
    minY(0.),
    maxY(0.),
    startWorkDay(9, 0, 0),
    workingHoursPerDay(9.)
{
}

double DayGraphicsScene::MapTimeToXCoord(const QDateTime &time) const
{
    QTime t = time.time();

    double secsFromStartOfDay = startWorkDay.secsTo(t);

    double workDayInSecs = workingHoursPerDay*3600.;

    double lambda = secsFromStartOfDay / workDayInSecs;

    double xCoord = minX + lambda * (maxX - minX);

    return xCoord;
}

QTime DayGraphicsScene::MapXCoordToTime(const double &x) const
{
    double lambda = (x - minX) / (maxX - minX);

    double workDayInSecs = workingHoursPerDay*3600.;

    double secsFromStartOfDay = lambda * workDayInSecs;

    return startWorkDay.addSecs(secsFromStartOfDay);
}

void DayGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsItem *i = itemAt(event->scenePos());

    if(i)
    {
        QMenu menu;

        QAction *a = menu.addAction("Remove");

        QDate date;

        if(menu.exec(event->screenPos()) == a)
        {
            ProjectGraphicsItem *p = dynamic_cast<ProjectGraphicsItem*>(i);

            date = p->Date();

            p->RemoveFromDb();

            removeItem(i);
            delete i;

            emit itemRemoved(date);

            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->ignore();
    }
}

void DayGraphicsScene::ItemModified()
{
    emit itemChanged();
}

void DayGraphicsScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QPointF tl = rect.topLeft();
    QPointF br = rect.bottomRight();

    double w = rect.width();
    double h = rect.height();

    QPointF diff_x(w, 0.);
    QPointF diff_y(0., h);

    QLineF xAxis(tl + 0.1*diff_x + 0.9*diff_y, br - 0.1*diff_x - 0.1*diff_y);

    minX = tl.x() + 0.1*w;
    maxX = tl.x() + 0.9*w;
    minY = tl.y() + 0.05*h;
    maxY = br.y() - 0.1*h;

    QVector<QLineF> verticalGridLines;

    for(int l=0; l<workingHoursPerDay+1; ++l)
    {
        verticalGridLines.append(QLineF(tl + (0.1 + 0.8/workingHoursPerDay*l)*diff_x + 0.05*diff_y, tl + (0.1 + 0.8/workingHoursPerDay*l)*diff_x + 0.95*diff_y));

        painter->drawText(QRectF(tl + (0.1 + 0.8/workingHoursPerDay*l - 0.4/workingHoursPerDay)*diff_x + 0.95*diff_y, tl + (0.1 + 0.8/workingHoursPerDay*l + 0.4/workingHoursPerDay)*diff_x + 1.*diff_y), startWorkDay.addSecs(l*3600).toString("HH:mm"), QTextOption(Qt::AlignHCenter));
    }

    painter->drawLine(xAxis);

    QPen pen(Qt::gray);
    pen.setStyle(Qt::DashLine);
    pen.setWidthF(0.5);

    painter->save();
    painter->setPen(pen);
    painter->drawLines(verticalGridLines);
    painter->restore();

    painter->save();
    painter->setBrush(QBrush(QColor(70, 70, 70, 50), Qt::SolidPattern));
    painter->drawRect(QRectF(minX, minY, maxX-minX, maxY-minY));
    painter->restore();
}
