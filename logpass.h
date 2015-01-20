#ifndef LOGPASS_H
#define LOGPASS_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class LogPass;
}

class LogPass : public QDialog
{
    Q_OBJECT
    
public:
    explicit LogPass(QWidget *parent = 0, QString *usverName = 0,  QString *usverPass = 0);
    ~LogPass();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::LogPass *ui;
    QString *usverName, *usverPass;
};

#endif // LOGPASS_H
