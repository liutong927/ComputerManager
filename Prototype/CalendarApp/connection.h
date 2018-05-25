#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSQLDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QApplication>

/*
    This file defines a helper function to open a connection to an
    SQLITE database and to create a test table.
*/

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(":memory:");//in-memory db
    db.setDatabaseName("arrangment.db");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
            qApp->tr("Unable to establish a database connection.\n"
                     "This example needs SQLite support. Please read "
                     "the Qt SQL driver documentation for information how "
                     "to build it.\n\n"
                     "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query;

    // create table
    // note index seems a keyword of sql, should not write like this:
    // query.exec("create table dailyArrangement (date int primary key, index int, item varchar(20))");
    // use id as primary key, date should not be primary key due to it is not unique
    query.exec("create table dailyArrangement (Id integer primary key autoincrement, Date varchar(15), Item varchar(255))");
//    query.exec("insert into dailyArrangement(Date,Item) values('2018-05-01', 'test1')");
//    query.exec("insert into dailyArrangement(Date,Item) values('2018-05-17', 'test2')");
//    query.exec("insert into dailyArrangement(Date,Item) values('2018-05-18', 'test3')");
//    query.exec("insert into dailyArrangement(Date,Item) values('2018-05-19', 'test4')");
//    query.exec("insert into dailyArrangement(Date,Item) values('2018-05-20', 'test5')");
//    query.exec("insert into dailyArrangement(Date,Item) values('2018-05-31', 'test6')");

    // as test data, explicitly insert primary key value, otherwise every connection to db will
    // insert this data again along with default increased id.
    query.exec("insert into dailyArrangement values(1, '2018-05-01', 'test1')");
    query.exec("insert into dailyArrangement values(2, '2018-05-17', 'test2')");
    query.exec("insert into dailyArrangement values(3, '2018-05-18', 'test3')");
    query.exec("insert into dailyArrangement values(4, '2018-05-19', 'test4')");
    query.exec("insert into dailyArrangement values(5, '2018-05-20', 'test5')");
    query.exec("insert into dailyArrangement values(6, '2018-05-31', 'test6')");

    return true;
}
//! [0]

#endif
