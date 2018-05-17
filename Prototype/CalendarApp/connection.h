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
    query.exec("create table dailyArrangement (Date date primary key, Id int, Item varchar(255))");
    query.exec("insert into dailyArrangement values(20180517, 0, 'test1')");
    query.exec("insert into dailyArrangement values(20180518, 1, 'test2')");
    query.exec("insert into dailyArrangement values(20180519, 2, 'test3')");
    query.exec("insert into dailyArrangement values(20180520, 3, 'test4')");

    return true;
}
//! [0]

#endif
