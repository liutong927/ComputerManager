#include "mainwindow.h"
#include <QSqlRecord>
#include <QDebug>

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
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, tr("date"));
    model->setHeaderData(1, Qt::Horizontal, tr("index"));
    model->setHeaderData(2, Qt::Horizontal, tr("item"));

    dailyArrangement->setModel(model);

    // signal and slot
    // click date in calendarWidget to fetch tableData from this date.
    // we only have one tableView, but its data varies by date.
    connect(calendarWidget,SIGNAL(clicked(QDate)),this,SLOT(OnClickDate()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnClickDate()
{
    auto selectedDate = calendarWidget->selectedDate();
    PopulateDate(selectedDate);

    // if empty, insert one empty record to display in view for edit.
    AddRecordToDB(QString(""));
}

void MainWindow::PopulateDate(QDate& date)
{
    // Note: now use varchar to store Date for compare with QDate string.
    // sql date is not easily to compare.
    model->setFilter(QObject::tr("Date = '%1'").arg(date.toString(Qt::ISODate)));
    model->select();
}

void MainWindow::AddRecordToDB(const QString& itemString)
{
    auto selectedDate = calendarWidget->selectedDate();

    int currentRow = model->rowCount();
    model->insertRows(currentRow, 1);
    model->setData(model->index(currentRow,ColumnDate), selectedDate);
    model->setData(model->index(currentRow,ColumnIndx), currentRow);
    model->setData(model->index(currentRow,ColumnItem), itemString);
}
