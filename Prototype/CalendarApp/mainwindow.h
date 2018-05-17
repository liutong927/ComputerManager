#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QCalendarWidget>
#include <QTableView>
#include <QHBoxLayout>
#include <QSqlTableModel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void PopulateSelectedDate();

private:
    void AddRecordToDB();

private:
    QWidget* centralWidget;
    QCalendarWidget* calendarWidget;
    QTableView* dailyArrangement;
    QHBoxLayout* hLayout;

    QSqlTableModel* model;
};

#endif // MAINWINDOW_H
