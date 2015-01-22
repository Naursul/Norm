#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlError>

#include <QMessageBox>
#include <QTimer>

#include <QColor>

#include <QList>
#include <QPoint>

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

    void WorkTableCellChanged(int row, int column);

    void on_SearchButton_clicked();

    void on_SearchNext_clicked();

    void on_SearchPrev_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;

    int tIn; // интервал таймера
    int rowsOp, rowsPro;  //количество колонок и строк в списках
    int curSPoz; //текущая позиция в списке поиска

    QString hName; // имя хоста
    QString bName; // название базы данных
    QString uName, uPass; //логин и пользователь
    QString IDName, Role; //ID пользователя и его статус
    QString usFields, prodFields, workFields;  //перечни используемых полей из БД
    QString tmpStr; //переменная для разовых операций со строками

    QStringList idOpList, idOpNList, idOpNBList, idOpRList; //ID оператора, Имя оператора, Имя оператора в БД, статус оператора
    QStringList idProList, idProNList; //списки ID и наименований
    QStringList fields; //список полей в рабочей таблице

    QList<QPoint> sList; //Список координат в таблице. соответствующих запросу поиска

    QColor worCol, okCol;  //цвет позиции в работе, готовой позиции

    bool timerFlag; //флаг включенного таймера

//    void optLoad(void);  //загрузка настроек
    void askPass(); //процедура запроса пароля
    void RoleCheck(QString queryText); //проверка статуса пользователя и его ID
    void initWorkTable(); //Инициализация (очистка) всех листов, переменных для работы в рабочей таблице
    void loadData(); //загрузка данных
    void LoadOperators(QString queryText); //загрузка списка операторов
    void LoadProdGroup(QString queryText); //загрузка списка групп продукции
    void LoadWorkTable(QString queryText); //загрузка данных для обработки
    void color(int row); //установка цвета ячеек и запрет редактирования
    void updateDataBase(int row, int col); //обновление базы данных значением из ячейки

    LogPass *Passcard;  //окно запроса логина пароля

};

#endif // MAINWINDOW_H
