#include <any>
#include <cppdb/errors.h>
#include <cppdb/frontend.h>
#include <ctime>
#include <iostream>
#include <sstream>
#include <type_traits>
#include <variant>
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

using SQLDataTypes = std::variant<int, std::tm, std::string, bool, double>;

class SQLSchema {
public:
  SQLSchema(const std::string _table, const std::string _uniqueKey) {
    tableName = _table;
    uniqueKeyName = _uniqueKey;
  }
  ~SQLSchema() {}

  typedef struct Column {
    std::string name;
    bool insert;
    bool update;
    bool isPrimary;
    SQLDataTypes data;
  } Column_t;

  std::string table() { return tableName; };

  void addColumn(std::string const &_name, bool _insert, bool _update,
                 bool _isPrimary, SQLDataTypes _data) {

    Column_t infoData;
    infoData.name = _name;
    infoData.insert = _insert;
    infoData.update = _update;
    infoData.isPrimary = _isPrimary;
    infoData.data = _data;

    columns.push_back(infoData);
  }

  std::string uniqueKey() { return uniqueKeyName; };
  SQLDataTypes keyData() {
    size_t index = 0;
    for (Column_t const &n : columns) {
      if (n.isPrimary) {
        return columns.at(index).data;
      }
      index++;
    }
    return "";
  };

  std::string updateColumns() {
    std::string col = "";
    for (size_t n = 0; n < columns.size(); n++) {

      if (columns.at(n).update == true) {

        if (n < columns.size() - 1) {

          col += columns.at(n).name + "=?, ";
        } else {

          col += columns.at(n).name + "=? ";
        }
      }
    }
    return col;
  }

  std::string insertColumns() {
    std::string col = "(";
    for (size_t n = 0; n < columns.size(); n++) {

      if (columns.at(n).insert == true) {

        if (n < columns.size() - 1) {

          col += columns.at(n).name + ", ";
        } else {

          col += columns.at(n).name + ")";
        }
      }
    }
    return col;
  }

  std::string insertPlaceholders() {
    std::string col = "(";
    for (size_t n = 0; n < columns.size(); n++) {

      if (columns.at(n).insert == true) {

        if (n < columns.size() - 1) {

          col += "?,";
        } else {

          col += "?)";
        }
      }
    }
    return col;
  }

  std::vector<SQLDataTypes> updateColumnsData() {
    std::vector<SQLDataTypes> out;
    for (size_t n = 0; n < columns.size(); n++) {

      if (columns.at(n).update == true) {

        out.push_back(columns.at(n).data);
      }
    }
    return out;
  }

  std::vector<SQLDataTypes> insertColumnsData() {
    std::vector<SQLDataTypes> out;
    for (size_t n = 0; n < columns.size(); n++) {

      if (columns.at(n).insert == true) {

        out.push_back(columns.at(n).data);
      }
    }
    return out;
  }

  // std::vector<SQLDataTypes> getData() { return data; }
  void setData(std::vector<SQLDataTypes> &_data) {
    if (_data.size() != columns.size()) {
      return;
    }
    for (size_t n = 0; n < columns.size(); n++) {
      columns.at(n).data = _data.at(n);
    }
  }

private:
  std::string tableName;
  std::string uniqueKeyName;

  std::vector<Column_t> columns;
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

  bool addSchemaData(SQLSchema *_schema, bool _allowUpdate = false) {
    std::vector<SQLDataTypes> data;
    if (exists(_schema->table(), _schema->uniqueKey(), _schema->keyData())) {
      if (_allowUpdate) {
        std::cout << "Entry already exists, updating data in DB\n";
        prepare("UPDATE " + _schema->table() + " SET " +
                _schema->updateColumns() + "WHERE " + _schema->uniqueKey() +
                "=?");

        data = _schema->updateColumnsData();
      } else {
        std::cerr << "Entry already exists in table!\n";
        return false;
      }
    } else {
      prepare("INSERT INTO " + _schema->table() + _schema->insertColumns() +
              " VALUES" + _schema->insertPlaceholders());

      data = _schema->insertColumnsData();
    }

    for (SQLDataTypes const &_d : data) {
      if (!bindAny(_d)) {
        std::cerr << "Failed to bind value to statement!\n";
      }
    }
    bindAny(_schema->keyData());
    return run();
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
              SQLDataTypes const &_value) {
    prepare("SELECT 1 FROM " + _table + " WHERE " + _key + "=?");
    cppdb::result res = runQuery(_value);

    try {
      if (res.next()) {
        return true;
      }
    } catch (cppdb::cppdb_error const &e) {
      std::cerr << "Failed to check if key exists in table(" << _table
                << ") at column(" << _key << ")\n";
      std::cerr << "Returning true for safety\n";
      return true;
    }
    return false;
  }

