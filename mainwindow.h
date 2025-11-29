// mainwindow.h
// Главное окно приложения АТС с вкладками и панелью инструментов

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QToolBar> // <--- Добавлен заголовочный файл для панели инструментов
#include "DataManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Слоты для тарифов
    void onAddTariff();
    void onEditTariff();
    void onDeleteTariff();
    void onSortTariffs();

    // Слоты для клиентов
    void onAddClient();
    void onEditClient();
    void onDeleteClient();
    void onSortClients();

    // Слоты для VIP-клиентов
    void onAddVIPClient();
    void onEditVIPClient();
    void onDeleteVIPClient();
    void onSortVIPClients();

    // Слоты для звонков
    void onAddCall();
    void onDeleteCall();
    void onSortCalls();
    void onShowCallStatistics();

    // Слоты для меню и панели инструментов
    void onSaveData();
    void onLoadData();
    void onInitTestData();
    void onClearAllData();
    void onAbout();

private:
    Ui::MainWindow *ui;
    DataManager *dataManager;

    // Таблицы
    QTableWidget *tariffsTable;
    QTableWidget *clientsTable;
    QTableWidget *vipClientsTable;
    QTableWidget *callsTable;

    // Метки статистики
    QLabel *statsLabel;

    // Методы обновления таблиц
    void updateTariffsTable();
    void updateClientsTable();
    void updateVIPClientsTable();
    void updateCallsTable();
    void updateStatistics();

    // Инициализация UI
    void setupTariffsTab();
    void setupClientsTab();
    void setupVIPClientsTab();
    void setupCallsTab();
    void setupMenuBar();
    void setupToolBar(); // <--- Новый метод для настройки панели кнопок

    // Вспомогательные методы
    void showMessage(const QString& title, const QString& message);
    void showError(const QString& message);
};

#endif // MAINWINDOW_H
