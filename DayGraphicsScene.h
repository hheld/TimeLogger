#ifndef DAYGRAPHICSSCENE_H
#define DAYGRAPHICSSCENE_H

#include <QGraphicsScene>

class DayGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit DayGraphicsScene(QObject *parent = 0);

signals:

public slots:

private:
    void drawBackground(QPainter *painter, const QRectF &rect);
};

#endif // DAYGRAPHICSSCENE_H
