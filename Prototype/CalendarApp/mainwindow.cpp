#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setObjectName(QStringLiteral("ToolSet"));
    this->setWindowTitle("ToolSet");
    this->resize(1000, 800);

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

    // layout
    hLayout = new QHBoxLayout(centralWidget);
    hLayout->addWidget(calendarWidget);
    hLayout->addSpacing(20);
    hLayout->addWidget(dailyArrangement);
}

MainWindow::~MainWindow()
{

}
