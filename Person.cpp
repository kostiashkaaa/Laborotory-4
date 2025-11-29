#include "Person.h"
#include <iostream>

Person::Person() : name(""), phoneNumber("") {}

Person::Person(const std::string& name, const std::string& phoneNumber)
    : name(name), phoneNumber(phoneNumber) {}

Person::~Person() {}

std::string Person::getName() const {
    return name;
}

void Person::setName(const std::string& name) {
    this->name = name;
}

std::string Person::getPhoneNumber() const {
    return phoneNumber;
}

void Person::setPhoneNumber(const std::string& phone) {
    this->phoneNumber = phone;
}

void Person::display() const {
    std::cout << "Имя: " << name << ", Телефон: " << phoneNumber << std::endl;
}
