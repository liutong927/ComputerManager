#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateEdit>
#include <QLabel>
#include <QPlainTextEdit>
#include <QDialogButtonBox>

class AddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddItemDialog(QWidget *parent = 0);
    ~AddItemDialog();

    void SetDate(QDate& date);

private:
    QWidget* centralWidget;
    QVBoxLayout* verticalLayout;
    QHBoxLayout* dateHLayout;
    QLabel* dateLabel;
    QDateEdit* dateEdit;
    QHBoxLayout* itemHLayout;
    QLabel* itemLabel;
    QPlainTextEdit* plainTextEdit;
    QDialogButtonBox* buttonBox;
};

#endif // ADDITEMDIALOG_H
