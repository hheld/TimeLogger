#include "DayView.h"
#include "ui_DayView.h"

#include "DayGraphicsScene.h"

DayView::DayView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DayView),
    dayScene(0)
{
    ui->setupUi(this);

    ui->dateEdit_selectDay->setDate(QDate::currentDate());

    dayScene = new DayGraphicsScene(this);

    ui->graphicsView->setScene(dayScene);
}

DayView::~DayView()
{
    delete ui;
    delete dayScene;
}
