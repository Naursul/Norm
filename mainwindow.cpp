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
    workFields = "WorkTable.FlagReady, ProdGroup.ProdGroupName, IshodMas.idIshodMas, IshodMas.IshNameShort, WorkTable.WorkTablecol, WorkTable.WorkTablecol1, WorkTable.WorkTablecol2"; //список полей для работы
    HFields << tr("Готово") << tr("Тип продукции") << tr("ID") << tr("Исходное наименование");

/* =============================
    Задание цветового выделения
   ===========================*/

    worCol.setRgb(255, 255, 255);
    okCol.setRgb(198, 255, 203);

/* =================================
    Задание переменных по умолчанию
   ===============================*/

    filFrameVisible = false;
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

/* ==========================================================================
    далее идут настройки видимости/невидимости различных полей главного окна
   ========================================================================*/

    ui->frame_2->setVisible(filFrameVisible);
    if (Role == "User")
    {
        ui->tabWidget->removeTab(1);
    }

    loadData();  //Загрузка данных из БД в рабочую форму
    on_addFilItem_clicked();
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

/* ==============================================================================
    Инициализация (очистка) всех листов, переменных для работы в рабочей таблице
   ============================================================================*/

void MainWindow::initWorkTable()
{
    sList.clear();
    curSPoz = 0;
    ui->SearchNext->setEnabled(false);
    ui->SearchPrev->setEnabled(false);
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
    disconnect(ui->ProdTypes, SIGNAL(currentRowChanged(int)), this, SLOT(ProdTypesCurrentRowChanged(int)));
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
        connect(ui->ProdTypes, SIGNAL(currentRowChanged(int)), this, SLOT(ProdTypesCurrentRowChanged(int)));
        ui->ProdTypes->setCurrentRow(0);
    }
}

/* =========================================
    Загрузка данных для обработки в таблицу
   =======================================*/

