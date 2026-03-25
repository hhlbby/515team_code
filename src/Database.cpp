#include "Database.h"
#include <iostream>

static std::string safe(const unsigned char* txt) {
    return txt ? std::string(reinterpret_cast<const char*>(txt)) : "";
}

Database::Database(const std::string& filename) {
    openDatabase(filename);
}

Database::~Database() {
    if (db) sqlite3_close(db);
}

bool Database::openDatabase(const std::string& filename) {
    return sqlite3_open(filename.c_str(), &db) == SQLITE_OK;
}

bool Database::execute(const std::string& sql) {
    char* err = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err);
    if (err) sqlite3_free(err);
    return rc == SQLITE_OK;
}

User* Database::authenticateUser(const std::string& username,const std::string& password) {
    const char* sql = "SELECT user_id,username,password_hash,full_name,role FROM USERS WHERE username=? AND password_hash=?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return nullptr;

    sqlite3_bind_text(stmt,1,username.c_str(),-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,2,password.c_str(),-1,SQLITE_STATIC);

    User* user = nullptr;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user = new User(
            sqlite3_column_int(stmt,0),
            safe(sqlite3_column_text(stmt,1)),
            safe(sqlite3_column_text(stmt,2)),
            safe(sqlite3_column_text(stmt,3)),
            safe(sqlite3_column_text(stmt,4))
        );
    }

    sqlite3_finalize(stmt);
    return user;
}

std::vector<Bus> Database::getAllBuses() {
    std::vector<Bus> res;
    const char* sql = "SELECT bus_id,bus_number,model_name,total_mileage FROM BUSES";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return res;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        res.push_back(Bus(
            sqlite3_column_int(stmt,0),
            safe(sqlite3_column_text(stmt,1)),
            safe(sqlite3_column_text(stmt,2)),
            sqlite3_column_int(stmt,3)
        ));
    }

    sqlite3_finalize(stmt);
    return res;
}

std::vector<Route> Database::getAllRoutes() {
    std::vector<Route> res;
    const char* sql = "SELECT route_id,route_name,start_point,end_point,distance_km FROM ROUTES";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return res;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        res.push_back(Route(
            sqlite3_column_int(stmt,0),
            safe(sqlite3_column_text(stmt,1)),
            safe(sqlite3_column_text(stmt,2)),
            safe(sqlite3_column_text(stmt,3)),
            sqlite3_column_double(stmt,4)
        ));
    }

    sqlite3_finalize(stmt);
    return res;
}

std::vector<CrewMember> Database::getCrewByBus(int bus_id) {
    std::vector<CrewMember> res;
    const char* sql = "SELECT crew_id,tax_number,last_name,first_name,patronymic,birth_year,address,experience_years,category,bus_id,user_id FROM CREW_MEMBERS WHERE bus_id=?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return res;

    sqlite3_bind_int(stmt,1,bus_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        res.push_back(CrewMember(
            sqlite3_column_int(stmt,0),
            safe(sqlite3_column_text(stmt,1)),
            safe(sqlite3_column_text(stmt,2)),
            safe(sqlite3_column_text(stmt,3)),
            safe(sqlite3_column_text(stmt,4)),
            sqlite3_column_int(stmt,5),
            safe(sqlite3_column_text(stmt,6)),
            sqlite3_column_int(stmt,7),
            safe(sqlite3_column_text(stmt,8)),
            sqlite3_column_int(stmt,9),
            sqlite3_column_int(stmt,10)
        ));
    }

    sqlite3_finalize(stmt);
    return res;
}

std::vector<Trip> Database::getTripsByBus(int bus_id) {
    std::vector<Trip> res;
    const char* sql = "SELECT trip_id,bus_id,route_id,departure_date,arrival_date,passengers_count,ticket_price,total_cost FROM TRIPS WHERE bus_id=?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) return res;

    sqlite3_bind_int(stmt,1,bus_id);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        res.push_back(Trip(
            sqlite3_column_int(stmt,0),
            sqlite3_column_int(stmt,1),
            sqlite3_column_int(stmt,2),
            safe(sqlite3_column_text(stmt,3)),
            safe(sqlite3_column_text(stmt,4)),
            sqlite3_column_int(stmt,5),
            sqlite3_column_double(stmt,6),
            sqlite3_column_double(stmt,7)
        ));
    }

    sqlite3_finalize(stmt);
    return res;
}

void Database::addCrewMember(
    const std::string& tax,
    const std::string& last,
    const std::string& first,
    const std::string& category,
    int bus_id,
    int birth_year,
    int experience,
    const std::string& address
) {
    std::string sql =
        "INSERT INTO CREW_MEMBERS(tax_number,last_name,first_name,category,bus_id,birth_year,experience_years,address) VALUES('" +
        tax + "','" + last + "','" + first + "','" + category + "'," +
        std::to_string(bus_id) + "," +
        std::to_string(birth_year) + "," +
        std::to_string(experience) + ",'" +
        address + "')";
    execute(sql);
}

