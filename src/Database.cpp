#include "Database.h"
#include <iostream>

Database::Database(const std::string& filename) {
    if (!openDatabase(filename)) {
        std::cerr << "Cannot open database!" << std::endl;
        exit(1);
    }
}

Database::~Database() {
    sqlite3_close(db);
}

bool Database::openDatabase(const std::string& filename) {
    return sqlite3_open(filename.c_str(), &db) == SQLITE_OK;
}

std::vector<User> Database::getAllUsers() {
    std::vector<User> users;
    const char* sql = "SELECT user_id, username, password_hash, full_name, role FROM USERS;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to fetch users: " << sqlite3_errmsg(db) << std::endl;
        return users;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string full_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        users.push_back(User(id, username, password, full_name, role));
    }

    sqlite3_finalize(stmt);
    return users;
}

std::vector<Bus> Database::getAllBuses() {
    std::vector<Bus> buses;
    const char* sql = "SELECT bus_id, bus_number, model_name, total_mileage FROM BUSES;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to fetch buses: " << sqlite3_errmsg(db) << std::endl;
        return buses;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string number = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string model = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        int mileage = sqlite3_column_int(stmt, 3);
        buses.push_back(Bus(id, number, model, mileage));
    }

    sqlite3_finalize(stmt);
    return buses;
}

User* Database::authenticateUser(const std::string& username, const std::string& password) {
    const char* sql = "SELECT user_id, username, password_hash, full_name, role FROM USERS WHERE username = ? AND password_hash = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
        return nullptr;
    }
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    User* user = nullptr;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        std::string uname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string pwd = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string full_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        std::string role = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        user = new User(id, uname, pwd, full_name, role);
    }

    sqlite3_finalize(stmt);
    return user;
}