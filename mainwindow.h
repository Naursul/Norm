#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlError>

#include <QMessageBox>
#include <QTimer>

#include <QTableWidget>
#include <QTableWidgetItem>

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

    void on_ProdTypes_currentRowChanged(int currentRow);

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;

    int tIn; // интервал таймера
    int rowsOp, rowsPro;  //количество колонок и строк в списках

    QStringList idOpList, idOpNList, idOpNBList, idOpRList; //ID оператора, Имя оператора, Имя оператора в БД, статус оператора
    QStringList idProList, idProNList; //списки ID и наименований

    QString hName; // имя хоста
    QString bName; // название базы данных
    QString uName, uPass; //логин и пользователь
    QString IDName, Role; //ID пользователя и его статус
    QString usFields, prodFields, workFields;  //перечни используемых полей из БД

    bool timerFlag; //флаг включенного таймера

//    void optLoad(void);  //загрузка настроек
    void askPass(); //процедура запроса пароля
    void RoleCheck(QString queryText); //проверка статуса пользователя и его ID
    void loadData(); //загрузка данных
    void LoadOperators(QString queryText); //загрузка списка операторов
    void LoadProdGroup(QString queryText); //загрузка списка групп продукции
    void LoadWorkTable(QString queryText); //загрузка данных для обработки

    LogPass *Passcard;  //окно запроса логина пароля

};

#endif // MAINWINDOW_H
