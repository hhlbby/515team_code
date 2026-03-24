#include "Database.h"
#include <iostream>

Database::Database(const std::string& filename) {
    if (!openDatabase(filename)) exit(1);
}

Database::~Database() { sqlite3_close(db); }

bool Database::openDatabase(const std::string& filename) {
    return sqlite3_open(filename.c_str(), &db) == SQLITE_OK;
}

bool Database::execute(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if(rc != SQLITE_OK) { if(errMsg) sqlite3_free(errMsg); return false; }
    return true;
}

std::vector<User> Database::getAllUsers() {
    std::vector<User> users;
    const char* sql = "SELECT user_id,username,password_hash,full_name,role FROM USERS;";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return users;
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        users.push_back(User(
            sqlite3_column_int(stmt,0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,2)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,3)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,4))
        ));
    }
    sqlite3_finalize(stmt);
    return users;
}

User* Database::authenticateUser(const std::string& username,const std::string& password) {
    const char* sql="SELECT user_id,username,password_hash,full_name,role FROM USERS WHERE username=? AND password_hash=?;";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK) return nullptr;
    sqlite3_bind_text(stmt,1,username.c_str(),-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,2,password.c_str(),-1,SQLITE_STATIC);
    User* user = nullptr;
    if(sqlite3_step(stmt)==SQLITE_ROW) {
        user = new User(
            sqlite3_column_int(stmt,0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,2)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,3)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,4))
        );
    }
    sqlite3_finalize(stmt);
    return user;
}

std::vector<Bus> Database::getAllBuses() {
    std::vector<Bus> buses;
    const char* sql="SELECT bus_id,bus_number,model_name,total_mileage FROM BUSES;";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK) return buses;
    while(sqlite3_step(stmt)==SQLITE_ROW){
        buses.push_back(Bus(
            sqlite3_column_int(stmt,0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,2)),
            sqlite3_column_int(stmt,3)
        ));
    }
    sqlite3_finalize(stmt);
    return buses;
}

std::vector<CrewMember> Database::getCrewByBus(int bus_id){
    std::vector<CrewMember> crew;
    const char* sql="SELECT crew_id,tax_number,last_name,first_name,patronymic,birth_year,address,experience_years,category,bus_id,user_id FROM CREW_MEMBERS WHERE bus_id=?;";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK) return crew;
    sqlite3_bind_int(stmt,1,bus_id);
    while(sqlite3_step(stmt)==SQLITE_ROW){
        crew.push_back(CrewMember(
            sqlite3_column_int(stmt,0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,2)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,3)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,4)),
            sqlite3_column_int(stmt,5),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,6)),
            sqlite3_column_int(stmt,7),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,8)),
            sqlite3_column_int(stmt,9),
            sqlite3_column_int(stmt,10)
        ));
    }
    sqlite3_finalize(stmt);
    return crew;
}

std::vector<Route> Database::getAllRoutes(){
    std::vector<Route> routes;
    const char* sql="SELECT route_id,route_name,start_point,end_point,distance_km FROM ROUTES;";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK) return routes;
    while(sqlite3_step(stmt)==SQLITE_ROW){
        routes.push_back(Route(
            sqlite3_column_int(stmt,0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,1)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,2)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,3)),
            sqlite3_column_double(stmt,4)
        ));
    }
    sqlite3_finalize(stmt);
    return routes;
}

std::vector<Trip> Database::getTripsByBus(int bus_id){
    std::vector<Trip> trips;
    const char* sql="SELECT trip_id,bus_id,route_id,departure_date,arrival_date,passengers_count,ticket_price,total_cost FROM TRIPS WHERE bus_id=?;";
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK) return trips;
    sqlite3_bind_int(stmt,1,bus_id);
    while(sqlite3_step(stmt)==SQLITE_ROW){
        trips.push_back(Trip(
            sqlite3_column_int(stmt,0),
            sqlite3_column_int(stmt,1),
            sqlite3_column_int(stmt,2),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,3)),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt,4)),
            sqlite3_column_int(stmt,5),
            sqlite3_column_double(stmt,6),
            sqlite3_column_double(stmt,7)
        ));
    }
    sqlite3_finalize(stmt);
    return trips;
}

void Database::addCrewMember(
    const std::string& tax_number,
    const std::string& last_name,
    const std::string& first_name,
    const std::string& category,
    int bus_id,
    int birth_year,
    int experience_years,
    const std::string& address)
{
    if (!db) return;
    std::string sql = "INSERT INTO CREW_MEMBERS(tax_number,last_name,first_name,category,bus_id,birth_year,experience_years,address) VALUES('"
                      + tax_number + "','" + last_name + "','" + first_name + "','" + category + "',"
                      + std::to_string(bus_id) + "," + std::to_string(birth_year) + ","
                      + std::to_string(experience_years) + ",'" + address + "');";
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        if (errMsg) { std::cerr << errMsg << "\n"; sqlite3_free(errMsg); }
    }
}

void Database::updateBusMileage(int bus_id,int new_mileage) {
    if(!db) return;
    std::string sql="UPDATE BUSES SET total_mileage="+std::to_string(new_mileage)+" WHERE bus_id="+std::to_string(bus_id)+";";
    char* errMsg=nullptr;
    sqlite3_exec(db,sql.c_str(),nullptr,nullptr,&errMsg);
    if(errMsg){ std::cerr<<errMsg<<"\n"; sqlite3_free(errMsg);}
}