#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QCalendarWidget>
#include <QTableWidget>
#include <QHBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QWidget* centralWidget;
    QCalendarWidget* calendarWidget;
    QTableWidget* dailyArrangement;
    QHBoxLayout* hLayout;
};

#endif // MAINWINDOW_H
