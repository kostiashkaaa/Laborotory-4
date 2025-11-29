#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <vector>
#include <string>
#include "Tariff.h"
#include "Client.h"
#include "VIPClient.h"
#include "Call.h"

class DataManager {
private:
    std::vector<Tariff> tariffs;
    std::vector<Client> clients;
    std::vector<VIPClient> vipClients;
    std::vector<Call> calls;

    std::string dataFilePath;

public:
    DataManager();
    ~DataManager();

    // Управление тарифами
    void addTariff(const Tariff& tariff);
    void removeTariff(int index);
    void updateTariff(int index, const Tariff& tariff);
    const std::vector<Tariff>& getTariffs() const;
    Tariff* findTariffByCity(const std::string& city);

    // Управление клиентами
    void addClient(const Client& client);
    void removeClient(int index);
    void updateClient(int index, const Client& client);
    const std::vector<Client>& getClients() const;

    // НОВЫЙ МЕТОД: Проверка существования клиента
    bool clientExists(const std::string& name) const;

    // Управление VIP-клиентами
    void addVIPClient(const VIPClient& client);
    void removeVIPClient(int index);
    void updateVIPClient(int index, const VIPClient& client);
    const std::vector<VIPClient>& getVIPClients() const;

    // Управление звонками
    // ИЗМЕНЕНО: возвращает bool (успех/неудача)
    bool addCall(const Call& call);
    void removeCall(int index);
    const std::vector<Call>& getCalls() const;

    // Статистика
    double calculateClientTotalCost(const std::string& clientName) const;
    int getClientCallCount(const std::string& clientName) const;
    double calculateTotalRevenue() const;

    // Сортировка
    void sortTariffsByPrice(bool ascending = true);
    void sortClientsByName(bool ascending = true);
    void sortVIPClientsByDiscount(bool ascending = true);
    void sortCallsByDuration(bool ascending = true);

    // Работа с файлами
    bool saveToFile(const std::string& filename);
    bool loadFromFile(const std::string& filename);

    // Очистка данных
    void clearAll();

    // Инициализация тестовыми данными
    void initializeTestData();
};

#endif // DATAMANAGER_H
