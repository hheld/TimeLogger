#include "ProjectGraphicsItem.h"
#include "DayGraphicsScene.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

ProjectGraphicsItem::ProjectGraphicsItem(ProjectGraphicsItem *parent) :
    QGraphicsItem(parent),
    isLeftEdge(false),
    doMove(false),
    doStretch(false),
    originalXPos(0.)
{
    setFlag(QGraphicsItem::ItemIsMovable);
}

QRectF ProjectGraphicsItem::boundingRect() const
{
    DayGraphicsScene *projectViewScene = dynamic_cast<DayGraphicsScene*>(scene());

    double startXCoord = projectViewScene->MapTimeToXCoord(start);
    double endXCoord = projectViewScene->MapTimeToXCoord(end);

    double availableHeight = projectViewScene->maxY - projectViewScene->minY;

    double heightOfEachProject = availableHeight / static_cast<double>(numOfProjectsThisDay);

    return QRectF(QPointF(startXCoord, projectViewScene->minY+indexOfThisProject*heightOfEachProject), QPointF(endXCoord, projectViewScene->minY + (indexOfThisProject+1.)*heightOfEachProject));
}

void ProjectGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF br = boundingRect();

    painter->fillRect(br, QBrush(QColor(9, 91, 255, 100), Qt::SolidPattern));

    painter->save();
    painter->setPen(QColor(35, 73, 108));
    painter->drawRect(br);
    painter->restore();

    painter->setPen(Qt::white);
    painter->drawText(br, projectName, QTextOption(Qt::AlignCenter));
}

void ProjectGraphicsItem::SetProjectInfo(const QString &name, const QDateTime &start, const QDateTime &end, const int &numOfProjectsThisDay, const int &indexOfThisProject)
{
    projectName = name;
    this->start = start;
    this->end = end;
    this->numOfProjectsThisDay = numOfProjectsThisDay;
    this->indexOfThisProject = indexOfThisProject;
}

void ProjectGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF br = boundingRect();
    QPointF pos = event->pos();

    if(qAbs(pos.x() - br.right()) < 5.)
    {
        isLeftEdge = false;

        doStretch = true;
        doMove = false;
    }
    else if(qAbs(pos.x() - br.left()) < 5.)
    {
        isLeftEdge = true;

        doStretch = true;
        doMove = false;
    }
    else
    {
        originalXPos = pos.x();

        doStretch = false;
        doMove = true;
    }
}

void ProjectGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    DayGraphicsScene *projectViewScene = dynamic_cast<DayGraphicsScene*>(scene());

    if(doStretch)
    {
        QPointF pos = event->pos();
        double xCoord = pos.x();

        if(isLeftEdge)
        {
            QTime newStartTime = projectViewScene->MapXCoordToTime(xCoord);

            if(newStartTime>=end.time())
            {
                return;
            }

            prepareGeometryChange();
            start.setTime(newStartTime);

            UpdateToolTip();
        }
        else
        {
            QTime newEndTime = projectViewScene->MapXCoordToTime(xCoord);

            if(newEndTime<=start.time())
            {
                return;
            }

            prepareGeometryChange();
            end.setTime(newEndTime);

            UpdateToolTip();
        }
    }
    else if(doMove)
    {
        QPointF newPos = event->pos();
        newPos.setY(0.);

        QPointF oldPos(originalXPos, 0.);
        QPointF diff = newPos - oldPos;

        QRectF br = boundingRect();

        QTime newStartTime = projectViewScene->MapXCoordToTime(br.left() + diff.x());
        QTime newEndTime = projectViewScene->MapXCoordToTime(br.right() + diff.x());

        prepareGeometryChange();
        start.setTime(newStartTime);
        end.setTime(newEndTime);

        UpdateToolTip();
        originalXPos += diff.x();
    }
}

void ProjectGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    doMove = false;
    doStretch = false;
}

void ProjectGraphicsItem::UpdateToolTip()
{
    QString toolTip = QObject::tr("Project name: %1\nStart: %2\nEnd: %3")
            .arg(projectName)
            .arg(start.toString(Qt::SystemLocaleShortDate))
            .arg(end.toString(Qt::SystemLocaleShortDate));

    setToolTip(toolTip);
}
