#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QMYSQL");

/* ==========================================
    задание полей для таблицы вывода в форме
   ========================================*/

    usFields = "Operator.idOperator, Operator.OperatorName, Operator.BaseName, Operator.Role";  //Список полей для таблицы исполнителей (операторов)
    prodFields = "ProdGroup.idProdGroup, ProdGroup.ProdGroupName, ProdGroup.Operator_idOperator"; //список полей таблицы групп продукции
    workFields = "ProdGroup.ProdGroupName, IshodMas.IshNameShort, WorkTable.WorkTablecol, WorkTable.WorkTablecol1, WorkTable.WorkTablecol2"; //список полей для работы


/* =================================
    Задание переменных по умолчанию
   ===============================*/

//    timerFlag = true; //таймер по умолчанию включен
//    tIn = 30000; // задание интервала обновления по умолчанию
    hName = "localhost"; //имя хоста по умолчанию
    bName = "PolusGold"; //имя БД по умолчанию
    uName = "Naursul"; //Логин по умолчанию (admin)                       УДАЛИТЬ!!!
    uPass = "ynjwiosx145449821"; //Пароль по умолчанию                    УДАЛИТЬ!!!
//    uName = "toster"; //Логин по умолчанию (user)                         УДАЛИТЬ!!!
//    uPass = "tosterpass"; //Пароль по умолчанию                           УДАЛИТЬ!!!

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

    if (db.open())                                                             //ТЕСТОВАЯ ВСТАВКА
    {
        db.close();
        ui->plainTextEdit->appendPlainText("Подключение произошло... типа");
        RoleCheck("Select Operator.idOperator, Operator.Role from Operator where Operator.BaseName like '" + uName + "'");  //Проверка статуса пользователя и его ID
    }

    if (Role == "User")
    {
        ui->tabWidget->removeTab(1);
    }
    loadData();  //Загрузка данных из БД в рабочую форму

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
    if (fileName != NULL)
    {
        QFile inFile;
        inFile.setFileName(fileName);
        if(inFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&inFile);
            while (!in.atEnd())
            {
                QString line = in.readLine(1024);
                QStringList fields = line.split(" = ");
                if (fields.at(0) == "[Host Name]")   //чтение имени хоста
                {
                    hName = fields.at(1).trimmed();
                    hna = false;
                }
                if (fields.at(0) == "[Timer ON]")     //чтение статуса таймера
                {
                    timerFlag = fields.at(1).trimmed().toInt();
                }
                if (fields.at(0) == "[Timer Interval]")   //чтение времени таймера
                {
                    tIn = fields.at(1).trimmed().toInt();
                    ti = false;
                }
            }
        }
        else
        {
            QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, tr("Ошибка"), tr("Не могу открыть файл настроек!\n\nБудут использоваться значения по умолчанию.") + fileName, QMessageBox::Close, this);
            msgBox->exec();
            hna = false;
            ti = false;
        }
    }
    if (hna)
    {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, tr("Ошибка"), tr("Ошибка в файле настроек!\n\nБудет использоваться имя хоста по умолчанию.") + fileName, QMessageBox::Close, this);
        msgBox->exec();
    }
    if (ti)
    {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Warning, tr("Ошибка"), tr("Ошибка в файле настроек!\n\nБудет использоваться интервал обновления по умолчанию.") + fileName, QMessageBox::Close, this);
        msgBox->exec();
    }
}*/

/* ==========================
    процедура запроса пароля
   ========================*/

void MainWindow::askPass()
{
    QString usverName, usverPass;
    Passcard = new LogPass(this, &usverName, &usverPass);
        switch (Passcard->exec())
        {
            case QDialog::Accepted:
                uName = usverName; //установка имени пользователя для подключения
                uPass = usverPass; //установка пароля для входа
                db.setUserName(uName); //установка имени пользователя для подключения
                db.setPassword(uPass); //установка пароля для входа
                if (db.open())
                {
                    db.close();
                    QTimer::singleShot(0, this, SLOT(enableMain()));
                    RoleCheck("Select Operator.idOperator, Operator.Role from Operator where Operator.BaseName like '" + uName + "'");  //Проверка статуса пользователя и его ID
                }
                else
                {
                    QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("\nНеверно указан логин/пароль!"), QMessageBox::Close, this);
                    msgBox->exec();
                    QTimer::singleShot(0, this, SLOT(enableMain()));
                }
                break;
            case QDialog::Rejected:
                QTimer::singleShot(0, this, SLOT(close()));
                break;
            default:
                QTimer::singleShot(0, this, SLOT(close()));
                break;
         }
}

/* ================================================
    реакция на проверку правильности логина/пароля
   ==============================================*/

void MainWindow::enableMain(void)
{
    if (db.open())
    {
        db.close();
        ui->centralWidget->setEnabled(true);
    }
    else
    {
        ui->centralWidget->setEnabled(false);
    }
}

/* ===============================
    проверка статуса пользователя
   =============================*/

void MainWindow::RoleCheck(QString queryText)
{
    if (db.open())
    {
        QSqlQuery query(db);
        if (query.exec(queryText))
        {
            while (query.next())
            {
                IDName.append(query.value(0).toString());
                Role.append(query.value(1).toString());
            }
            ui->plainTextEdit->appendPlainText("Получен статус пользователя (" + Role + ") и его ID (" + IDName +")");
        }
        else
        {
            ui->plainTextEdit->setPlainText("Бяка произошла, не работает у тебя нихрена, не получается запрос к базе для получения роли оператора и его ID :(");
            QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), query.lastError().text()); //вывод сообщения об ошибке
            msgBox->exec();
        }
        db.close();
    }
    else
    {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("\nОшибка подключения к БД"), QMessageBox::Close, this);
        msgBox->exec();
    }
}