void MainWindow::LoadWorkTable(QString queryText)
{
    int row = ui->WorkTable->rowCount();
    for (int i = row-1; i >= 0; i--)
    {
        ui->WorkTable->removeRow(i);
    }
    ui->statusBar->clearMessage();
    row = 0;
    if (db.open())
    {
        QSqlQuery query(db);
        if (query.exec(queryText))
        {
            fields.clear();
            fields = workFields.split(", ");
            ui->plainTextEdit->appendPlainText("Количество столбцов в рабочей таблице = " + tmpStr.setNum(fields.count()));
            ui->WorkTable->setColumnCount(fields.count());
            ui->WorkTable->setWordWrap(true);
            while (query.next())
            {
                ui->WorkTable->insertRow(row);
                ui->WorkTable->setRowHeight(row, 40);
                int colR = 0;
                if (fields.indexOf("WorkTable.FlagReady") >= 0)
                {
                    QTableWidgetItem *tItem = new QTableWidgetItem(); //"Готово"
                    tItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                    tItem->setCheckState((query.value(fields.indexOf("WorkTable.FlagReady")) == "1") ? Qt::Checked : Qt::Unchecked);
                    ui->WorkTable->setItem(row, fields.indexOf("WorkTable.FlagReady"), tItem);
                    ui->WorkTable->setColumnWidth(fields.indexOf("WorkTable.FlagReady"), 55);
                    colR++;
                }
                if (fields.indexOf("ProdGroup.ProdGroupName") >= 0)
                {
                    QTableWidgetItem *tItem = new QTableWidgetItem(query.value(fields.indexOf("ProdGroup.ProdGroupName")).toString());
                    tItem->setFlags(tItem->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
                    ui->WorkTable->setItem(row, fields.indexOf("ProdGroup.ProdGroupName"), tItem);
                    ui->WorkTable->setColumnWidth(fields.indexOf("ProdGroup.ProdGroupName"), 150);
                    colR++;
                }
                if (fields.indexOf("IshodMas.idIshodMas") >= 0)
                {
                    QTableWidgetItem *tItem = new QTableWidgetItem(query.value(fields.indexOf("IshodMas.idIshodMas")).toString());
                    tItem->setFlags(tItem->flags() ^ Qt::ItemIsEditable);
                    ui->WorkTable->setItem(row, fields.indexOf("IshodMas.idIshodMas"), tItem);
                    ui->WorkTable->setColumnWidth(fields.indexOf("IshodMas.idIshodMas"), 100);
                    colR++;
                }
                if (fields.indexOf("IshodMas.IshNameShort") >= 0)
                {
                    QTableWidgetItem *tItem = new QTableWidgetItem(query.value(fields.indexOf("IshodMas.IshNameShort")).toString());
                    tItem->setFlags(tItem->flags() ^ Qt::ItemIsEditable);
                    ui->WorkTable->setItem(row, fields.indexOf("IshodMas.IshNameShort"), tItem);
                    ui->WorkTable->setColumnWidth(fields.indexOf("IshodMas.IshNameShort"), 200);
                    colR++;
                }
                for (int col = 0; col < fields.count(); col++)
                {
                    if (col == fields.indexOf("WorkTable.FlagReady")) continue;
                    if (col == fields.indexOf("ProdGroup.ProdGroupName")) continue;
                    if (col == fields.indexOf("IshodMas.idIshodMas")) continue;
                    if (col == fields.indexOf("IshodMas.IshNameShort")) continue;
                    if (ui->WorkTable->columnCount() < fields.count())
                    {
                        ui->WorkTable->setColumnCount(ui->WorkTable->columnCount() + 1);
                    }
                    QTableWidgetItem *tItem = new QTableWidgetItem(query.value(col).toString());
                    ui->WorkTable->setItem(row, colR, tItem);
                    if (HFields.count() <= colR)
                    {
                        HFields.append(tr("Столбец ") + tmpStr.setNum(colR + 1));
                    }
                    ui->WorkTable->setColumnWidth(colR++, 200);
                }
                color(row);
                row++;
            }
            ui->WorkTable->setHorizontalHeaderLabels(HFields);
            ui->statusBar->showMessage(tr("Загружено позиций: ") + tmpStr.setNum(ui->WorkTable->rowCount()), 10000);
           // ui->WorkTable->resizeRowsToContents();
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

void MainWindow::ProdTypesCurrentRowChanged(int currentRow)
{
    initWorkTable();
    disconnect(ui->WorkTable, SIGNAL(cellChanged(int,int)), this, SLOT(WorkTableCellChanged(int,int)));
    LoadWorkTable("select " + workFields + " from WorkTable join IshodMas on (WorkTable.IshodMas_idIshodMas = IshodMas.idIshodMas) left join ProdGroup on (WorkTable.ProdGroup_idProdGroup = ProdGroup.idProdGroup) where ProdGroup.idProdGroup = " + idProList.at(currentRow));
    connect(ui->WorkTable, SIGNAL(cellChanged(int,int)), this, SLOT(WorkTableCellChanged(int,int)));
}

/* ========================================================
    Реакция на изменение значения ячейки в рабочей таблице
   ======================================================*/

void MainWindow::WorkTableCellChanged(int row, int column)
{
    if (column == fields.indexOf("WorkTable.FlagReady"))
    {
        disconnect(ui->WorkTable, SIGNAL(cellChanged(int,int)), this, SLOT(WorkTableCellChanged(int,int)));
        color(row);
        connect(ui->WorkTable, SIGNAL(cellChanged(int,int)), this, SLOT(WorkTableCellChanged(int,int)));
    }
    updateDataBase(row, column);
}

/* ===============================================
    установка цвета ячеек и запрет редактирования
   =============================================*/

void MainWindow::color(int row)
{
    for (int col = 0; col < ui->WorkTable->columnCount(); col++)
    {
        if (ui->WorkTable->item(row, fields.indexOf("WorkTable.FlagReady"))->checkState())  //если запись выполнена
        {
            ui->WorkTable->item(row, col)->setBackground(okCol);
            if ((ui->WorkTable->item(row, col)->flags() & Qt::ItemIsEditable) > 0)
            {
                ui->WorkTable->item(row, col)->setFlags(ui->WorkTable->item(row, col)->flags() ^ (Qt::ItemIsEditable));// | Qt::ItemIsSelectable));
            }
        }
        else
        {
            ui->WorkTable->item(row, col)->setBackground(worCol);
            if (((ui->WorkTable->item(row, col)->flags() ^ Qt::ItemIsEditable) > 0) & (col>3))
            {
                ui->WorkTable->item(row, col)->setFlags(ui->WorkTable->item(row, col)->flags() | (Qt::ItemIsEditable));// | Qt::ItemIsSelectable));
            }
        }
    }
}

/* ===================================
    Реакция на нажатие кнопки "Поиск"
   =================================*/

void MainWindow::on_SearchButton_clicked()
{
    QString search = ui->SearchLine->text().trimmed();
    if (search.length() > 0)
    {
        sList.clear();
        ui->SearchNext->setEnabled(false);
        ui->SearchPrev->setEnabled(false);
        ui->statusBar->clearMessage();
        for (int row = 0; row < ui->WorkTable->rowCount(); row++)
        {
            for (int col = 2; col < ui->WorkTable->columnCount(); col++)
            {
                if (ui->WorkTable->item(row, col)->text().contains(search, Qt::CaseInsensitive))
                {
                    QPoint coor;
                    coor.setX(row);
                    coor.setY(col);
                    sList.append(coor);
                }
            }
        }
        if (sList.length() > 1)
        {
            ui->SearchNext->setEnabled(true);
        }
        if (sList.length() > 0)
        {
            ui->statusBar->showMessage(tr("Найдено совпадений: ") + tmpStr.setNum(sList.length()), 10000);
            curSPoz = 0;
            ui->WorkTable->setCurrentCell(sList.at(curSPoz).x(), sList.at(curSPoz).y());
        }
        else
        {
            ui->statusBar->showMessage(tr("Совпадений не найдено"), 10000);
        }
    }
    else
    {
        ui->statusBar->showMessage(tr("Не задана строка поиска"), 10000);
    }
}

/* =======================================================
    Реакция на нажатие кнопки "Следующий в списке поиска"
   =====================================================*/

void MainWindow::on_SearchNext_clicked()
{
    curSPoz++;
    ui->WorkTable->setCurrentCell(sList.at(curSPoz).x(), sList.at(curSPoz).y());
    ui->SearchPrev->setEnabled(true);
    if (curSPoz >= (sList.count()-1))
    {
        ui->SearchNext->setEnabled(false);
    }
}

/* ========================================================
    Реакция на нажатие кнопки "Предыдущий в списке поиска"
   ======================================================*/

void MainWindow::on_SearchPrev_clicked()
{
    curSPoz--;
    ui->WorkTable->setCurrentCell(sList.at(curSPoz).x(), sList.at(curSPoz).y());
    ui->SearchNext->setEnabled(true);
    if (curSPoz <= 0)
    {
        ui->SearchPrev->setEnabled(false);
    }
}

/* ===========================================
    Добавление/изменение данных в Базе Данных
   =========================================*/

void MainWindow::updateDataBase(int row, int col)
{
    QString posID = ui->WorkTable->item(row, fields.indexOf("IshodMas.idIshodMas"))->text().trimmed();
    if (col == fields.indexOf("WorkTable.FlagReady"))  //для случая, если обновился чекбокс
    {
        if (db.open())
        {
            tmpStr = (ui->WorkTable->item(row, col)->checkState() == Qt::Checked)? "1" : "0";
            QSqlQuery query(db);
            if (query.exec("update WorkTable set " + fields.at(col) + " = '" + tmpStr.trimmed() + "' where WorkTable.IshodMas_idIshodMas = " + posID))
            {
                ui->statusBar->showMessage(tr("Данные обновлены"), 5000);
            }
            else
            {
                ui->plainTextEdit->setPlainText("Бяка произошла, не работает у тебя нихрена, чекбокс не обновляется :(");
                QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), query.lastError().text()); //вывод сообщения об ошибке
                msgBox->exec();
            }
        }
        else
        {
            QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("\nОшибка подключения к БД"), QMessageBox::Close, this);
            msgBox->exec();
        }
    }
    else  //для всех остальных полей
    {
        tmpStr = ui->WorkTable->item(row, col)->text().trimmed();
        if (db.open())
        {
            QSqlQuery query(db);
            if (query.exec("update WorkTable set " + fields.at(col) + " = '" + tmpStr.trimmed() + "' where WorkTable.IshodMas_idIshodMas = " + posID))
            {
                ui->statusBar->showMessage(tr("Данные обновлены"), 5000);
            }
            else
            {
                ui->plainTextEdit->setPlainText("Бяка произошла, не работает у тебя нихрена, данные не обновляются :(");
                QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), query.lastError().text()); //вывод сообщения об ошибке
                msgBox->exec();
            }
        }
        else
        {
            QMessageBox *msgBox = new QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("\nОшибка подключения к БД"), QMessageBox::Close, this);
            msgBox->exec();
        }
    }
}

