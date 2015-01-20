#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlError>

#include <QMessageBox>
#include <QTimer>

#include "logpass.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void enableMain(void);

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;

    int tIn; // интервал таймера

    QString hName; // имя хоста
    QString bName; // название базы данных
    QString uName, uPass; //логин и пользователь

    bool timerFlag; //флаг включенного таймера

//    void optLoad(void);  //загрузка настроек
    void askPass(); //процедура запроса пароля

    LogPass *Passcard;  //окно запроса логина пароля

};

#endif // MAINWINDOW_H
