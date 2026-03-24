#include <iostream>
#include <limits>
#include "Database.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    Database db("../db/tourist_bureau.db");
    std::cout << "База данных открыта\n";

    std::string login, password;
    std::cout << "Логин: ";
    std::cin >> login;
    std::cout << "Пароль: ";
    std::cin >> password;

    User* user = db.authenticateUser(login, password);
    if (!user) {
        std::cout << "Неверный логин/пароль\n";
        return 1;
    }

    std::cout << "Привет, " << user->full_name << " (" << user->role << ")\n";

    bool running = true;
    while (running) {
        std::cout << "\nМеню:\n"
                  << "1. Посмотреть все автобусы\n"
                  << "2. Посмотреть маршруты\n"
                  << "3. Просмотр состава экипажа автобуса\n"
                  << "4. Просмотр поездок по автобусу\n"
                  << "5. Добавить члена экипажа\n"
                  << "6. Обновить пробег автобуса\n"
                  << "7. Выйти\n"
                  << "Выбор: ";
        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            auto buses = db.getAllBuses();
            for (auto& bus : buses)
                std::cout << bus.id << " | " << bus.bus_number << " | " << bus.model_name
                          << " | Пробег: " << bus.total_mileage << "\n";
            break;
        }
        case 2: {
            auto routes = db.getAllRoutes();
            for (auto& r : routes)
                std::cout << r.id << " | " << r.route_name << " | "
                          << r.start_point << " -> " << r.end_point
                          << " | Дистанция: " << r.distance_km << " км\n";
            break;
        }
        case 3: {
            int bus_id;
            std::cout << "Введите ID автобуса: ";
            std::cin >> bus_id;
            auto crew = db.getCrewByBus(bus_id);
            if (crew.empty()) {
                std::cout << "Экипаж не найден\n";
            } else {
                for (auto& c : crew)
                    std::cout << c.id << " | " << c.last_name << " " << c.first_name
                              << " | Категория: " << c.category
                              << " | Опыт: " << c.experience_years << " лет\n";
            }
            break;
        }
        case 4: {
            int bus_id;
            std::cout << "Введите ID автобуса: ";
            std::cin >> bus_id;
            auto trips = db.getTripsByBus(bus_id);
            if (trips.empty()) {
                std::cout << "Поездки не найдены\n";
            } else {
                for (auto& t : trips)
                    std::cout << t.id << " | Маршрут: " << t.route_id
                              << " | От: " << t.departure_date
                              << " До: " << t.arrival_date
                              << " | Пассажиры: " << t.passengers_count
                              << " | Цена билета: " << t.ticket_price
                              << " | Общая стоимость: " << t.total_cost << "\n";
            }
            break;
        }
       case 5: {
            std::string tax, last, first, category, address;
            int bus_id, birth_year, experience_years;
            
            std::cout << "Налоговый номер: ";
            std::cin >> tax;
            std::cout << "Фамилия: ";
            std::cin >> last;
            std::cout << "Имя: ";
            std::cin >> first;
            std::cout << "Категория: ";
            std::cin >> category;
            std::cout << "ID автобуса: ";
            std::cin >> bus_id;
            std::cout << "Год рождения: ";
            std::cin >> birth_year;
            std::cout << "Опыт работы (лет): ";
            std::cin >> experience_years;
            std::cin.ignore(); 
            std::cout << "Адрес: ";
            std::getline(std::cin, address);
            
            db.addCrewMember(tax, last, first, category, bus_id, birth_year, experience_years, address);
            std::cout << "Член экипажа добавлен.\n";
            break;
        }
        case 6: {
            int bus_id, new_mileage;
            std::cout << "ID автобуса: ";
            std::cin >> bus_id;
            std::cout << "Новый пробег: ";
            std::cin >> new_mileage;
            db.updateBusMileage(bus_id, new_mileage);
            std::cout << "Пробег обновлен.\n";
            break;
        }
        case 7:
            running = false;
            break;
        default:
            std::cout << "Неверный выбор\n";
            break;
        }
        clearInput();
    }

    delete user;
    std::cout << "Выход...\n";
    return 0;
}