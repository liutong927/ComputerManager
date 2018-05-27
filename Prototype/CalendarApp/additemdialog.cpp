#include "additemdialog.h"
#include <QPushButton>

AddItemDialog::AddItemDialog(QWidget *parent, bool isEdit) :
    QDialog(parent)
{
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("AddItemDialog"));
    this->resize(400, 300);
    QString dialogTitle = isEdit? tr("Edit Item"): tr("Add Item");
    this->setWindowTitle(dialogTitle);

    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("AddDialogCentralWidget"));
    centralWidget->setGeometry(QRect(30, 20, 320, 253));

    // date region
    dateLabel = new QLabel(centralWidget);
    dateLabel->setObjectName(QStringLiteral("label"));
    dateLabel->setText(tr("Date"));

    dateEdit = new QDateEdit(centralWidget);
    dateEdit->setObjectName(QStringLiteral("dateEdit"));
    if(isEdit)
    {
        dateEdit->setEnabled(false);
    }

    // item region
    itemLabel = new QLabel(centralWidget);
    itemLabel->setObjectName(QStringLiteral("label_2"));
    itemLabel->setText(tr("Item"));

    plainTextEdit = new QPlainTextEdit(centralWidget);
    plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));

    // button okay and cancel
    buttonBox = new QDialogButtonBox(centralWidget);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    if(!isEdit)
    {
        // for add mode, it is empty for plainTextEdit. ok button should be insensitive.
        OnTextChanged();
    }

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
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    if(isEdit)
    {
        QObject::connect(buttonBox, SIGNAL(accepted()), this->parent(), SLOT(OnEditItemCommit()));
    }
    else
    {
        QObject::connect(buttonBox, SIGNAL(accepted()), this->parent(), SLOT(OnAddItemCommit()));
    }
    // plainTextEdit controls ok button sensitivity
    connect(plainTextEdit, SIGNAL(textChanged()), this, SLOT(OnTextChanged()));
}

AddItemDialog::~AddItemDialog()
{
}

void AddItemDialog::SetDate(QDate &date)
{
    dateEdit->setDate(date);
}

QDate AddItemDialog::GetCurrentDateInDialog()
{
    return dateEdit->date();
}

QString AddItemDialog::GetItemText()
{
    return plainTextEdit->toPlainText();
}

void AddItemDialog::SetItemText(QString &text)
{
    plainTextEdit->setPlainText(text);
}

void AddItemDialog::OnTextChanged()
{
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!plainTextEdit->toPlainText().isEmpty());
}
