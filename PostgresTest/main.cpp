#include <cppdb/errors.h>
#include <cppdb/frontend.h>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

namespace robot_db {

static constexpr bool ALLOW_UPDATE = true;

typedef struct WiFiConfig {
  std::string ssid;
  std::string password;
  std::tm created;
  std::tm updated;

  void show() {
    std::cout << "\tSSID: " << this->ssid << ", PWD: " << this->password
              << ",\n\tCREATED: " << asctime(&this->created)
              << "\tUPDATED: " << asctime(&this->updated) << "";
  }
} WiFiConfig_t;

} // namespace robot_db

using namespace robot_db;

template <typename DataT> class SQLSquema {
public:
  SQLSquema() {}
  ~SQLSquema() {}

  typedef struct ColumnInfo {
    bool insert;
    bool update;
    bool isPrimary;
  } ColumnInfo_t;

  virtual const std::string table() = 0;
  virtual const std::string uniqueKey() = 0;

  DataT getData() { return data; }
  void setData(DataT &_data) { data = _data; }

private:
  std::vector<std::string> columns;
  std::vector<ColumnInfo_t> info;
  DataT data;
};

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

  bool addWiFiConfig(WiFiConfig_t const &_config, bool _allowUpdate = false) {
    if (exists("wifi_profiles", "ssid", _config.ssid)) {
      if (_allowUpdate) {
        std::cout << "WiFi profile already exists, updating data in DB\n";
        prepare("UPDATE wifi_profiles SET ssid=?, password=?, updated_at=? "
                "WHERE ssid=?");
        return run(_config.ssid, _config.password, _config.updated,
                   _config.ssid);
      } else {
        std::cerr << "Error! WiFi profile already exists.\n";
        return false;
      }
    } else {
      prepare("INSERT INTO wifi_profiles(ssid, password, created_at, "
              "updated_at) VALUES(?,?,?,?)");

      return run(_config.ssid, _config.password, _config.created,
                 _config.updated);
    }
  }

  bool removeWiFiConfig(std::string const &_ssid) {
    if (not exists("wifi_profiles", "ssid", _ssid)) {
      std::cerr << "WiFi profile not found!\n";
      return false;
    } else {
      std::cout << "Found WiFi profile for SSID=" << _ssid
                << "! Deleting from DB.\n";
      prepare("DELETE FROM wifi_profiles WHERE ssid=?");
      return run(_ssid);
    }
  }

  WiFiConfig_t getWiFiConfig(std::string const &_ssid) {
    WiFiConfig_t cfg;
    if (not exists("wifi_profiles", "ssid", _ssid)) {
      std::cerr << "WiFi profile not found!\n";
      cfg.ssid = "";
      cfg.password = "";
    } else {
      std::cout << "WiFi " << _ssid << " Found!\n";
      prepare("SELECT * FROM wifi_profiles WHERE ssid=?");
      cppdb::result res = runQuery(_ssid);

      if (res.next()) {
        cfg.ssid = res.get<std::string>("ssid");
        cfg.password = res.get<std::string>("password");
        cfg.created = res.get<std::tm>("created_at");
        cfg.updated = res.get<std::tm>("updated_at");
      }
    }

    return cfg;
  }

  std::vector<WiFiConfig_t> getAllWiFiConfigs() {
    std::vector<WiFiConfig_t> array;
    WiFiConfig_t cfg;
    cppdb::result res = get("SELECT * FROM wifi_profiles");

    while (res.next()) {
      cfg.ssid = res.get<std::string>("ssid");
      cfg.password = res.get<std::string>("password");
      cfg.created = res.get<std::tm>("created_at");
      cfg.updated = res.get<std::tm>("updated_at");

      std::cout << "Found:";
      cfg.show();
      array.push_back(cfg);
    }
    return array;
  }

private:
  std::unique_ptr<cppdb::session> currentSession;
  cppdb::statement stat;

  bool exists(std::string const &_table, std::string const &_key,
              std::string const &_value) {
    prepare("SELECT 1 FROM " + _table + " WHERE " + _key + "=?");
    cppdb::result res = runQuery(_value);

    try {
      if (res.next()) {
        return true;
      }
    } catch (cppdb::cppdb_error const &e) {
      std::cerr << "Failed to check if " << _value << " exists in table("
                << _table << ") at column(" << _key << ")\n";
      std::cerr << "Returning true for safety\n";
      return true;
    }
    return false;
  }

  void prepare(std::string const &_statement) {
    stat = currentSession->create_statement(_statement);
  }

  cppdb::result get(std::string const &_query) {
    return *currentSession << _query.c_str();
  }

  template <typename T, typename... Args>
  cppdb::result runQuery(T first, Args... args) {
    try {
      stat.bind(first);
    } catch (cppdb::cppdb_error const &e) {
      std::cerr << "Statement Bind ERROR: " << e.what() << std::endl;
      std::cerr << "Make sure to run prepare() before runQuery()!\n";
      return cppdb::result();
    }
    return runQuery(args...);
  }

  cppdb::result runQuery() {
    try {
      cppdb::result res = stat.query();
      // std::cout << "ID: " << stat.last_insert_id() << "\n";
      // std::cout << "Affected rows: " << stat.affected() << "\n";

      stat.reset();
      return res;

    } catch (cppdb::cppdb_error const &e) {
      std::cerr << "Statement Query ERROR: " << e.what() << std::endl;
      return cppdb::result();
    }
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

  bool run() {
    try {
      stat.exec();
      // std::cout << "ID: " << stat.last_insert_id() << "\n";
      // std::cout << "Affected rows: " << stat.affected() << "\n";

      stat.reset();
      return true;

    } catch (cppdb::cppdb_error const &e) {
      std::cerr << "Statement Run ERROR: " << e.what() << std::endl;
      stat.reset();
      return false;
    }

    return true;
  }
};

int main(int argc, char *argv[]) {

  RobotSQLManager dbManager;
  bool ok = dbManager.init("setup_management", "postgres", "postgres");
  std::cout << "Manager Init OK=" << ok << "\n";

  if (ok) {
    WiFiConfig_t cfg;
    cfg.ssid = "SOCIALDROIDS";
    cfg.password = "Slinky";
    std::time_t now = std::time(0);
    cfg.created = *std::localtime(&now);
    cfg.updated = cfg.created;
    // dbManager.addWiFiConfig(cfg, ALLOW_UPDATE);

    // dbManager.removeWiFiConfig("SOCIALDROIDS");

    cfg = dbManager.getWiFiConfig("SOCIALDROIDS");
    if (cfg.ssid != "") {
      cfg.show();
    } else {
      std::cerr << "Failed to get WiFi Config\n";
    }

    dbManager.getAllWiFiConfigs();
  }

  std::cout << "OK!\n";
  return 0;
}
