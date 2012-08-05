#include "Report.h"
#include "ui_Report.h"
#include "ProjectDatabase.h"

#include <QDebug>

Report::Report(ProjectDatabase *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Report),
    db(db)
{
    ui->setupUi(this);

    QDate currentDate = QDate::currentDate();

    ui->dateEdit_from->setDate(QDate(currentDate.year(), currentDate.month(), 1));
    ui->dateEdit_to->setDate(QDate(currentDate.year(), currentDate.month(), currentDate.daysInMonth()));
}

Report::~Report()
{
    delete ui;
}

void Report::on_pushButton_generateReport_clicked()
{
    clearHtml();
    AddHtmlHeader();

    html += tr("<h2>Total hours worked on projects between %1 and %2:</h2>")
            .arg(ui->dateEdit_from->date().toString(Qt::ISODate))
            .arg(ui->dateEdit_to->date().toString(Qt::ISODate));

    html += "<p>";
    html += "<table border='1' width='100%'cellpadding='5'>";
    html += "<tr>";
    html += "<th>Project</th>";
    html += "<th>Hours worked</th>";
    html += "</tr>";

    QMap<QString, double> workedHoursTotal = db->GetProjectsTotalWorkedHoursInRange(ui->dateEdit_from->date(), ui->dateEdit_to->date());

    QMap<QString, double>::const_iterator cit = workedHoursTotal.constBegin();

    while(cit != workedHoursTotal.constEnd())
    {
        html += "<tr>";

        html += "<td>" + cit.key() + "</td>";
        html += "<td>" + QString::number(cit.value()) + "</td>";

        html += "</tr>";

        ++cit;
    }

    html += "</table>";
    html += "</p>";

    QMap<QDate, QList<QPair<QString, double> > > workedHoursPerDay = db->GetProjectsDailyWorkedHoursInRange(ui->dateEdit_from->date(), ui->dateEdit_to->date());

    qDebug() << workedHoursPerDay;

    AddHtmlFooter();

    ui->textBrowser_report->setHtml(html);
}

void Report::clearHtml()
{
    html.clear();
    ui->textBrowser_report->clear();
}

void Report::AddHtmlHeader()
{
    html = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01//EN\" \"http://www.w3.org/TR/html4/strict.dtd\">"
            "<html>"
            "<body>";
}

void Report::AddHtmlFooter()
{
    html += "</body>"
            "</html>";
}
