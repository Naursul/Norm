#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QMYSQL");


/* =================================
    Задание переменных по умолчанию
   ===============================*/

    timerFlag = true; //таймер по умолчанию включен
    tIn = 30000; // задание интервала обновления по умолчанию
    hName = "localhost"; //имя хоста по умолчанию
    bName = "PolusGold"; //имя БД по умолчанию
    uName = "Naursul"; //Логин по умолчанию                        УДАЛИТЬ!!!
    uPass = "ynjwiosx145449821"; //Пароль по умолчанию             УДАЛИТЬ!!!

/* =======================
    чтение файла настроек
   =====================*/

//    optLoad();


/* =======================================
    далее идут параметры подключения к БД
   =====================================*/

    db.setHostName(hName); //установка имени хоста
    db.setDatabaseName(bName); //установка БД, к которой будет подключение
//    askPass(); //запрос логина-пароля
    db.setUserName(uName); //установка имени пользователя для подключения
    db.setPassword(uPass); //установка пароля для входа

    if (db.open()) {
        db.close();
        ui->plainTextEdit->setPlainText("подключение произошло... типа");
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

/* =================================
    процедура чтения файла настроек
   ===============================*/
/*
void MainWindow::optLoad(void)
{
    QString fileName = "options.ini";
    bool hna = true;
    bool ti = true;
    if (fileName != NULL) {
        QFile inFile;
        inFile.setFileName(fileName);
        if(inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&inFile);
            while (!in.atEnd()) {
                QString line = in.readLine(1024);
                QStringList fields = line.split(" = ");
                if (fields.at(0) == "[Host Name]") {  //чтение имени хоста
                    hName = fields.at(1).trimmed();
                    hna = false;
                }
                if (fields.at(0) == "[Timer ON]") {    //чтение статуса таймера
                    timerFlag = fields.at(1).trimmed().toInt();
                }
                if (fields.at(0) == "[Timer Interval]") {  //чтение времени таймера
                    tIn = fields.at(1).trimmed().toInt();
                    ti = false;
                }
            }
        } else {
            QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, tr("Ошибка"), tr("Не могу открыть файл настроек!\n\nБудут использоваться значения по умолчанию.") + fileName, QMessageBox::Close, this);
            msgBox->exec();
            hna = false;
            ti = false;
        }
    }
    if (hna) {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, tr("Ошибка"), tr("Ошибка в файле настроек!\n\nБудет использоваться имя хоста по умолчанию.") + fileName, QMessageBox::Close, this);
        msgBox->exec();
    }
    if (ti) {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, tr("Ошибка"), tr("Ошибка в файле настроек!\n\nБудет использоваться интервал обновления по умолчанию.") + fileName, QMessageBox::Close, this);
        msgBox->exec();
    }
}*/

/* ==========================
    процедура запроса пароля
   ========================*/
/*
void MainWindow::askPass()
{
    //QTextCodec *ttt = QTextCodec::codecForName("Windows-1251");
    Passcard = new NamePass(this, &usverName, &usverPass);
        switch (Passcard->exec()) {
            case QDialog::Accepted:

            //ui->textEdit->append(usverName);
            //ui->textEdit->append(usverName);
                db.setUserName(usverName); //установка имени пользователя для подключения (попробовать сделать отдельным окном!)
                db.setPassword(usverPass); //установка пароля для входа
                if (db.open()) {
                    db.close();
                    QTimer::singleShot(0, this, SLOT(enableMain()));
                    loadBase();
                    timerON();
                } else {
                    QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("\nНеверно указан логин/пароль!"), QMessageBox::Close, this);
                    msgBox->exec();
                    QTimer::singleShot(0, this, SLOT(enableMain()));
                    ui->OrglistWidget->clear();
                    ui->DoglistWidget->clear();
                    ui->treeWidget->clear();
                    timer->stop();
                }
                break;
            case QDialog::Rejected:
                QTimer::singleShot(0, this, SLOT(close()));
                break;
            default:
                QTimer::singleShot(0, this, SLOT(close()));
                break;
         }
}*/
