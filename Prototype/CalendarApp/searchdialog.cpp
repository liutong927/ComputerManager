#include "searchdialog.h"

SearchDialog::SearchDialog(QWidget *parent) : QDialog(parent)
{
    if (this->objectName().isEmpty())
         this->setObjectName(QStringLiteral("SearchDialog"));
     this->resize(351, 327);
     this->setWindowTitle(tr("Search"));

     layoutWidget = new QWidget(this);
     layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
     layoutWidget->setGeometry(QRect(20, 20, 310, 301));

     // search by date ui block
     dateRadioButton = new QRadioButton(layoutWidget);
     dateRadioButton->setObjectName(QStringLiteral("dateRadioButton"));
     dateRadioButton->setText(tr("Search by Date"));
     dateRadioButton->setChecked(true);

     fromDateLabel = new QLabel(layoutWidget);
     fromDateLabel->setObjectName(QStringLiteral("fromDateLabel"));
     fromDateLabel->setText(tr("From"));

     fromDateEdit = new QDateEdit(layoutWidget);
     fromDateEdit->setObjectName(QStringLiteral("dateFromEdit"));

     toDateLabel = new QLabel(layoutWidget);
     toDateLabel->setObjectName(QStringLiteral("toDateLabel"));
     toDateLabel->setText(tr("To"));

     toDateEdit = new QDateEdit(layoutWidget);
     toDateEdit->setObjectName(QStringLiteral("dateToEdit"));

     dateHorizontalLayout = new QHBoxLayout();
     dateHorizontalLayout->setObjectName(QStringLiteral("dateHorizontalLayout"));
     dateHorizontalLayout->addWidget(fromDateLabel);
     dateHorizontalLayout->addWidget(fromDateEdit);
     dateHorizontalLayout->addWidget(toDateLabel);
     dateHorizontalLayout->addWidget(toDateEdit);

     dateVerticalLayout = new QVBoxLayout();
     dateVerticalLayout->setObjectName(QStringLiteral("dateVerticalLayout"));
     dateVerticalLayout->addWidget(dateRadioButton);
     dateVerticalLayout->addLayout(dateHorizontalLayout);

     // search by keyword ui block
     keywordRadioButton = new QRadioButton(layoutWidget);
     keywordRadioButton->setObjectName(QStringLiteral("keywordRadioButton"));
     keywordRadioButton->setText(tr("Search by Keyword"));
     keywordRadioButton->setChecked(false);

     keywordLabel = new QLabel(layoutWidget);
     keywordLabel->setObjectName(QStringLiteral("keywordLabel"));
     keywordLabel->setText(tr("Keyword"));

     keywordTextEdit = new QPlainTextEdit(layoutWidget);
     keywordTextEdit->setObjectName(QStringLiteral("keywordTextEdit"));
     keywordTextEdit->setEnabled(false);

     keywordHorizontalLayout = new QHBoxLayout();
     keywordHorizontalLayout->setObjectName(QStringLiteral("keywordHorizontalLayout"));
     keywordHorizontalLayout->addWidget(keywordLabel);
     keywordHorizontalLayout->addWidget(keywordTextEdit);

     keywordVerticalLayout = new QVBoxLayout();
     keywordVerticalLayout->setObjectName(QStringLiteral("keywordVerticalLayout"));
     keywordVerticalLayout->addWidget(keywordRadioButton);
     keywordVerticalLayout->addLayout(keywordHorizontalLayout);

     // button ok and cancel.
     buttonBox = new QDialogButtonBox(layoutWidget);
     buttonBox->setObjectName(QStringLiteral("buttonBox"));
     buttonBox->setOrientation(Qt::Horizontal);
     buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

     // whole layout
     wholeVerticalLayout = new QVBoxLayout(layoutWidget);
     wholeVerticalLayout->setObjectName(QStringLiteral("wholeVerticalLayout"));
     wholeVerticalLayout->setContentsMargins(0, 0, 0, 0);
     wholeVerticalLayout->addLayout(dateVerticalLayout);
     wholeVerticalLayout->addLayout(keywordVerticalLayout);
     wholeVerticalLayout->addWidget(buttonBox);

     // signal and slot.
     connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
     connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
     connect(dateRadioButton, SIGNAL(toggled(bool)), this, SLOT(OnRadioButtonToggled()));
     connect(keywordRadioButton, SIGNAL(toggled(bool)), this, SLOT(OnRadioButtonToggled()));

 }

SearchDialog::~SearchDialog()
{
}

void SearchDialog::SetDateRange(QDate &fromDate, QDate &toDate)
{
    fromDateEdit->setDate(fromDate);
    toDateEdit->setDate(toDate);
}

void SearchDialog::OnRadioButtonToggled()
{
    bool isDateRadioChecked = dateRadioButton->isChecked();
    bool isKeywordRadioChecked = keywordRadioButton->isChecked();
    fromDateEdit->setEnabled(isDateRadioChecked);
    toDateEdit->setEnabled(isDateRadioChecked);
    keywordTextEdit->setEnabled(isKeywordRadioChecked);
}
