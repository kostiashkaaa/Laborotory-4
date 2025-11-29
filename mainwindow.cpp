// mainwindow.cpp
// Реализация главного окна приложения АТС

#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QGroupBox>
#include <QToolBar> // <--- Важно для работы панели
#include "addtariffdialog.h"
#include "addclientdialog.h"
#include "addvipclientdialog.h"
#include "addcalldialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), dataManager(new DataManager()) {

    setWindowTitle("Система управления АТС");
    setMinimumSize(1000, 700);

    // Создание центрального виджета
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 1. Настройка меню и панели инструментов
    setupMenuBar();
    setupToolBar(); // <--- Вызов настройки панели кнопок

    // 2. Создание панели вкладок
    QTabWidget *tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // Создание вкладок
    QWidget *tariffsTab = new QWidget();
    QWidget *clientsTab = new QWidget();
    QWidget *vipClientsTab = new QWidget();
    QWidget *callsTab = new QWidget();

    tabWidget->addTab(tariffsTab, "Тарифы");
    tabWidget->addTab(clientsTab, "Клиенты");
    tabWidget->addTab(vipClientsTab, "VIP-клиенты");
    tabWidget->addTab(callsTab, "Звонки");

    // Настройка содержимого вкладок
    setupTariffsTab();
    setupClientsTab();
    setupVIPClientsTab();
    setupCallsTab();

    // Метка статистики
    statsLabel = new QLabel(this);
    statsLabel->setStyleSheet("QLabel { padding: 10px; background-color: #f0f0f0; border-radius: 5px; font-weight: bold; }");
    mainLayout->addWidget(statsLabel);

    // Инициализация тестовыми данными
    dataManager->initializeTestData();
    updateTariffsTable();
    updateClientsTable();
    updateVIPClientsTable();
    updateCallsTable();
    updateStatistics();

    // === Layouts для вкладок ===

    // --- Вкладка Тарифы ---
    QVBoxLayout *tariffsLayout = new QVBoxLayout(tariffsTab);
    tariffsLayout->addWidget(tariffsTable);
    QHBoxLayout *tariffsButtons = new QHBoxLayout();
    QPushButton *addTariffBtn = new QPushButton("Добавить тариф");
    QPushButton *editTariffBtn = new QPushButton("Редактировать");
    QPushButton *deleteTariffBtn = new QPushButton("Удалить");
    QPushButton *sortTariffsBtn = new QPushButton("Сортировать");
    tariffsButtons->addWidget(addTariffBtn);
    tariffsButtons->addWidget(editTariffBtn);
    tariffsButtons->addWidget(deleteTariffBtn);
    tariffsButtons->addWidget(sortTariffsBtn);
    tariffsLayout->addLayout(tariffsButtons);

    connect(addTariffBtn, &QPushButton::clicked, this, &MainWindow::onAddTariff);
    connect(editTariffBtn, &QPushButton::clicked, this, &MainWindow::onEditTariff);
    connect(deleteTariffBtn, &QPushButton::clicked, this, &MainWindow::onDeleteTariff);
    connect(sortTariffsBtn, &QPushButton::clicked, this, &MainWindow::onSortTariffs);

    // --- Вкладка Клиенты ---
    QVBoxLayout *clientsLayout = new QVBoxLayout(clientsTab);
    clientsLayout->addWidget(clientsTable);
    QHBoxLayout *clientsButtons = new QHBoxLayout();
    QPushButton *addClientBtn = new QPushButton("Добавить клиента");
    QPushButton *editClientBtn = new QPushButton("Редактировать");
    QPushButton *deleteClientBtn = new QPushButton("Удалить");
    QPushButton *sortClientsBtn = new QPushButton("Сортировать");
    clientsButtons->addWidget(addClientBtn);
    clientsButtons->addWidget(editClientBtn);
    clientsButtons->addWidget(deleteClientBtn);
    clientsButtons->addWidget(sortClientsBtn);
    clientsLayout->addLayout(clientsButtons);

    connect(addClientBtn, &QPushButton::clicked, this, &MainWindow::onAddClient);
    connect(editClientBtn, &QPushButton::clicked, this, &MainWindow::onEditClient);
    connect(deleteClientBtn, &QPushButton::clicked, this, &MainWindow::onDeleteClient);
    connect(sortClientsBtn, &QPushButton::clicked, this, &MainWindow::onSortClients);

    // --- Вкладка VIP-клиенты ---
    QVBoxLayout *vipClientsLayout = new QVBoxLayout(vipClientsTab);
    vipClientsLayout->addWidget(vipClientsTable);
    QHBoxLayout *vipClientsButtons = new QHBoxLayout();
    QPushButton *addVIPClientBtn = new QPushButton("Добавить VIP-клиента");
    QPushButton *editVIPClientBtn = new QPushButton("Редактировать");
    QPushButton *deleteVIPClientBtn = new QPushButton("Удалить");
    QPushButton *sortVIPClientsBtn = new QPushButton("Сортировать");
    vipClientsButtons->addWidget(addVIPClientBtn);
    vipClientsButtons->addWidget(editVIPClientBtn);
    vipClientsButtons->addWidget(deleteVIPClientBtn);
    vipClientsButtons->addWidget(sortVIPClientsBtn);
    vipClientsLayout->addLayout(vipClientsButtons);

    connect(addVIPClientBtn, &QPushButton::clicked, this, &MainWindow::onAddVIPClient);
    connect(editVIPClientBtn, &QPushButton::clicked, this, &MainWindow::onEditVIPClient);
    connect(deleteVIPClientBtn, &QPushButton::clicked, this, &MainWindow::onDeleteVIPClient);
    connect(sortVIPClientsBtn, &QPushButton::clicked, this, &MainWindow::onSortVIPClients);

    // --- Вкладка Звонки ---
    QVBoxLayout *callsLayout = new QVBoxLayout(callsTab);
    callsLayout->addWidget(callsTable);
    QHBoxLayout *callsButtons = new QHBoxLayout();
    QPushButton *addCallBtn = new QPushButton("Добавить звонок");
    QPushButton *deleteCallBtn = new QPushButton("Удалить");
    QPushButton *sortCallsBtn = new QPushButton("Сортировать");
    QPushButton *statsBtn = new QPushButton("Статистика");
    callsButtons->addWidget(addCallBtn);
    callsButtons->addWidget(deleteCallBtn);
    callsButtons->addWidget(sortCallsBtn);
    callsButtons->addWidget(statsBtn);
    callsLayout->addLayout(callsButtons);

    connect(addCallBtn, &QPushButton::clicked, this, &MainWindow::onAddCall);
    connect(deleteCallBtn, &QPushButton::clicked, this, &MainWindow::onDeleteCall);
    connect(sortCallsBtn, &QPushButton::clicked, this, &MainWindow::onSortCalls);
    connect(statsBtn, &QPushButton::clicked, this, &MainWindow::onShowCallStatistics);
}