/* ============================
    Загрузка данных для работы
   ==========================*/

void MainWindow::loadData()
{
    if (Role == "Admin")
    {
        LoadOperators("Select " + usFields + " from Operator order by Operator.OperatorName"); //Загрузка и создание списка операторов
    }
    LoadProdGroup("Select " + prodFields + " from ProdGroup where ProdGroup.Operator_idOperator like " + IDName + " order by ProdGroup.ProdGroupName"); //Загрузка и создание списка групп продукции у определенного пользователя
}

/* ============================
    Загрузка списка операторов
   ==========================*/

void MainWindow::LoadOperators(QString queryText)
{
    if (db.open())
    {
        QSqlQuery query(db);
        if (query.exec(queryText))
        {
            rowsOp = 0;
            idOpList.clear(); //очистка списка ID орераторов
            idOpNList.clear(); //очистка списка имен операторов
            idOpNBList.clear(); //очистка списка имен операторов в БД
            idOpRList.clear(); //очистка списка ролей операторов
            while (query.next())
            {
//                QListWidgetItem *newItem = new QListWidgetItem;
                idOpList.append(query.value(0).toString());
                idOpNList.append(query.value(1).toString());
                idOpNBList.append(query.value(2).toString());
                idOpRList.append(query.value(3).toString());
 //               newItem->setText(query.value(1).toString());
 //               ui->OrglistWidget->addItem(newItem);
                rowsOp++;
            }
            ui->plainTextEdit->appendPlainText("Список операторов создан");
        }
        else
        {
            ui->plainTextEdit->setPlainText("Бяка произошла, не работает у тебя нихрена, не получается запрос к базе для загрузки списка операторов! :(");
            QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), query.lastError().text()); //вывод сообщения об ошибке
            msgBox->exec();
        }
        db.close();
    }
    else
    {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("\nОшибка подключения к БД"), QMessageBox::Close, this);
        msgBox->exec();
    }
}

/* =================================
    Загрузка списка групп продукции
   ===============================*/

void MainWindow::LoadProdGroup(QString queryText)
{
    if (db.open())
    {
        QSqlQuery query(db);
        if (query.exec(queryText))
        {
            rowsPro = 0;
            ui->ProdTypes->clear(); //очистка виджета списков продукции
            idProList.clear(); //Очистка списка ID типов продукции
            idProNList.clear(); //Очистка списка наименований типов продукции
            while (query.next())
            {
                QListWidgetItem *newItem = new QListWidgetItem;
                idProList.append(query.value(0).toString());
                idProNList.append(query.value(1).toString());
                newItem->setText(query.value(1).toString());
                ui->ProdTypes->addItem(newItem);
                rowsPro++;
            }
            ui->plainTextEdit->appendPlainText("Список типов продукции создан");
        }
        else
        {
            ui->plainTextEdit->setPlainText("Бяка произошла, не работает у тебя нихрена, не получается запрос к базе для загрузки списка Типов продукции! :(");
            QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), query.lastError().text()); //вывод сообщения об ошибке
            msgBox->exec();
        }
        db.close();
    }
    else
    {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("\nОшибка подключения к БД"), QMessageBox::Close, this);
        msgBox->exec();
    }
    if (rowsPro > 0)
    {
    ui->ProdTypes->setCurrentRow(0);
    }
}

/* ===============================
    Загрузка данных для обработки
   =============================*/

void MainWindow::LoadWorkTable(QString queryText)
{
    int row = ui->WorkTable->rowCount();
    QString www;
    for (int i = row-1; i >= 0; i--)
    {
    ui->plainTextEdit->appendPlainText("количество строк в таблице:" + www.setNum(i));
    ui->WorkTable->removeRow(i);
    }
    row = 0;
    if (db.open())
    {
        QSqlQuery query(db);
        if (query.exec(queryText))
        {
            QStringList fields = workFields.split(", ");
            QString qqq;
            ui->plainTextEdit->appendPlainText("Количество столбцов в рабочей таблице = " + qqq.setNum(fields.count()));
            ui->WorkTable->setColumnCount(fields.count());
            while (query.next())
            {
                ui->WorkTable->insertRow(row);
                for (int col = 0; col < fields.count(); col++)
                {
                    QTableWidgetItem *tItem = new QTableWidgetItem(query.value(col).toString());
                    ui->WorkTable->setItem(row, col, tItem);
                }
                row++;
            }
        }
        else
        {
            ui->plainTextEdit->setPlainText("Бяка произошла, не работает у тебя нихрена, не получается запрос для загрузки рабочей таблицы :(");
            QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), query.lastError().text()); //вывод сообщения об ошибке
            msgBox->exec();
        }
        db.close();
    }
    else
    {
        QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("\nОшибка подключения к БД"), QMessageBox::Close, this);
        msgBox->exec();
    }
}

/* ======================================================
    Реакция на изменение строки в списке типов продукции
   ====================================================*/

void MainWindow::on_ProdTypes_currentRowChanged(int currentRow)
{
    LoadWorkTable("select " + workFields + " from WorkTable join IshodMas on (WorkTable.IshodMas_idIshodMas = IshodMas.idIshodMas) left join ProdGroup on (WorkTable.ProdGroup_idProdGroup = ProdGroup.idProdGroup) where ProdGroup.idProdGroup = " + idProList.at(currentRow));
}
