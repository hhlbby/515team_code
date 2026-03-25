#include <iostream>
#include <tuple>
#include "Database.h"

int main() {
    Database db("../db/tourist_bureau.db");

    std::string login, password;

    std::cout << "Логин: ";
    std::cin >> login;

    std::cout << "Пароль: ";
    std::cin >> password;

    User* user = db.authenticateUser(login, password);

    if (!user) {
        std::cout << "Ошибка авторизации\n";
        return 1;
    }

    std::cout << "Привет, " << user->full_name << " (" << user->role << ")\n";

    int choice;

    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Рейсы автобуса за период\n";
        std::cout << "2. Статистика автобуса\n";
        std::cout << "3. Деньги экипажей за период\n";
        std::cout << "4. Самый дорогой маршрут\n";
        std::cout << "5. Лучший автобус\n";
        std::cout << "6. Деньги экипажа на дату\n";
        std::cout << "7. Добавить члена экипажа\n";
        std::cout << "8. Обновить пробег автобуса\n";
        std::cout << "9. Удалить члена экипажа\n";
        std::cout << "0. Выход\n";
        std::cout << "Выбор: ";

        std::cin >> choice;

        if (choice == 1) {
            int id;
            std::string from, to;

            std::cout << "ID автобуса: ";
            std::cin >> id;
            std::cout << "Дата от: ";
            std::cin >> from;
            std::cout << "Дата до: ";
            std::cin >> to;

            auto trips = db.tripsByBusPeriod(id, from, to);

            for (auto& t : trips) {
                std::cout << "ID: " << t.id
                          << " Пассажиры: " << t.passengers_count
                          << " Цена: " << t.ticket_price
                          << " Общая сумма: " << t.total_cost
                          << "\n";
            }
        }

        else if (choice == 2) {
            int id;
            std::cout << "ID автобуса: ";
            std::cin >> id;

            auto stats = db.statsByBus(id);

            std::cout << "Поездки: " << std::get<0>(stats)
                      << " Пассажиры: " << std::get<1>(stats)
                      << " Деньги: " << std::get<2>(stats)
                      << "\n";
        }

        else if (choice == 3) {
            std::string from, to;
            double percent;

            std::cout << "Дата от: ";
            std::cin >> from;
            std::cout << "Дата до: ";
            std::cin >> to;
            std::cout << "Процент: ";
            std::cin >> percent;

            auto res = db.crewMoneyPeriod(from, to, percent);

            for (auto& x : res) {
                std::cout << "Экипаж ID: " << x.first
                          << " Деньги: " << x.second << "\n";
            }
        }

        else if (choice == 4) {
            auto res = db.mostExpensiveRoute();

            for (auto& x : res) {
                std::cout << "Автобус: " << std::get<0>(x)
                          << " Экипаж: " << std::get<1>(x)
                          << " Цена билета: " << std::get<2>(x)
                          << "\n";
            }
        }

        else if (choice == 5) {
            auto res = db.topBusStats();

            std::cout << "Автобус: " << std::get<0>(res)
                      << " Пассажиры: " << std::get<1>(res)
                      << " Пробег: " << std::get<2>(res)
                      << "\n";
        }

        else if (choice == 6) {
            int id;
            std::string date;
            double percent;

            std::cout << "ID экипажа: ";
            std::cin >> id;
            std::cout << "Дата: ";
            std::cin >> date;
            std::cout << "Процент: ";
            std::cin >> percent;

            double money = db.crewMoneyOnDate(id, date, percent);

            std::cout << "Начислено: " << money << "\n";
        }

        else if (choice == 7) {
            std::string tax, last, first, category, address;
            int bus_id, birth_year, exp;

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
            std::cout << "Опыт: ";
            std::cin >> exp;
            std::cout << "Адрес: ";
            std::cin.ignore();
            std::getline(std::cin, address);

            db.addCrewMember(tax, last, first, category, bus_id, birth_year, exp, address);

            std::cout << "Добавлено\n";
        }

        else if (choice == 8) {
            int id, mileage;

            std::cout << "ID автобуса: ";
            std::cin >> id;
            std::cout << "Новый пробег: ";
            std::cin >> mileage;

            db.updateBusMileage(id, mileage);

            std::cout << "Обновлено\n";
        }

        else if (choice == 9) {
            int id;
            std::cout << "ID члена экипажа: ";
            std::cin >> id;

            db.deleteCrewMember(id);

            std::cout << "Удалено\n";
        }

        else if (choice == 0) {
            break;
        }
    }

    delete user;
    return 0;
}