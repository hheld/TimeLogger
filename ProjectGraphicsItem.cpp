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
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF br = boundingRect();


//    painter->fillRect(br, QBrush(QColor(9, 91, 255), Qt::BDiagPattern));
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
