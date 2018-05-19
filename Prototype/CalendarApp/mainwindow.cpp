#include "mainwindow.h"
#include <QSqlRecord>
#include <QDebug>
#include <QTextCharFormat>

enum ColumnIndex
{
    ColumnDate,
    ColumnIndx,
    ColumnItem
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setObjectName(QStringLiteral("ToolSet"));
    this->setWindowTitle(tr("ToolSet"));
    this->resize(800, 600);

    // create central widget
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    this->setCentralWidget(centralWidget);

    // create calendar widget
    calendarWidget = new QCalendarWidget(centralWidget);
    calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
    calendarWidget->setGeometry(10,10,450,450);

    // create dailyArrangement table view
    dailyArrangement = new QTableView;
    dailyArrangement->setObjectName(QStringLiteral("dailyArrangement"));
    dailyArrangement->setGeometry(490,10,500,450);
    dailyArrangement->setColumnWidth(0,450);

    // layout
    hLayout = new QHBoxLayout(centralWidget);
    hLayout->addWidget(calendarWidget);
    hLayout->addSpacing(20);
    hLayout->addWidget(dailyArrangement);

    // sql model
    model = new QSqlTableModel(dailyArrangement);
    model->setTable("dailyArrangement");
    // apply OnRowChange or OnFieldChange will submit to db directly if modify row/field.
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->setHeaderData(0, Qt::Horizontal, tr("date"));
    model->setHeaderData(1, Qt::Horizontal, tr("index"));
    model->setHeaderData(2, Qt::Horizontal, tr("item"));

    dailyArrangement->setModel(model);
    OnDateActivated();// show current date data, no signal.

    // signal and slot

    // click date in calendarWidget to fetch tableData from this date.
    // we only have one tableView, but its data varies by date.
    // use selectionChanged signal since clicked only emit when date clicked not include
    // other ways to select date, e.g. key pressed or programtically.
    connect(calendarWidget,SIGNAL(selectionChanged()),this,SLOT(OnDateActivated()));
    // calendar month page changed, highlight date if has non-empty records.
    connect(calendarWidget,SIGNAL(currentPageChanged(int,int)),this,SLOT(OnCalendarPageChanged()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnDateActivated()
{
    auto selectedDate = calendarWidget->selectedDate();
    PopulateDate(selectedDate);

    // if empty, insert one empty record to display in view for edit.
    //AddRecordToDB(QString("xxx"));
}

void MainWindow::OnCalendarPageChanged()
{
    // if month page changed, should update selected date.
    auto oldSelectedDate = calendarWidget->selectedDate();
    int day = oldSelectedDate.day();
    QDate currentSelectedDate(calendarWidget->yearShown(),calendarWidget->monthShown(),day);
    calendarWidget->setSelectedDate(currentSelectedDate);

    // highlight all arranged dates in this month(page).
    QVector<QDate> dateVec(GetArrangedDatesOfMonth(currentSelectedDate));
    for(auto date: dateVec)
    {
        QTextCharFormat format;
        format.setBackground(Qt::red);
        calendarWidget->setDateTextFormat(date,format);
    }
}

bool MainWindow::PopulateDate(QDate& date)
{
    // Note: now use varchar to store Date for compare with QDate string.
    // sql date is not easily to compare.
    model->setFilter(QObject::tr("Date = '%1'").arg(date.toString(Qt::ISODate)));
    model->select();

    // use current model row count to check it is empty or not.
    return (model->rowCount() != 0);
}

void MainWindow::AddRecordToDB(const QString& itemString)
{
    auto selectedDate = calendarWidget->selectedDate();

    int currentRow = model->rowCount();
    model->insertRows(currentRow, 1);
    model->setData(model->index(currentRow,ColumnDate), selectedDate.toString(Qt::ISODate));
    model->setData(model->index(currentRow,ColumnIndx), currentRow);
    model->setData(model->index(currentRow,ColumnItem), itemString);
}

QVector<QDate> MainWindow::GetArrangedDatesOfMonth(QDate date)
{
    int year = date.year();
    int month = date.month();
    int daysOfMonth = date.daysInMonth();
    QDate fromDate(year,month,1);
    QDate toDate(year,month,daysOfMonth);
    model->setFilter(QObject::tr("Date >= '%1' and Date <= '%2'").arg(fromDate.toString(Qt::ISODate)).arg(toDate.toString(Qt::ISODate)));
    int rowCount = model->rowCount();
    QVector<QDate> dateVec;
    for(int row = 0; row < rowCount; ++row)
    {
        QVariant dateFromDB = model->data(model->index(row,0));
        QDate date(QDate::fromString(dateFromDB.toString(),Qt::ISODate)); // contruct date from string.
        dateVec.push_back(date);
    }

    return dateVec;
}
