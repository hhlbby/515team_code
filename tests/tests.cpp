#include <gtest/gtest.h>
#include "../includes/Database.h"
#include <filesystem>

const std::string TEST_DB_PATH = "test_tourist_bureau.db";

class ProjectTest : public ::testing::Test {
protected:
    Database* db;

    void SetUp() override {
        if (std::filesystem::exists(TEST_DB_PATH)) {
            std::filesystem::remove(TEST_DB_PATH);
        }
        db = new Database(TEST_DB_PATH);
        initTestData();
    }

    void TearDown() override {
        delete db;
        if (std::filesystem::exists(TEST_DB_PATH)) {
            std::filesystem::remove(TEST_DB_PATH);
        }
    }

    void initTestData() {
        db->execute(
            "CREATE TABLE USERS("
            "user_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "username TEXT UNIQUE,"
            "password_hash TEXT,"
            "full_name TEXT,"
            "role TEXT);"
        );

        db->execute(
            "CREATE TABLE BUSES("
            "bus_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "bus_number TEXT UNIQUE,"
            "model_name TEXT,"
            "total_mileage INTEGER);"
        );

        db->execute(
            "CREATE TABLE CREW_MEMBERS("
            "crew_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "tax_number TEXT UNIQUE,"
            "last_name TEXT,"
            "first_name TEXT,"
            "patronymic TEXT,"
            "birth_year INTEGER,"
            "address TEXT,"
            "experience_years INTEGER,"
            "category TEXT,"
            "bus_id INTEGER,"
            "user_id INTEGER);"
        );

        db->execute(
            "CREATE TABLE ROUTES("
            "route_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "route_name TEXT,"
            "start_point TEXT,"
            "end_point TEXT,"
            "distance_km REAL);"
        );

        db->execute(
            "CREATE TABLE TRIPS("
            "trip_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "bus_id INTEGER,"
            "route_id INTEGER,"
            "departure_date TEXT,"
            "arrival_date TEXT,"
            "passengers_count INTEGER,"
            "ticket_price REAL,"
            "total_cost REAL GENERATED ALWAYS AS (passengers_count * ticket_price) STORED);"
        );

        db->execute("INSERT INTO USERS(username, password_hash, full_name, role) VALUES('ivanov_driver','hash_ivanov_123','Иванов Иван Иванович','driver');");
        db->execute("INSERT INTO USERS(username, password_hash, full_name, role) VALUES('petrov_guide','hash_petrov_456','Петров Петр Петрович','guide');");

        db->execute("INSERT INTO BUSES(bus_number, model_name, total_mileage) VALUES('A001','Mercedes',1000);");
        db->execute("INSERT INTO BUSES(bus_number, model_name, total_mileage) VALUES('A002','Volvo',500);");

        db->execute(
            "INSERT INTO CREW_MEMBERS(tax_number,last_name,first_name,patronymic,birth_year,address,experience_years,category,bus_id,user_id) "
            "VALUES('T001','Ivanov','Ivan','Ivanovich',1980,'CityA',5,'driver',1,NULL);"
        );
        db->execute(
            "INSERT INTO CREW_MEMBERS(tax_number,last_name,first_name,patronymic,birth_year,address,experience_years,category,bus_id,user_id) "
            "VALUES('T002','Petrov','Petr','Petrovich',1982,'CityB',3,'guide',2,NULL);"
        );

        db->execute("INSERT INTO ROUTES(route_name,start_point,end_point,distance_km) VALUES('Route1','CityA','CityB',120.5);");
        db->execute("INSERT INTO ROUTES(route_name,start_point,end_point,distance_km) VALUES('Route2','CityC','CityD',200.0);");

        db->execute("INSERT INTO TRIPS(bus_id,route_id,departure_date,arrival_date,passengers_count,ticket_price) VALUES(1,1,'2026-03-23','2026-03-23',10,50);");
        db->execute("INSERT INTO TRIPS(bus_id,route_id,departure_date,arrival_date,passengers_count,ticket_price) VALUES(2,2,'2026-03-24','2026-03-24',20,75);");
    }
};

TEST_F(ProjectTest, DBConnectionWorks) {
    auto buses = db->getAllBuses();
    EXPECT_GE(buses.size(), 2);
}

TEST_F(ProjectTest, TripsByBus) {
    auto trips = db->getTripsByBus(1);
    EXPECT_EQ(trips.size(), 1);
    EXPECT_DOUBLE_EQ(trips[0].total_cost, 500.0);
}

TEST_F(ProjectTest, RoutesNotEmpty) {
    auto routes = db->getAllRoutes();
    EXPECT_GE(routes.size(), 2);
}

TEST_F(ProjectTest, AuthenticateValidUser) {
    User* user = db->authenticateUser("ivanov_driver", "hash_ivanov_123");
    ASSERT_NE(user, nullptr);
    EXPECT_EQ(user->role, "driver");
    delete user;
}

TEST_F(ProjectTest, AuthenticateInvalidUser) {
    User* user = db->authenticateUser("nonexistent", "123");
    EXPECT_EQ(user, nullptr);
}

TEST_F(ProjectTest, CheckUserRole) {
    User* user = db->authenticateUser("petrov_guide", "hash_petrov_456");
    ASSERT_NE(user, nullptr);
    EXPECT_EQ(user->role, "guide");
    delete user;
}

TEST_F(ProjectTest, GetAllBuses) {
    auto buses = db->getAllBuses();
    EXPECT_EQ(buses[0].bus_number, "A001");
    EXPECT_EQ(buses[1].bus_number, "A002");
}

TEST_F(ProjectTest, BusMileageUpdate) {
    auto buses = db->getAllBuses();
    int new_mileage = buses[0].total_mileage + 100;
    db->execute("UPDATE BUSES SET total_mileage=" + std::to_string(new_mileage) + " WHERE bus_id=1;");
    buses = db->getAllBuses();
    EXPECT_EQ(buses[0].total_mileage, new_mileage);
}

TEST_F(ProjectTest, BusCount) {
    auto buses = db->getAllBuses();
    EXPECT_EQ(buses.size(), 2);
}

TEST_F(ProjectTest, GetCrewByBus) {
    auto crew = db->getCrewByBus(1);
    EXPECT_EQ(crew.size(), 1);
    EXPECT_EQ(crew[0].last_name, "Ivanov");
}

TEST_F(ProjectTest, AddCrewMember) {
    db->execute(
        "INSERT INTO CREW_MEMBERS(tax_number,last_name,first_name,patronymic,birth_year,address,experience_years,category,bus_id,user_id) "
        "VALUES('T003','Sidorov','Sidor','Sidorovich',1990,'CityC',2,'driver',1,NULL);"
    );
    auto crew = db->getCrewByBus(1);
    EXPECT_EQ(crew.size(), 2);
}

TEST_F(ProjectTest, CrewMoneyCalculation) {
    auto trips = db->getTripsByBus(1);
    double total = trips[0].total_cost * 0.2;
    EXPECT_DOUBLE_EQ(total, 100.0);
}

TEST_F(ProjectTest, TripTotalCost) {
    auto trips = db->getTripsByBus(2);
    EXPECT_DOUBLE_EQ(trips[0].total_cost, 1500.0);
}

TEST_F(ProjectTest, TripPassengersCount) {
    auto trips = db->getTripsByBus(2);
    EXPECT_EQ(trips[0].passengers_count, 20);
}

TEST_F(ProjectTest, TripFilterByDate) {
    auto trips = db->getTripsByBus(1);
    EXPECT_EQ(trips[0].departure_date, "2026-03-23");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
