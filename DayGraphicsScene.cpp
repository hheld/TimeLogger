#include "DayGraphicsScene.h"

#include <QPainter>
#include <QTime>
#include <QDebug>

DayGraphicsScene::DayGraphicsScene(QObject *parent) :
    QGraphicsScene(parent)
{
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

    QVector<QLineF> verticalGridLines;

    QTime startWorkDay(9, 0, 0);

    for(int l=0; l<10; ++l)
    {
        verticalGridLines.append(QLineF(tl + (0.1 + 0.8/9.*l)*diff_x + 0.05*diff_y, tl + (0.1 + 0.8/9.*l)*diff_x + 0.95*diff_y));

        painter->drawText(QRectF(tl + (0.1 + 0.8/9.*l - 0.4/9.)*diff_x + 0.95*diff_y, tl + (0.1 + 0.8/9.*l + 0.4/9.)*diff_x + 1.*diff_y), startWorkDay.addSecs(l*3600).toString("HH:mm"), QTextOption(Qt::AlignHCenter));
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
