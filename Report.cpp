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

void Report::GetMapProjectName2RowIndex(QMap<QString, int> &projectName2RowIndex, const QList<QList<QPair<QString, double> > > &listOfProjects)
{
    QList<QList<QPair<QString, double> > >::const_iterator outerList_cit = listOfProjects.constBegin();

    while(outerList_cit != listOfProjects.constEnd())
    {
        QList<QPair<QString, double> >::const_iterator innerList_cit = outerList_cit->constBegin();

        while(innerList_cit != outerList_cit->constEnd())
        {
            if(!projectName2RowIndex.contains(innerList_cit->first))
            {
                projectName2RowIndex[innerList_cit->first] = projectName2RowIndex.size()-1;
            }

            ++innerList_cit;
        }

        ++outerList_cit;
    }
}

double Report::RoundHours(const double &hours)
{
    int trunc = 10.*hours;

    int modFive = trunc%5;

    if(modFive<3)
    {
        trunc -= modFive;
    }
    else
    {
        trunc += 5 - modFive;
    }

    return static_cast<double>(trunc / 10.);
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
        html += "<td>" + QString::number(RoundHours(cit.value())) + "</td>";

        html += "</tr>";

        ++cit;
    }

    html += "</table>";
    html += "</p>";

    QMap<QDate, QList<QPair<QString, double> > > workedHoursPerDay = db->GetProjectsDailyWorkedHoursInRange(ui->dateEdit_from->date(), ui->dateEdit_to->date());

    html += tr("<h2>Daily hours worked on projects between %1 and %2:</h2>")
            .arg(ui->dateEdit_from->date().toString(Qt::ISODate))
            .arg(ui->dateEdit_to->date().toString(Qt::ISODate));

    QList<QDate> listOfDays = workedHoursPerDay.keys();

    html += "<p>";
    html += "<table border='1' width='100%'cellpadding='5'>";
    html += "<tr>";
    html += "<th>Project</th>";

    foreach(const QDate &d, listOfDays)
    {
        html += "<th>" + d.toString(Qt::ISODate) + "</th>";
    }

    html += "</tr>";

    QList<QList<QPair<QString, double> > > listOfProjects = workedHoursPerDay.values();

    QMap<QString, int> projectName2RowIndex;

    GetMapProjectName2RowIndex(projectName2RowIndex, listOfProjects);

    int numOfRows = projectName2RowIndex.size();

    for(int row=0; row<numOfRows; ++row)
    {
        html += "<tr>";

        QString rowName;

        QMap<QString, int>::const_iterator rowInd_cit = projectName2RowIndex.constBegin();

        while(rowInd_cit != projectName2RowIndex.constEnd())
        {
            if(rowInd_cit.value() == row)
            {
                rowName = rowInd_cit.key();
                break;
            }

            ++rowInd_cit;
        }

        html += "<td>" + rowName + "</td>";

        foreach(const QDate &day, listOfDays)
        {
            QList<QPair<QString, double> > &projectsInColumn = workedHoursPerDay[day];

            QList<QPair<QString, double> >::const_iterator cit = projectsInColumn.constBegin();

            bool projectFound = false;

            while(cit != projectsInColumn.constEnd())
            {
                const QString &projectName = cit->first;

                if(projectName2RowIndex[projectName] == row)
                {
                    double hours = cit->second;

                    html += "<td>" + QString::number(RoundHours(hours)) + "</td>";

                    projectFound = true;
                    break;
                }

                ++cit;
            }

            if(!projectFound)
            {
                html += "<td>0</td>";
            }
        }

        html += "</tr>";
    }

    html += "</table>";
    html += "</p>";

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
