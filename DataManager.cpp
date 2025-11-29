#include "DataManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

DataManager::DataManager() : dataFilePath("atc_data.csv") {}

DataManager::~DataManager() {}

// ============================================
// УПРАВЛЕНИЕ ТАРИФАМИ
// ============================================

void DataManager::addTariff(const Tariff& tariff) {
    tariffs.push_back(tariff);
}

void DataManager::removeTariff(int index) {
    if (index >= 0 && index < static_cast<int>(tariffs.size())) {
        tariffs.erase(tariffs.begin() + index);
    }
}

void DataManager::updateTariff(int index, const Tariff& tariff) {
    if (index >= 0 && index < static_cast<int>(tariffs.size())) {
        tariffs[index] = tariff;
    }
}

const std::vector<Tariff>& DataManager::getTariffs() const {
    return tariffs;
}

Tariff* DataManager::findTariffByCity(const std::string& city) {
    for (auto& tariff : tariffs) {
        if (tariff.getCity() == city) {
            return &tariff;
        }
    }
    return nullptr;
}

// ============================================
// УПРАВЛЕНИЕ КЛИЕНТАМИ
// ============================================

void DataManager::addClient(const Client& client) {
    clients.push_back(client);
}

void DataManager::removeClient(int index) {
    if (index >= 0 && index < static_cast<int>(clients.size())) {
        clients.erase(clients.begin() + index);
    }
}

void DataManager::updateClient(int index, const Client& client) {
    if (index >= 0 && index < static_cast<int>(clients.size())) {
        clients[index] = client;
    }
}

const std::vector<Client>& DataManager::getClients() const {
    return clients;
}

// НОВАЯ ФУНКЦИЯ ПРОВЕРКИ
bool DataManager::clientExists(const std::string& name) const {
    for (const auto& client : clients) {
        if (client.getName() == name) return true;
    }
    for (const auto& vip : vipClients) {
        if (vip.getName() == name) return true;
    }
    return false;
}

// ============================================
// УПРАВЛЕНИЕ VIP-КЛИЕНТАМИ
// ============================================

void DataManager::addVIPClient(const VIPClient& client) {
    vipClients.push_back(client);
}

void DataManager::removeVIPClient(int index) {
    if (index >= 0 && index < static_cast<int>(vipClients.size())) {
        vipClients.erase(vipClients.begin() + index);
    }
}

void DataManager::updateVIPClient(int index, const VIPClient& client) {
    if (index >= 0 && index < static_cast<int>(vipClients.size())) {
        vipClients[index] = client;
    }
}

const std::vector<VIPClient>& DataManager::getVIPClients() const {
    return vipClients;
}

// ============================================
// УПРАВЛЕНИЕ ЗВОНКАМИ
// ============================================

// ОБНОВЛЕННАЯ ФУНКЦИЯ С ПРОВЕРКОЙ
bool DataManager::addCall(const Call& call) {
    // Защита: проверяем, существует ли клиент
    if (!clientExists(call.getCallerName())) {
        return false; // Клиент не найден, отказ
    }
    calls.push_back(call);
    return true; // Успех
}

void DataManager::removeCall(int index) {
    if (index >= 0 && index < static_cast<int>(calls.size())) {
        calls.erase(calls.begin() + index);
    }
}

const std::vector<Call>& DataManager::getCalls() const {
    return calls;
}

// ============================================
// СТАТИСТИКА
// ============================================

double DataManager::calculateClientTotalCost(const std::string& clientName) const {
    double total = 0.0;
    for (const auto& call : calls) {
        if (call.getCallerName() == clientName) {
            total += call.getCost();
        }
    }
    return total;
}

int DataManager::getClientCallCount(const std::string& clientName) const {
    int count = 0;
    for (const auto& call : calls) {
        if (call.getCallerName() == clientName) {
            count++;
        }
    }
    return count;
}

double DataManager::calculateTotalRevenue() const {
    double total = 0.0;
    for (const auto& call : calls) {
        total += call.getCost();
    }
    return total;
}

// ============================================
// СОРТИРОВКА
// ============================================

void DataManager::sortTariffsByPrice(bool ascending) {
    if (ascending) {
        std::sort(tariffs.begin(), tariffs.end(),
                  [](const Tariff& a, const Tariff& b) { return a.getPricePerMinute() < b.getPricePerMinute(); });
    } else {
        std::sort(tariffs.begin(), tariffs.end(),
                  [](const Tariff& a, const Tariff& b) { return a.getPricePerMinute() > b.getPricePerMinute(); });
    }
}

void DataManager::sortClientsByName(bool ascending) {
    if (ascending) {
        std::sort(clients.begin(), clients.end(),
                  [](const Client& a, const Client& b) { return a.getName() < b.getName(); });
    } else {
        std::sort(clients.begin(), clients.end(),
                  [](const Client& a, const Client& b) { return a.getName() > b.getName(); });
    }
}

