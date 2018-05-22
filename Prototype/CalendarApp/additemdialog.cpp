#include "additemdialog.h"

AddItemDialog::AddItemDialog(QWidget *parent) :
    QDialog(parent)
{
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("AddItemDialog"));
    this->resize(400, 300);

    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("AddDialogCentralWidget"));
    centralWidget->setGeometry(QRect(30, 20, 320, 253));

    // date region
    dateLabel = new QLabel(centralWidget);
    dateLabel->setObjectName(QStringLiteral("label"));
    dateLabel->setText(tr("Date"));

    dateEdit = new QDateEdit(centralWidget);
    dateEdit->setObjectName(QStringLiteral("dateEdit"));

    // item region
    itemLabel = new QLabel(centralWidget);
    itemLabel->setObjectName(QStringLiteral("label_2"));
    itemLabel->setText(tr("Date"));

    plainTextEdit = new QPlainTextEdit(centralWidget);
    plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));

    // button okay and cancel
    buttonBox = new QDialogButtonBox(centralWidget);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    //layout
    dateHLayout = new QHBoxLayout();
    dateHLayout->setObjectName(QStringLiteral("horizontalLayout"));
    dateHLayout->addWidget(dateLabel);
    dateHLayout->addWidget(dateEdit);

    itemHLayout = new QHBoxLayout();
    itemHLayout->setObjectName(QStringLiteral("horizontalLayout_2"));
    itemHLayout->addWidget(itemLabel);
    itemHLayout->addWidget(plainTextEdit);

    verticalLayout = new QVBoxLayout(centralWidget);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->addLayout(dateHLayout);
    verticalLayout->addLayout(itemHLayout);
    verticalLayout->addWidget(buttonBox);

    // signal and slot
    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));


}

AddItemDialog::~AddItemDialog()
{
}

void AddItemDialog::SetDate(QDate &date)
{
    dateEdit->setDate(date);
}
