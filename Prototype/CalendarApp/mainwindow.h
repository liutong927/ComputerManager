#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QCalendarWidget>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSqlTableModel>
#include <QPushButton>
#include <additemdialog.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void OnDateActivated();
    void OnCalendarPageChanged();
    void OnAddItemClicked();
    void OnAddItemCommit();

private:
    bool PopulateDate(QDate& date);
    bool AddRecordToDB(const QDate& date, const QString& itemString);

    void MarkDateArranged(QDate& date);
    QVector<QDate> GetArrangedDatesOfMonth(QDate& date);

private:
    QWidget* centralWidget;
    QCalendarWidget* calendarWidget;
    QTableView* dailyArrangement;
    QHBoxLayout* wholeLayout;
    QHBoxLayout* buttonLayout;
    QVBoxLayout* rightLayout;
    QPushButton* addBtn;
    QPushButton* deleteBtn;
    QPushButton* searchBtn;
    AddItemDialog* addDlg;

    QSqlTableModel* model;
};

#endif // MAINWINDOW_H
