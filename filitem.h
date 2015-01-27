#ifndef FILITEM_H
#define FILITEM_H

#include <QFrame>

namespace Ui {
class FilItem;
}

class FilItem : public QFrame
{
    Q_OBJECT
    
public:
    explicit FilItem(QWidget *parent = 0, QStringList *filLst = NULL);
    ~FilItem();
    bool filItemOn();
    int filColumnIndex();
    int filOptionIndex();
    QString filOptionText();
    QString filText();
    
private slots:
    void on_CloseItem_clicked();

    void on_checkBox_toggled(bool checked);

private:
    Ui::FilItem *ui;
    QStringList optList, optQList;
};

#endif // FILITEM_H