/* ======================================
    Реакция на нажатие кнопки "Обновить"
   ====================================*/

void MainWindow::on_ReloadButton_clicked()
{
    LoadProdGroup("Select " + prodFields + " from ProdGroup where ProdGroup.Operator_idOperator like " + IDName + " order by ProdGroup.ProdGroupName"); //Загрузка и создание списка групп продукции у определенного пользователя
}

/* ==================================
    Реакция на выбор меню "Обновить"
   ================================*/

void MainWindow::on_ReloadAction_triggered()
{
    LoadProdGroup("Select " + prodFields + " from ProdGroup where ProdGroup.Operator_idOperator like " + IDName + " order by ProdGroup.ProdGroupName"); //Загрузка и создание списка групп продукции у определенного пользователя
}

/* ===================================
    Реакция на нажатие кнопки "Выход"
   =================================*/

void MainWindow::on_ExitButton_clicked()
{
    close();
}

/* ===============================
    Реакция на выбор меню "Выход"
   =============================*/

void MainWindow::on_ExitAction_triggered()
{
    close();
}

/* =============================================
    Реакция на нажатие кнопки "Включить фильтр"
   ===========================================*/

void MainWindow::on_FilterOn_clicked()
{
    filFrameVisible = !filFrameVisible;
    ui->frame_2->setVisible(filFrameVisible);
}

