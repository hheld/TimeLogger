#include "ProjectGraphicsItem.h"
#include "DayGraphicsScene.h"
#include "ProjectDatabase.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ProjectGraphicsItem::ProjectGraphicsItem(ProjectDatabase *db, ProjectGraphicsItem *parent) :
    QGraphicsItem(parent),
    isLeftEdge(false),
    doMove(false),
    doStretch(false),
    hasChanged(false),
    originalXPos(0.),
    db(db)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptsHoverEvents(true);
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

    originalProjectName = name;
    originalStart = start;
    originalEnd = end;
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

        setCursor(QCursor(Qt::ClosedHandCursor));
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
            hasChanged = true;

            projectViewScene->ItemModified();
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
            hasChanged = true;

            projectViewScene->ItemModified();
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
        hasChanged = true;

        originalXPos += diff.x();

        projectViewScene->ItemModified();
    }
}

void ProjectGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    doMove = false;
    doStretch = false;
}

void ProjectGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QRectF br = boundingRect();
    QPointF pos = event->pos();

    if(qAbs(pos.x() - br.right()) < 5. || qAbs(pos.x() - br.left()) < 5.)
    {
        setCursor(QCursor(Qt::SizeHorCursor));
    }
    else
    {
        setCursor(QCursor(Qt::OpenHandCursor));
    }

    QGraphicsItem::hoverMoveEvent(event);
}

void ProjectGraphicsItem::UpdateToolTip()
{
    QString toolTip = QObject::tr("Project name: %1\nStart: %2\nEnd: %3")
            .arg(projectName)
            .arg(start.toString(Qt::SystemLocaleShortDate))
            .arg(end.toString(Qt::SystemLocaleShortDate));

    setToolTip(toolTip);
}

void ProjectGraphicsItem::UpdateDatabaseEntry()
{
    if(hasChanged)
    {
        QString sql = "UPDATE Projects SET Start = '" + start.toString(Qt::ISODate) + "',";
        sql += "End = '" + end.toString(Qt::ISODate) + "' ";
        sql += "WHERE Start='" + originalStart.toString(Qt::ISODate) + "' AND Name='" + originalProjectName + "' AND End='" + originalEnd.toString(Qt::ISODate) + "'";

        QSqlQuery query(*db->Db());

        bool query_ok = query.exec(sql);

        if(!query_ok)
        {
            qDebug() << query.lastError().text();
        }

        hasChanged = false;

        originalProjectName = projectName;
        originalStart = start;
        originalEnd = end;
    }
}

void ProjectGraphicsItem::RemoveFromDb() const
{
    QString sql = "DELETE FROM Projects ";
    sql += "WHERE Start='" + originalStart.toString(Qt::ISODate) + "' AND Name='" + originalProjectName + "' AND End='" + originalEnd.toString(Qt::ISODate) + "'";

    QSqlQuery query(*db->Db());

    bool query_ok = query.exec(sql);

    if(!query_ok)
    {
        qDebug() << query.lastError().text();
    }
}

QDate ProjectGraphicsItem::Date() const
{
    return start.date();
}