  void prepare(std::string const &_statement) {
    // std::cout << "\t-- Preparing <" << _statement << ">\n";
    stat = currentSession->create_statement(_statement);
  }

  cppdb::result get(std::string const &_query) {
    return *currentSession << _query.c_str();
  }

  bool bindAny(SQLDataTypes const &_value) {
    try {
      std::visit([this](auto &&arg) { this->stat.bind(arg); }, _value);
      return true;
    } catch (const cppdb::cppdb_error &e) {
      std::cerr << "Bind error: " << e.what() << "\n";
      return false;
    }
  }

  template <typename T, typename... Args>
  cppdb::result runQuery(T first, Args... args) {
    bool ok = false;

    SQLDataTypes dummy = 0;
    if constexpr (std::is_same_v<decltype(first), decltype(dummy)>) {
      bindAny(first);
    }

    if (!ok) {
      try {
        const std::string dummy = "";
        if constexpr (std::is_same_v<decltype(first), decltype(dummy)>) {
          std::cout << "Binding value: " << first << "\n";
          stat.bind(first);
        }
        ok = true;
      } catch (cppdb::cppdb_error const &e) {
        std::cerr << "Statement Bind ERROR: " << e.what() << std::endl;
        std::cerr << "Make sure to run prepare() before runQuery()!\n";
        return cppdb::result();
      }
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

  template <typename... Args> bool run(Args &&...args) {
    try {
      // Fold expression para dar bind em todos os argumentos
      (stat.bind(std::forward<Args>(args)), ...);
      stat.exec();
      stat.reset();
      return true;
    } catch (cppdb::cppdb_error const &e) {
      std::cerr << "Statement Bind ERROR: " << e.what() << std::endl;
      std::cerr << "Make sure to run prepare() before run()!\n";
      return false;
    }
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

  SQLSchema schema("wifi_profiles", "ssid");
  schema.addColumn("ssid", true, true, true, "");
  schema.addColumn("password", true, true, false, "");
  schema.addColumn("created_at", true, false, false, std::tm());
  schema.addColumn("updated_at", true, true, false, std::tm());

  std::cout << "Schema: " << schema.table() << ", " << schema.uniqueKey()
            << ", UPC:" << schema.updateColumns()
            << ", PLH:" << schema.insertPlaceholders()
            << ", ICC:" << schema.insertColumns() << "\n";

  if (ok) {
    WiFiConfig_t cfg;
    cfg.ssid = "SOCIALDROIDS";
    cfg.password = "Slinky";
    std::time_t now = std::time(0);
    cfg.created = *std::localtime(&now);
    cfg.updated = *std::localtime(&now);
    dbManager.addWiFiConfig(cfg, ALLOW_UPDATE);

    std::vector<SQLDataTypes> data = {"SD", "senha123", *std::localtime(&now),
                                      *std::localtime(&now)};
    schema.setData(data);
    if (!dbManager.addSchemaData(&schema, ALLOW_UPDATE)) {
      std::cerr << "Failed to add data!\n";
    }

    // dbManager.getAllWiFiConfigs();
    // dbManager.removeWiFiConfig("Socialdroids");

    // cfg = dbManager.getWiFiConfig("SOCIALDROIDS");
    // if (cfg.ssid != "") {
    //   cfg.show();
    // } else {
    //   std::cerr << "Failed to get WiFi Config\n";
    // }

    dbManager.getAllWiFiConfigs();
  }

  std::cout << "OK!\n";
  return 0;
}
