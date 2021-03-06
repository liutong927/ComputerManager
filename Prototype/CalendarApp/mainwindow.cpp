#include "mainwindow.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>
#include <QTextCharFormat>
#include <QMessageBox>
#include <QSqlError>
#include <QModelIndex>

enum ColumnIndex
{
    ColumnId,
    ColumnDate,
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
    dailyArrangement->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // create pushbotton of Add/Delete/Search.
    addBtn = new QPushButton(tr("Add"),centralWidget);
    deleteBtn = new QPushButton(tr("Delete"),centralWidget);
    deleteBtn->setEnabled(false);
    searchBtn = new QPushButton(tr("Search"),centralWidget);

    // layout
    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addBtn);
    buttonLayout->addWidget(deleteBtn);
    buttonLayout->addWidget(searchBtn);

    rightLayout = new QVBoxLayout;
    rightLayout->addLayout(buttonLayout);
    rightLayout->addWidget(dailyArrangement);

    wholeLayout = new QHBoxLayout(centralWidget);
    wholeLayout->addWidget(calendarWidget);
    wholeLayout->addSpacing(20);
    wholeLayout->addLayout(rightLayout);

    // sql model
    model = new QSqlTableModel(dailyArrangement);
    model->setTable("dailyArrangement");
    // apply OnRowChange or OnFieldChange will submit to db directly if modify row/field.
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(ColumnDate, Qt::Horizontal, tr("date"));
    model->setHeaderData(ColumnItem, Qt::Horizontal, tr("item"));

    dailyArrangement->setModel(model);
    // hide column only takes effect after setmodel.
    dailyArrangement->hideColumn(ColumnId);
    dailyArrangement->hideColumn(ColumnDate);
    // set item column width to be same as whole table view.
    dailyArrangement->setColumnWidth(ColumnItem,dailyArrangement->width());
    //dailyArrangement->resizeRowsToContents();

    OnDateActivated();// show current date data.
    OnCalendarPageChanged(); // highlight current page arranged dates.

    // signal and slot

    // click date in calendarWidget to fetch tableData from this date.
    // we only have one tableView, but its data varies from date.
    // use selectionChanged signal since clicked only emit when date clicked not include
    // other ways to select date, e.g. key pressed or programtically.
    connect(calendarWidget,SIGNAL(selectionChanged()),this,SLOT(OnDateActivated()));
    // calendar month page changed, highlight date if has non-empty records.
    connect(calendarWidget,SIGNAL(currentPageChanged(int,int)),this,SLOT(OnCalendarPageChanged()));
    // button AddItem
    connect(addBtn,SIGNAL(clicked(bool)),this,SLOT(OnAddItemClicked()));
    // button delete
    // TODO: if no selection for item in dailyArrangement, delete button should be disabled.
    connect(dailyArrangement,SIGNAL(clicked(QModelIndex)),this,SLOT(OnArrangementSelected()));
    connect(deleteBtn,SIGNAL(clicked(bool)),this,SLOT(OnDeleteItemClicked()));
    // edit item
    // OnCurrentRowChanged signal is emit before tableview clicked signal.
    // use double click to popup edit dialog.
    connect(dailyArrangement,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(OnCurrentRowChanged()));
    //connect(dailyArrangement->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
    //this,SLOT(OnCurrentRowChanged()));

    // button search
    connect(searchBtn,SIGNAL(clicked(bool)),this,SLOT(OnSearchClicked()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::OnDateActivated()
{
    auto selectedDate = calendarWidget->selectedDate();
    PopulateDate(selectedDate);
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
        MarkDateArranged(date);
    }
}

void MainWindow::OnAddItemClicked()
{
    addDlg = new AddItemDialog(this);
    addDlg->SetDate(calendarWidget->selectedDate());
    addDlg->show();
}

void MainWindow::OnAddItemCommit()
{
    auto currentDate = addDlg->GetCurrentDateInDialog();
    auto itemString = addDlg->GetItemText();

    if(AddRecordInDB(currentDate, itemString))
    {
        // issue: when focus is still on tableview, calendarwidget does not get updated immediately.
        // so here set focus to calendar firstly.
        calendarWidget->setFocus();
        MarkDateArranged(currentDate);
    }
}

void MainWindow::OnEditItemCommit()
{
    bool isSuccess = false;
    // current index should be selected Item index
    QModelIndex modelIndex = dailyArrangement->currentIndex();
    QString itemString = addDlg->GetItemText();
    model->setData(modelIndex, itemString);

    // only if item is not empty, we submit it.
    if(!itemString.isNull() && !itemString.isEmpty())
    {
        isSuccess = model->submitAll();
        if(!isSuccess)
        {
            auto error = model->lastError();
            QMessageBox::critical(this, tr("SQL Error"), error.text());
            model->revertAll();
        }
    }
}

void MainWindow::OnDeleteItemClicked()
{
    auto button = QMessageBox::question(this,tr("Delete"),tr("Are you sure to delete this item?"));
    if(button == QMessageBox::Yes)
    {
        // only allow single deletion currently.
        QModelIndex modelIndex = dailyArrangement->currentIndex();
        model->removeRow(modelIndex.row());
        model->submitAll();

        // after deletion, reset delete button status.
        if(!dailyArrangement->currentIndex().isValid())
        {
            deleteBtn->setEnabled(false);
        }
    }
}

void MainWindow::OnArrangementSelected()
{
    // if one item in dailyArrangement is selected, then "delete" button is enabled.
    deleteBtn->setEnabled(true);
}

void MainWindow::OnCurrentRowChanged()
{
    // this slot is to pop up "Edit Item" dialog.
    addDlg = new AddItemDialog(this, true);
    addDlg->SetDate(calendarWidget->selectedDate());
    QModelIndex modelIndex = dailyArrangement->currentIndex();
    QVariant currentData = model->data(modelIndex);
    addDlg->SetItemText(currentData.toString());
    addDlg->show();
}

void MainWindow::OnSearchClicked()
{
    searchDlg = new SearchDialog(this);
    QDate currentDate = QDate::currentDate();
    QDate oneMonthAgoDate = currentDate.addDays(-30);
    searchDlg->SetDateRange(oneMonthAgoDate, currentDate);
    searchDlg->show();
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

bool MainWindow::AddRecordInDB(const QDate& date, const QString& itemString)
{
    Q_ASSERT(date.isValid());
    bool isSuccess = false;
    int currentRow = model->rowCount();
    model->insertRows(currentRow, 1);
    model->setData(model->index(currentRow,ColumnDate), date.toString(Qt::ISODate));
    model->setData(model->index(currentRow,ColumnItem), itemString);

    // only if item is not empty, we submit it.
    if(!itemString.isNull() && !itemString.isEmpty())
    {
        isSuccess = model->submitAll();
        if(!isSuccess)
        {
            auto error = model->lastError();
            QMessageBox::critical(this, tr("SQL Error"), error.text());
            model->revertAll();
        }
    }

    return isSuccess;
}

void MainWindow::MarkDateArranged(QDate &date)
{
    QTextCharFormat format;
    format.setBackground(Qt::gray);
    calendarWidget->setDateTextFormat(date,format);
}

QVector<QDate> MainWindow::GetArrangedDatesOfMonth(QDate& date)
{
    int year = date.year();
    int month = date.month();
    int daysOfMonth = date.daysInMonth();
    QDate fromDate(year,month,1);
    QDate toDate(year,month,daysOfMonth);

//    auto oldFilter = model->filter();
//    model->setFilter(QObject::tr("Date >= '%1' and Date <= '%2'")
//        .arg(fromDate.toString(Qt::ISODate)).arg(toDate.toString(Qt::ISODate)));
//    int rowCount = model->rowCount();
//    QVector<QDate> dateVec;
//    for(int row = 0; row < rowCount; ++row)
//    {
//        QVariant dateFromDB = model->data(model->index(row,0));
//        QDate date(QDate::fromString(dateFromDB.toString(),Qt::ISODate)); // contruct date from string.
//        dateVec.push_back(date);
//    }

//    //need to reset to old filter, otherwise new filter results will be shown, here we just want filtered data.
//    model->setFilter(oldFilter);

    // use local QSqlQuery variable to execute select on db, this will avoid using class member model
    // which will change global QSQLTableModel object. Here we just want to access data not change model.
    QVector<QDate> dateVec;
    QString sql = tr("SELECT * FROM dailyArrangement WHERE Date >= '%1' and Date <= '%2'")
            .arg(fromDate.toString(Qt::ISODate)).arg(toDate.toString(Qt::ISODate));
    QSqlQuery query(sql);
    while(query.next())
    {
        QVariant dateFromDB = query.value(ColumnDate);
        QDate date(QDate::fromString(dateFromDB.toString(),Qt::ISODate)); // contruct date from string.
        dateVec.push_back(date);
    }

    return dateVec;
}
