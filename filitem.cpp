#include "filitem.h"
#include "ui_filitem.h"

FilItem::FilItem(QWidget *parent, QStringList *filLst):
    QFrame(parent),
    ui(new Ui::FilItem)
{
    ui->setupUi(this);
    optList << tr("РАВНО") << tr("НЕ РАВНО") << tr("СОДЕРЖИТ") << tr("НЕ СОДЕРЖИТ") << tr("НАЧИНАЕТСЯ С") << tr("НЕ НАЧИНАЕТСЯ С");
    optQList << tr(" = '") << tr(" <> '") << tr(" like '%") << tr(" not like '%") << tr(" like '") << tr(" not like '");
    ui->Column->addItems(*filLst);
    ui->Option->addItems(optList);
}

FilItem::~FilItem()
{
    delete ui;
}

void FilItem::on_CloseItem_clicked()
{
    QWidget *par = this->parentWidget();
    QObjectList qList = par->children();
    int cnt = 0;
    for ( int i = 0; i < qList.count(); i++)
    {
        if (qList.at(i)->objectName() == "FilItem") cnt++;
    }
    if(cnt <= 1) return;
    this->~FilItem();
}

void FilItem::on_checkBox_toggled(bool checked)
{
    ui->Column->setEnabled(checked);
    ui->Option->setEnabled(checked);
    ui->lineEdit->setEnabled(checked);
}

bool FilItem::filItemOn()
{
    return ui->checkBox->isChecked();
}

int FilItem::filColumnIndex()
{
    return ui->Column->currentIndex();
}

int FilItem::filOptionIndex()
{
    return ui->Option->currentIndex();
}

QString FilItem::filOptionText()
{
    return optQList.at(ui->Option->currentIndex());
}

QString FilItem::filText()
{
    return ui->lineEdit->text();
}