MainWindow::~MainWindow() {
    delete dataManager;
}

// ============ Инициализация UI ============

void MainWindow::setupToolBar() {
    QToolBar *toolbar = addToolBar("Main Toolbar");
    toolbar->setMovable(false); // Зафиксировать панель
    toolbar->setIconSize(QSize(24, 24));

    // Кнопка Сохранить
    QAction *saveAction = toolbar->addAction("💾 Сохранить базу");
    saveAction->setToolTip("Сохранить данные в CSV файл");
    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveData);

    // Кнопка Загрузить
    QAction *loadAction = toolbar->addAction("📂 Загрузить базу");
    loadAction->setToolTip("Загрузить данные из CSV файла");
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadData);

    toolbar->addSeparator(); // Разделитель

    // Кнопка Очистить
    QAction *clearAction = toolbar->addAction("🗑️ Очистить всё");
    clearAction->setToolTip("Удалить все данные из памяти");
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClearAllData);
}

void MainWindow::setupTariffsTab() {
    tariffsTable = new QTableWidget();
    tariffsTable->setColumnCount(3);
    tariffsTable->setHorizontalHeaderLabels({"Город", "Цена/мин (₽)", "Плата за подключение (₽)"});
    tariffsTable->horizontalHeader()->setStretchLastSection(true);
    tariffsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    tariffsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::setupClientsTab() {
    clientsTable = new QTableWidget();
    clientsTable->setColumnCount(3);
    clientsTable->setHorizontalHeaderLabels({"Имя", "Телефон", "Баланс (₽)"});
    clientsTable->horizontalHeader()->setStretchLastSection(true);
    clientsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    clientsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::setupVIPClientsTab() {
    vipClientsTable = new QTableWidget();
    vipClientsTable->setColumnCount(5);
    vipClientsTable->setHorizontalHeaderLabels({"Имя", "Телефон", "Баланс (₽)", "Скидка (%)", "Персональный менеджер"});
    vipClientsTable->horizontalHeader()->setStretchLastSection(true);
    vipClientsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    vipClientsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::setupCallsTab() {
    callsTable = new QTableWidget();
    callsTable->setColumnCount(4);
    callsTable->setHorizontalHeaderLabels({"Абонент", "Направление", "Длительность (мин)", "Стоимость (₽)"});
    callsTable->horizontalHeader()->setStretchLastSection(true);
    callsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    callsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::setupMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("Файл");
    QAction *saveAction = fileMenu->addAction("Сохранить данные");
    QAction *loadAction = fileMenu->addAction("Загрузить данные");
    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction("Выход");

    QMenu *dataMenu = menuBar->addMenu("Данные");
    QAction *initTestAction = dataMenu->addAction("Загрузить тестовые данные");
    QAction *clearAction = dataMenu->addAction("Очистить все данные");

    QMenu *helpMenu = menuBar->addMenu("Справка");
    QAction *aboutAction = helpMenu->addAction("О программе");

    connect(saveAction, &QAction::triggered, this, &MainWindow::onSaveData);
    connect(loadAction, &QAction::triggered, this, &MainWindow::onLoadData);
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    connect(initTestAction, &QAction::triggered, this, &MainWindow::onInitTestData);
    connect(clearAction, &QAction::triggered, this, &MainWindow::onClearAllData);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

// ============ Обновление таблиц ============

void MainWindow::updateTariffsTable() {
    tariffsTable->setRowCount(0);
    const auto& tariffs = dataManager->getTariffs();

    for (size_t i = 0; i < tariffs.size(); ++i) {
        tariffsTable->insertRow(i);
        tariffsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(tariffs[i].getCity())));
        tariffsTable->setItem(i, 1, new QTableWidgetItem(QString::number(tariffs[i].getPricePerMinute(), 'f', 2)));
        tariffsTable->setItem(i, 2, new QTableWidgetItem(QString::number(tariffs[i].getConnectionFee(), 'f', 2)));
    }
}

void MainWindow::updateClientsTable() {
    clientsTable->setRowCount(0);
    const auto& clients = dataManager->getClients();

    for (size_t i = 0; i < clients.size(); ++i) {
        clientsTable->insertRow(i);
        clientsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(clients[i].getName())));
        clientsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(clients[i].getPhoneNumber())));
        clientsTable->setItem(i, 2, new QTableWidgetItem(QString::number(clients[i].getBalance(), 'f', 2)));
    }
}