void Database::updateBusMileage(int bus_id, int mileage) {
    std::string sql = "UPDATE BUSES SET total_mileage=" + std::to_string(mileage) +
                      " WHERE bus_id=" + std::to_string(bus_id);
    execute(sql);
}
std::vector<Trip> Database::tripsByBusPeriod(int bus_id,const std::string& from,const std::string& to){
    std::vector<Trip> res;
    const char* sql="SELECT trip_id,bus_id,route_id,departure_date,arrival_date,passengers_count,ticket_price,total_cost FROM TRIPS WHERE bus_id=? AND departure_date BETWEEN ? AND ?";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK) return res;

    sqlite3_bind_int(stmt,1,bus_id);
    sqlite3_bind_text(stmt,2,from.c_str(),-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,3,to.c_str(),-1,SQLITE_STATIC);

    while(sqlite3_step(stmt)==SQLITE_ROW){
        res.push_back(Trip(
            sqlite3_column_int(stmt,0),
            sqlite3_column_int(stmt,1),
            sqlite3_column_int(stmt,2),
            safe(sqlite3_column_text(stmt,3)),
            safe(sqlite3_column_text(stmt,4)),
            sqlite3_column_int(stmt,5),
            sqlite3_column_double(stmt,6),
            sqlite3_column_double(stmt,7)
        ));
    }

    sqlite3_finalize(stmt);
    return res;
}
std::tuple<int,int,double> Database::statsByBus(int bus_id){
    const char* sql="SELECT COUNT(*),SUM(passengers_count),SUM(total_cost) FROM TRIPS WHERE bus_id=?";
    sqlite3_stmt* stmt;

    int trips=0, passengers=0;
    double money=0;

    if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK) return {0,0,0};

    sqlite3_bind_int(stmt,1,bus_id);

    if(sqlite3_step(stmt)==SQLITE_ROW){
        trips=sqlite3_column_int(stmt,0);
        passengers=sqlite3_column_int(stmt,1);
        money=sqlite3_column_double(stmt,2);
    }

    sqlite3_finalize(stmt);
    return {trips,passengers,money};
}
std::vector<std::pair<int,double>> Database::crewMoneyPeriod(const std::string& from,const std::string& to,double percent){
    std::vector<std::pair<int,double>> res;

    const char* sql=
    "SELECT crew_id,SUM(total_cost)*? FROM TRIPS "
    "JOIN CREW_MEMBERS USING(bus_id) "
    "WHERE departure_date BETWEEN ? AND ? "
    "GROUP BY crew_id";

    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK) return res;

    sqlite3_bind_double(stmt,1,percent);
    sqlite3_bind_text(stmt,2,from.c_str(),-1,SQLITE_STATIC);
    sqlite3_bind_text(stmt,3,to.c_str(),-1,SQLITE_STATIC);

    while(sqlite3_step(stmt)==SQLITE_ROW){
        res.push_back({
            sqlite3_column_int(stmt,0),
            sqlite3_column_double(stmt,1)
        });
    }

    sqlite3_finalize(stmt);
    return res;
}
std::vector<std::tuple<int,int,double>> Database::mostExpensiveRoute(){
    std::vector<std::tuple<int,int,double>> res;

    const char* sql=
    "SELECT bus_id,crew_id,ticket_price FROM TRIPS "
    "JOIN CREW_MEMBERS USING(bus_id) "
    "WHERE ticket_price=(SELECT MAX(ticket_price) FROM TRIPS)";

    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK) return res;

    while(sqlite3_step(stmt)==SQLITE_ROW){
        res.push_back({
            sqlite3_column_int(stmt,0),
            sqlite3_column_int(stmt,1),
            sqlite3_column_double(stmt,2)
        });
    }

    sqlite3_finalize(stmt);
    return res;
}
std::tuple<int,int,int> Database::topBusStats(){
    const char* sql=
    "SELECT bus_id,total_mileage,(SELECT SUM(passengers_count) FROM TRIPS WHERE bus_id=BUSES.bus_id) "
    "FROM BUSES ORDER BY total_mileage DESC LIMIT 1";

    sqlite3_stmt* stmt;

    int id=0,mileage=0,passengers=0;

    if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK) return {0,0,0};

    if(sqlite3_step(stmt)==SQLITE_ROW){
        id=sqlite3_column_int(stmt,0);
        mileage=sqlite3_column_int(stmt,1);
        passengers=sqlite3_column_int(stmt,2);
    }

    sqlite3_finalize(stmt);
    return {id,mileage,passengers};
}
double Database::crewMoneyOnDate(int crew_id,const std::string& date,double percent){
    const char* sql=
    "SELECT SUM(total_cost)*? FROM TRIPS "
    "JOIN CREW_MEMBERS USING(bus_id) "
    "WHERE crew_id=? AND departure_date=?";

    sqlite3_stmt* stmt;

    double money=0;

    if(sqlite3_prepare_v2(db,sql,-1,&stmt,0)!=SQLITE_OK) return 0;

    sqlite3_bind_double(stmt,1,percent);
    sqlite3_bind_int(stmt,2,crew_id);
    sqlite3_bind_text(stmt,3,date.c_str(),-1,SQLITE_STATIC);

    if(sqlite3_step(stmt)==SQLITE_ROW){
        money=sqlite3_column_double(stmt,0);
    }

    sqlite3_finalize(stmt);
    return money;
}
void Database::deleteCrewMember(int id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM CREW_MEMBERS WHERE id = ?;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Ошибка удаления члена экипажа\n";
    } else {
        std::cout << "Член экипажа удалён\n";
    }

    sqlite3_finalize(stmt);
}