void DataManager::sortVIPClientsByDiscount(bool ascending) {
    if (ascending) {
        std::sort(vipClients.begin(), vipClients.end(),
                  [](const VIPClient& a, const VIPClient& b) {
                      return a.getDiscount() < b.getDiscount();
                  });
    } else {
        std::sort(vipClients.begin(), vipClients.end(),
                  [](const VIPClient& a, const VIPClient& b) {
                      return a.getDiscount() > b.getDiscount();
                  });
    }
}

void DataManager::sortCallsByDuration(bool ascending) {
    if (ascending) {
        std::sort(calls.begin(), calls.end(),
                  [](const Call& a, const Call& b) { return a.getDuration() < b.getDuration(); });
    } else {
        std::sort(calls.begin(), calls.end(),
                  [](const Call& a, const Call& b) { return a.getDuration() > b.getDuration(); });
    }
}


bool DataManager::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // UTF-8 BOM
    file << (char)0xEF << (char)0xBB << (char)0xBF;

    file << "[TARIFFS]\n";
    for (const auto& tariff : tariffs) {
        file << tariff.getCity() << "," << tariff.getPricePerMinute() << "," << tariff.getConnectionFee() << "\n";
    }

    file << "[CLIENTS]\n";
    for (const auto& client : clients) {
        file << client.getName() << "," << client.getPhoneNumber() << "," << client.getBalance() << "\n";
    }

    file << "[VIP_CLIENTS]\n";
    for (const auto& vip : vipClients) {
        file << vip.getName() << "," << vip.getPhoneNumber() << ","
             << vip.getBalance() << "," << vip.getDiscount() << ","
             << vip.getPersonalManager() << "\n";
    }

    file << "[CALLS]\n";
    for (const auto& call : calls) {
        file << call.getCallerName() << "," << call.getDestination() << ","
             << call.getDuration() << "," << call.getCost() << "\n";
    }

    file.close();
    return true;
}

bool DataManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    clearAll();

    std::string line;
    std::string currentSection;

    char bom[3] = {0, 0, 0};
    file.read(bom, 3);
    if (bom[0] != (char)0xEF || bom[1] != (char)0xBB || bom[2] != (char)0xBF) {
        file.seekg(0);
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '[') {
            currentSection = line;
            continue;
        }

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (currentSection == "[TARIFFS]" && tokens.size() >= 3) {
            std::string city = tokens[0];
            double price = std::stod(tokens[1]);
            double fee = std::stod(tokens[2]);
            tariffs.push_back(Tariff(city, price, fee));
        }
        else if (currentSection == "[CLIENTS]" && tokens.size() >= 3) {
            std::string name = tokens[0];
            std::string phone = tokens[1];
            double balance = std::stod(tokens[2]);
            clients.push_back(Client(name, phone, balance));
        }
        else if (currentSection == "[VIP_CLIENTS]" && tokens.size() >= 5) {
            std::string name = tokens[0];
            std::string phone = tokens[1];
            double balance = std::stod(tokens[2]);
            double discount = std::stod(tokens[3]);
            std::string manager = tokens[4];
            vipClients.push_back(VIPClient(name, phone, balance, discount, manager));
        }
        // ПРОВЕРКА ПРИ ЗАГРУЗКЕ ЗВОНКОВ
        else if (currentSection == "[CALLS]" && tokens.size() >= 4) {
            std::string clientName = tokens[0];
            std::string city = tokens[1];
            int duration = std::stoi(tokens[2]);
            double cost = std::stod(tokens[3]);

            // ЗАЩИТА: Добавляем звонок ТОЛЬКО если клиент существует в уже загруженной базе
            if (clientExists(clientName)) {
                calls.push_back(Call(clientName, city, duration, cost));
            }
        }
    }

    file.close();
    return true;
}

void DataManager::clearAll() {
    tariffs.clear();
    clients.clear();
    vipClients.clear();
    calls.clear();
}

void DataManager::initializeTestData() {
    clearAll();
    tariffs.push_back(Tariff("Москва", 2.50, 0.50));
    tariffs.push_back(Tariff("Санкт-Петербург", 2.30, 0.50));
    tariffs.push_back(Tariff("Минск", 1.80, 0.20));
    tariffs.push_back(Tariff("Киев", 2.00, 0.30));

    clients.push_back(Client("Иванов", "+79001234567", 100.0));
    clients.push_back(Client("Петров", "+79007654321", 50.0));

    vipClients.push_back(VIPClient("Сидоров", "+79009876543", 500.0, 15.0, "Анна"));
    vipClients.push_back(VIPClient("Алексеев", "+79005551234", 1000.0, 25.0, "Мария"));
}