void MainWindow::updateVIPClientsTable() {
    vipClientsTable->setRowCount(0);
    const auto& vipClients = dataManager->getVIPClients();

    for (size_t i = 0; i < vipClients.size(); ++i) {
        vipClientsTable->insertRow(i);
        vipClientsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(vipClients[i].getName())));
        vipClientsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(vipClients[i].getPhoneNumber())));
        vipClientsTable->setItem(i, 2, new QTableWidgetItem(QString::number(vipClients[i].getBalance(), 'f', 2)));
        vipClientsTable->setItem(i, 3, new QTableWidgetItem(QString::number(vipClients[i].getDiscount(), 'f', 2)));
        vipClientsTable->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(vipClients[i].getPersonalManager())));
    }
}

void MainWindow::updateCallsTable() {
    callsTable->setRowCount(0);
    const auto& calls = dataManager->getCalls();

    for (size_t i = 0; i < calls.size(); ++i) {
        callsTable->insertRow(i);
        callsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(calls[i].getCallerName())));
        callsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(calls[i].getDestination())));
        callsTable->setItem(i, 2, new QTableWidgetItem(QString::number(calls[i].getDuration())));
        callsTable->setItem(i, 3, new QTableWidgetItem(QString::number(calls[i].getCost(), 'f', 2)));
    }
}

