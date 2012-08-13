#include "DayGraphicsScene.h"

#include <QPainter>
#include <QTime>
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
}