/* ===========================================
    Реакция на нажатие кнопки "Добавить поле"
   =========================================*/

void MainWindow::on_addFilItem_clicked()
{
    filList.clear();
    for (int col = 2; col < ui->WorkTable->columnCount(); col++)
    {
        filList.append(HFields.at(col));
    }
    ui->verticalLayout_2->addWidget(fItem = new FilItem(ui->frame_2, &filList));
}

/* ==============================================
    Реакция на нажатие кнопки "Применить фильтр"
   ============================================*/

void MainWindow::on_FilOn_clicked()
{
    QString filQuer = "", filQuer1 = "";
    QList<int> indCol; //индексы колонок
    QStringList filtxt; //текст запроса
    QList<bool> loockup; //запрос добавлен в строку запроса
    QObjectList qList = ui->frame_2->children();
    for ( int i = 0; i < qList.count(); i++)
    {
        if (qList.at(i)->objectName() == "FilItem")
        {
            FilItem *it = (FilItem *) qList.at(i);
            if (it->filItemOn())
            {
                indCol.append(it->filColumnIndex());
                if (it->filOptionIndex() > 1)
                {
                    filtxt.append(fields.at(HFields.indexOf(filList.at(it->filColumnIndex()))) + it->filOptionText() + it->filText() + "%'");
                }
                else
                {
                    filtxt.append(fields.at(HFields.indexOf(filList.at(it->filColumnIndex()))) + it->filOptionText() + it->filText() + "'");
                }
                loockup.append(true);
            }
        }
    }
    for (int i = 0; i < indCol.count(); i++)
    {
        if (loockup.at(i))
        {
            loockup.replace(i, false);
            filQuer1 = "(" + filtxt.at(i) + ")";
            for (int j = i+1; j < indCol.count(); j++)
            {
                if ((loockup.at(j)) && (indCol.at(j) == indCol.at(i)))
                {
                    loockup.replace(j, false);
                    filQuer1 = "(" + filQuer1 + " OR (" + filtxt.at(j) + "))";
                }
            }
            filQuer = filQuer + " AND " + filQuer1;
        }
    }
    initWorkTable();
    disconnect(ui->WorkTable, SIGNAL(cellChanged(int,int)), this, SLOT(WorkTableCellChanged(int,int)));
    LoadWorkTable("select " + workFields + " from WorkTable join IshodMas on (WorkTable.IshodMas_idIshodMas = IshodMas.idIshodMas) left join ProdGroup on (WorkTable.ProdGroup_idProdGroup = ProdGroup.idProdGroup) where (ProdGroup.idProdGroup = " + idProList.at(ui->ProdTypes->currentRow()) + ")" + filQuer);
    connect(ui->WorkTable, SIGNAL(cellChanged(int,int)), this, SLOT(WorkTableCellChanged(int,int)));
}

/* ==========================================
    Реакция на нажатие кнопки "Снять фильтр"
   ============================================*/

void MainWindow::on_FilterOff_clicked()
{
    initWorkTable();
    disconnect(ui->WorkTable, SIGNAL(cellChanged(int,int)), this, SLOT(WorkTableCellChanged(int,int)));
    LoadWorkTable("select " + workFields + " from WorkTable join IshodMas on (WorkTable.IshodMas_idIshodMas = IshodMas.idIshodMas) left join ProdGroup on (WorkTable.ProdGroup_idProdGroup = ProdGroup.idProdGroup) where ProdGroup.idProdGroup = " + idProList.at(ui->ProdTypes->currentRow()));
    connect(ui->WorkTable, SIGNAL(cellChanged(int,int)), this, SLOT(WorkTableCellChanged(int,int)));
    QObjectList qList = ui->frame_2->children();
    for ( int i = 0; i < qList.count(); i++)
    {
        if (qList.at(i)->objectName() == "FilItem")
        {
            FilItem *it = (FilItem *) qList.at(i);
            it->close();
        }
    }
    on_addFilItem_clicked();
    filFrameVisible = false;
    ui->frame_2->setVisible(filFrameVisible);
}