void MainWindow::updateStatistics() {
    double totalRevenue = dataManager->calculateTotalRevenue();
    int totalCalls = dataManager->getCalls().size();
    int totalClients = dataManager->getClients().size() + dataManager->getVIPClients().size();

    QString stats = QString("📊 Статистика: Всего клиентов: %1 | Всего звонков: %2 | Общая выручка: %3 ₽")
                        .arg(totalClients)
                        .arg(totalCalls)
                        .arg(totalRevenue, 0, 'f', 2);

    statsLabel->setText(stats);
}

// ============ Слоты ============

void MainWindow::onAddTariff() {
    AddTariffDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        dataManager->addTariff(dialog.getTariff());
        updateTariffsTable();
        showMessage("Успех", "Тариф успешно добавлен!");
    }
}

void MainWindow::onEditTariff() {
    int currentRow = tariffsTable->currentRow();
    if (currentRow < 0) {
        showError("Выберите тариф для редактирования!");
        return;
    }

    const auto& tariffs = dataManager->getTariffs();
    AddTariffDialog dialog(this, tariffs[currentRow]);
    if (dialog.exec() == QDialog::Accepted) {
        dataManager->updateTariff(currentRow, dialog.getTariff());
        updateTariffsTable();
        showMessage("Успех", "Тариф успешно обновлен!");
    }
}

void MainWindow::onDeleteTariff() {
    int currentRow = tariffsTable->currentRow();
    if (currentRow < 0) {
        showError("Выберите тариф для удаления!");
        return;
    }

    dataManager->removeTariff(currentRow);
    updateTariffsTable();
    showMessage("Успех", "Тариф успешно удален!");
}

void MainWindow::onSortTariffs() {
    dataManager->sortTariffsByPrice(true);
    updateTariffsTable();
}

void MainWindow::onAddClient() {
    AddClientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        dataManager->addClient(dialog.getClient());
        updateClientsTable();
        updateStatistics();
        showMessage("Успех", "Клиент успешно добавлен!");
    }
}

void MainWindow::onEditClient() {
    int currentRow = clientsTable->currentRow();
    if (currentRow < 0) {
        showError("Выберите клиента для редактирования!");
        return;
    }

    const auto& clients = dataManager->getClients();
    AddClientDialog dialog(this, clients[currentRow]);
    if (dialog.exec() == QDialog::Accepted) {
        dataManager->updateClient(currentRow, dialog.getClient());
        updateClientsTable();
        showMessage("Успех", "Клиент успешно обновлен!");
    }
}

void MainWindow::onDeleteClient() {
    int currentRow = clientsTable->currentRow();
    if (currentRow < 0) {
        showError("Выберите клиента для удаления!");
        return;
    }

    dataManager->removeClient(currentRow);
    updateClientsTable();
    updateStatistics();
    showMessage("Успех", "Клиент успешно удален!");
}

void MainWindow::onSortClients() {
    dataManager->sortClientsByName(true);
    updateClientsTable();
}

void MainWindow::onAddVIPClient() {
    AddVIPClientDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        dataManager->addVIPClient(dialog.getVIPClient());
        updateVIPClientsTable();
        updateStatistics();
        showMessage("Успех", "VIP-клиент успешно добавлен!");
    }
}

void MainWindow::onEditVIPClient() {
    int currentRow = vipClientsTable->currentRow();
    if (currentRow < 0) {
        showError("Выберите VIP-клиента для редактирования!");
        return;
    }

    const auto& vipClients = dataManager->getVIPClients();
    AddVIPClientDialog dialog(this, vipClients[currentRow]);
    if (dialog.exec() == QDialog::Accepted) {
        dataManager->updateVIPClient(currentRow, dialog.getVIPClient());
        updateVIPClientsTable();
        showMessage("Успех", "VIP-клиент успешно обновлен!");
    }
}

void MainWindow::onDeleteVIPClient() {
    int currentRow = vipClientsTable->currentRow();
    if (currentRow < 0) {
        showError("Выберите VIP-клиента для удаления!");
        return;
    }

    dataManager->removeVIPClient(currentRow);
    updateVIPClientsTable();
    updateStatistics();
    showMessage("Успех", "VIP-клиент успешно удален!");
}

