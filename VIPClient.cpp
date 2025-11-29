#include "VIPClient.h"
#include <iostream>

VIPClient::VIPClient() : Client(), discount(0.0), personalManager(""), loyaltyProgram() {}


VIPClient::VIPClient(const std::string& name, const std::string& phoneNumber,
                     double balance, double discount, const std::string& manager)
    : Client(name, phoneNumber, balance), discount(discount),
    personalManager(manager), loyaltyProgram("VIP", discount) {}

double VIPClient::getDiscount() const {
    return discount;
}

void VIPClient::setDiscount(double discount) {
    this->discount = discount;
}

std::string VIPClient::getPersonalManager() const {
    return personalManager;
}

void VIPClient::setPersonalManager(const std::string& manager) {
    this->personalManager = manager;
}

LoyaltyProgram& VIPClient::getLoyaltyProgram() {
    return loyaltyProgram;
}

void VIPClient::display() const {
    Client::display();
    std::cout << "Скидка: " << discount << "%" << std::endl;
    std::cout << "Персональный менеджер: " << personalManager << std::endl;
    loyaltyProgram.displayLoyaltyInfo();
}
