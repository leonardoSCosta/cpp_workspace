#include <cppdb/errors.h>
#include <cppdb/frontend.h>
#include <ctime>
#include <iostream>
#include <sstream>

class RobotSQLManager {
public:
  RobotSQLManager() { currentSession = std::make_unique<cppdb::session>(); }
  ~RobotSQLManager() {}

  bool init(std::string _dbname, std::string _user, std::string _pwd,
            std::string _host = "localhost") {
    std::stringstream config;
    config << "postgresql:dbname=" << _dbname << ";user=" << _user
           << ";password=" << _pwd << ";host=" << _host;

    try {
      currentSession->open(config.str());
    } catch (cppdb::cppdb_error const &e) {
      std::cerr << "Connection Error: " << e.what() << std::endl;
    }

    return currentSession->is_open();
  }

  void prepare(std::string const &_statement) {
    stat = currentSession->create_statement(_statement);
  }

  template <typename T, typename... Args> bool run(T first, Args... args) {
    try {
      stat.bind(first);
    } catch (cppdb::cppdb_error const &e) {
      std::cerr << "Statement Bind ERROR: " << e.what() << std::endl;
      std::cerr << "Make sure to run prepare() before run()!\n";
      return false;
    }
    return run(args...);
  }

  cppdb::result get(std::string const &_query) {
    return *currentSession << _query.c_str();
  }

private:
  std::unique_ptr<cppdb::session> currentSession;
  cppdb::statement stat;

  bool run() {
    try {
      stat.exec();
      // std::cout << "ID: " << stat.last_insert_id() << "\n";
      std::cout << "Affected rows: " << stat.affected() << "\n";

      stat.reset();
      return true;

    } catch (cppdb::cppdb_error const &e) {
      std::cerr << "Statement Run ERROR: " << e.what() << std::endl;
      return false;
    }

    return true;
  }

};

int main(int argc, char *argv[]) {

  RobotSQLManager manager;
  bool ok = manager.init("setup_management", "postgres", "postgres");
  std::cout << "Manager Init OK=" << ok << "\n";

  if (ok) {
    std::time_t now = std::time(0);
    std::tm *timeinfo = std::localtime(&now);

    // manager.prepare("INSERT INTO wifi_profiles(ssid, password, created_at, "
    //                 "updated_at) VALUES(?,?,?,?)");
    // ok = manager.run("SOCIALDROIDS4", "Teste123", *timeinfo, *timeinfo);

    manager.prepare("UPDATE wifi_profiles SET ssid=? WHERE ssid=?");
    manager.run("SOCIALDROIDS4", "SOCIALDROIDS5");

    cppdb::result res = manager.get("SELECT * FROM wifi_profiles");

    std::string ssid, pwd;
    std::tm created, updated;

    while (res.next()) {
      res >> ssid >> pwd >> created >> updated;
      std::cout << "SSID: " << ssid << "\nPWD: " << pwd
                << "\nCreated At: " << asctime(&created)
                << "Updated At: " << asctime(&updated) << "\n";
      // std::cout << res.get<std::string>("tablename") << "\n";
    }
  }

  // cppdb::session sql(
  //     "postgresql:dbname=setup_management;user=postgres;password=postgres;"
  //     "host=localhost");
  //
  // cppdb::statement stat;
  // stat = sql
  //        << "INSERT INTO wifi_profiles(ssid, password, created_at,
  //        updated_at) "
  //           " VALUES(?,?,?,?)"
  //        << "SOCIALDROIDS" << "Teste123" << *timeinfo << *timeinfo;
  // try {
  //   stat.exec();
  // } catch (cppdb::cppdb_error const &e) {
  //   std::cerr << "ERROR: " << e.what() << std::endl;
  // }
  //
  // // cppdb::result res = sql << "SELECT tablename FROM pg_catalog.pg_tables";
  // cppdb::result res = sql << "SELECT * FROM wifi_profiles";
  //
  // std::string ssid, pwd;
  // std::tm created, updated;
  //
  // while (res.next()) {
  //   res >> ssid >> pwd >> created >> updated;
  //   std::cout << "SSID: " << ssid << "\nPWD: " << pwd
  //             << "\nCreated At: " << asctime(&created)
  //             << "Updated At: " << asctime(&updated) << "\n";
  //   // std::cout << res.get<std::string>("tablename") << "\n";
  // }

  std::cout << "OK!\n";
  return 0;
}
