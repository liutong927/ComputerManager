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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void OnDateActivated();
    void OnCalendarPageChanged();

private:
    bool PopulateDate(QDate& date);
    void AddRecordToDB(const QString& itemString);

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

    QSqlTableModel* model;
};

#endif // MAINWINDOW_H
