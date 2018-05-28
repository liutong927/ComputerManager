#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QVBoxLayout>

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = 0);
    ~SearchDialog();

    void SetDateRange(QDate& fromDate, QDate& toDate);

public slots:
    void OnRadioButtonToggled();

private:
    QWidget *layoutWidget;
    QVBoxLayout *wholeVerticalLayout;
    QVBoxLayout *dateVerticalLayout;
    QRadioButton *dateRadioButton;
    QHBoxLayout *dateHorizontalLayout;
    QLabel *fromDateLabel;
    QDateEdit *fromDateEdit;
    QLabel *toDateLabel;
    QDateEdit *toDateEdit;
    QVBoxLayout *keywordVerticalLayout;
    QRadioButton *keywordRadioButton;
    QHBoxLayout *keywordHorizontalLayout;
    QLabel *keywordLabel;
    QPlainTextEdit *keywordTextEdit;
    QDialogButtonBox *buttonBox;
};

#endif // SEARCHDIALOG_H
