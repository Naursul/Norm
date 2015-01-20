#include "logpass.h"
#include "ui_logpass.h"

LogPass::LogPass(QWidget *parent, QString *userName, QString *userPass) :
    QDialog(parent),
    ui(new Ui::LogPass)
{
    ui->setupUi(this);
    usverName = userName;
    usverPass = userPass;
}

LogPass::~LogPass()
{
    delete ui;
}

void LogPass::on_buttonBox_clicked(QAbstractButton *button)
{
    switch (ui->buttonBox->buttonRole(button)) {
        case QDialogButtonBox::AcceptRole:
            *usverName = ui->userName->text().trimmed();
            *usverPass = ui->userPass->text().trimmed();
            break;
        case QDialogButtonBox::RejectRole:
            break;
        default:
            break;
    }
}
