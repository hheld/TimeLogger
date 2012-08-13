#include "ProjectGraphicsItem.h"
#include "DayGraphicsScene.h"

#include <QPainter>
#include <QDebug>

ProjectGraphicsItem::ProjectGraphicsItem(ProjectGraphicsItem *parent) :
    QGraphicsItem(parent)
{
//    setFlag(QGraphicsItem::ItemIsMovable);
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
    QRectF br = boundingRect();

    painter->setPen(Qt::red);
    painter->fillRect(br, QBrush(Qt::red, Qt::BDiagPattern));
    painter->drawRect(br);
}

void ProjectGraphicsItem::SetProjectInfo(const QString &name, const QDateTime &start, const QDateTime &end, const int &numOfProjectsThisDay, const int &indexOfThisProject)
{
    projectName = name;
    this->start = start;
    this->end = end;
    this->numOfProjectsThisDay = numOfProjectsThisDay;
    this->indexOfThisProject = indexOfThisProject;
}