void MainWindow::onSortVIPClients() {
    dataManager->sortVIPClientsByDiscount(true);
    updateVIPClientsTable();
}

void MainWindow::onAddCall() {
    AddCallDialog dialog(this, dataManager);
    if (dialog.exec() == QDialog::Accepted) {
        Call newCall = dialog.getCall();

        // ВАЖНО: Проверяем, успешно ли добавлен звонок (защита от несуществующих клиентов)
        if (dataManager->addCall(newCall)) {
            updateCallsTable();
            updateStatistics();
            showMessage("Успех", "Звонок успешно добавлен!");
        } else {
            showError("Ошибка: Не удалось добавить звонок. Клиент не найден в базе данных.");
        }
    }
}

void MainWindow::onDeleteCall() {
    int currentRow = callsTable->currentRow();
    if (currentRow < 0) {
        showError("Выберите звонок для удаления!");
        return;
    }

    dataManager->removeCall(currentRow);
    updateCallsTable();
    updateStatistics();
    showMessage("Успех", "Звонок успешно удален!");
}

void MainWindow::onSortCalls() {
    dataManager->sortCallsByDuration(false);
    updateCallsTable();
}

void MainWindow::onShowCallStatistics() {
    QString stats = "📈 Статистика по звонкам:\n\n";

    const auto& clients = dataManager->getClients();
    for (const auto& client : clients) {
        int callCount = dataManager->getClientCallCount(client.getName());
        double totalCost = dataManager->calculateClientTotalCost(client.getName());
        if (callCount > 0) {
            stats += QString("%1: %2 звонков, сумма: %3 ₽\n")
                         .arg(QString::fromStdString(client.getName()))
                         .arg(callCount)
                         .arg(totalCost, 0, 'f', 2);
        }
    }

    const auto& vipClients = dataManager->getVIPClients();
    for (const auto& vip : vipClients) {
        int callCount = dataManager->getClientCallCount(vip.getName());
        double totalCost = dataManager->calculateClientTotalCost(vip.getName());
        if (callCount > 0) {
            stats += QString("%1 (VIP): %2 звонков, сумма: %3 ₽\n")
                         .arg(QString::fromStdString(vip.getName()))
                         .arg(callCount)
                         .arg(totalCost, 0, 'f', 2);
        }
    }

    showMessage("Статистика", stats);
}

void MainWindow::onSaveData() {
    QString filename = QFileDialog::getSaveFileName(this, "Сохранить данные", "", "CSV Files (*.csv)");
    if (!filename.isEmpty()) {
        if (dataManager->saveToFile(filename.toStdString())) {
            showMessage("Успех", "Данные успешно сохранены!");
        } else {
            showError("Ошибка при сохранении данных!");
        }
    }
}

void MainWindow::onLoadData() {
    QString filename = QFileDialog::getOpenFileName(this, "Загрузить данные", "", "CSV Files (*.csv)");
    if (!filename.isEmpty()) {
        if (dataManager->loadFromFile(filename.toStdString())) {
            updateTariffsTable();
            updateClientsTable();
            updateVIPClientsTable();
            updateCallsTable();
            updateStatistics();
            showMessage("Успех", "Данные успешно загружены!");
        } else {
            showError("Ошибка при загрузке данных!");
        }
    }
}

void MainWindow::onInitTestData() {
    dataManager->initializeTestData();
    updateTariffsTable();
    updateClientsTable();
    updateVIPClientsTable();
    updateCallsTable();
    updateStatistics();
    showMessage("Успех", "Тестовые данные загружены!");
}

void MainWindow::onClearAllData() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение",
                                                              "Вы уверены, что хотите очистить все данные?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        dataManager->clearAll();
        updateTariffsTable();
        updateClientsTable();
        updateVIPClientsTable();
        updateCallsTable();
        updateStatistics();
        showMessage("Успех", "Все данные очищены!");
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "О программе",
                       "Система управления АТС\n\nВерсия 1.0\n\nЛабораторная работа №4\nДесктопное приложение на Qt");
}

void MainWindow::showMessage(const QString& title, const QString& message) {
    QMessageBox::information(this, title, message);
}

void MainWindow::showError(const QString& message) {
    QMessageBox::critical(this, "Ошибка", message);
}
