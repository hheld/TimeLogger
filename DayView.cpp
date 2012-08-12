#include "DayView.h"
#include "ui_DayView.h"

DayView::DayView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DayView)
{
    ui->setupUi(this);

    ui->dateEdit_selectDay->setDate(QDate::currentDate());
}

DayView::~DayView()
{
    delete ui;
}
