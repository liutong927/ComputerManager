#include "mainwindow.h"
#include <QSqlRecord>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setObjectName(QStringLiteral("ToolSet"));
    this->setWindowTitle("ToolSet");
    this->resize(800, 600);

    // create central widget
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    this->setCentralWidget(centralWidget);

    // create calendar widget
    calendarWidget = new QCalendarWidget(centralWidget);
    calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
    calendarWidget->setGeometry(10,10,450,450);

    // create dailyArrangement table widget
    dailyArrangement = new QTableWidget(5, 1, centralWidget);
    dailyArrangement->setObjectName(QStringLiteral("dailyArrangement"));
    dailyArrangement->setGeometry(490,10,500,450);

    QStringList header;
    header.push_back("事项");
    dailyArrangement->setHorizontalHeaderLabels(header);
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
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Date"));
    model->setHeaderData(1, Qt::Horizontal, tr("Index"));
    model->setHeaderData(2, Qt::Horizontal, tr("Item"));


    // signal and slot
    // click date in calendarWidget to fetch tableData from this date.
    // we only have one tableWidget, but its data varies by date.
    connect(calendarWidget,SIGNAL(clicked(QDate)),dailyArrangement,SLOT(MainWindow::PopulateSelectedDate()));

}

MainWindow::~MainWindow()
{

}

void MainWindow::PopulateSelectedDate()
{
    auto selectedDate = calendarWidget->selectedDate();
    if(selectedDate == QDate(2018,5,20))
    {
        // fetch item data from DB.
        QSqlRecord record = model->record(0);
        QVariant value = record.value(2);

        QTableWidgetItem item;
        item.setText(value.toString());
        dailyArrangement->setItem(0,1,&item);
    }


}

void MainWindow::AddArrangmentToDB()
{
    QDate testDate(2018,5,20);
    //model->insertRecord();
}
