#include <any>
#include <cppdb/errors.h>
#include <cppdb/frontend.h>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

// INFO:
// short -> bool
// Não existe fetch(bool arg) no cppdb
using SQLData = std::variant<int, std::tm, std::string, short, double,
                             long long, std::vector<uint8_t>>;
using SQLConditionMap = std::unordered_map<std::string, SQLData>;
enum class SQLDataType
{
    Int,      // int
    DateTime, // std::tm
    String,   // std::string
    Short,    // short
    Double,   // double
    BigInt,   // long long (int64_t)
    Blob,     // std::vector<uint8_t>
};

std::ostream& operator<<(std::ostream& os, const SQLData& v)
{
    std::visit(
        [&os](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::vector<uint8_t>>)
            {
                os << "[BLOB data: " << arg.size() << " bytes]";
            }
            else
            {
                os << arg;
            }
        },
        v);
    return os;
}

class SQLSchema
{
public:
    SQLSchema(const std::string _table, const std::string _uniqueKey)
        : tableName(_table), uniqueKeyName(_uniqueKey)
    {
    }

    ~SQLSchema()
    {
    }

    typedef struct Column
    {
        std::string name;
        bool insert;
        bool update;
        bool isPrimary;
        SQLDataType type;
    } Column_t;

    const std::string& table() const
    {
        return tableName;
    };

    void addColumn(std::string const& _name, SQLDataType _type, bool _insert,
                   bool _update, bool _isPrimary)
    {
        columns.push_back({_name, _insert, _update, _isPrimary, _type});
    }

    const std::string& uniqueKey() const
    {
        return uniqueKeyName;
    };

    const std::vector<Column_t>& getColumns() const
    {
        return columns;
    }

private:
    std::string tableName;
    std::string uniqueKeyName;

    std::vector<Column_t> columns;
};

class SQLEntry
{
public:
    typedef enum SQL_ENTRY_GET_TYPES
    {
        FULL = 0,
        UPDATE,
        INSERT
    } SQL_ENTRY_GET_TYPES_t;

    void setValue(const std::string& columnName, SQLData value)
    {
        data[columnName] = value;
    }

    SQLData getValue(const std::string& columnName) const
    {
        auto it = data.find(columnName);
        if (it != data.end())
        {
            return it->second;
        }
        throw std::invalid_argument("Undefined column: " + columnName);
    }

    bool hasColumn(const std::string& columnName) const
    {
        return data.find(columnName) != data.end();
    }

private:
    std::unordered_map<std::string, SQLData> data;
};

class SQLManager
{
public:
    SQLManager()
    {
        currentSession = std::make_unique<cppdb::session>();
    }
    ~SQLManager()
    {
    }

    bool init(std::string _dbname, std::string _user, std::string _pwd,
              std::string _host = "localhost")
    {
        std::stringstream config;
        config << "postgresql:dbname=" << _dbname << ";user=" << _user
               << ";password=" << _pwd << ";host=" << _host;

        try
        {
            currentSession->open(config.str());
        }
        catch (cppdb::cppdb_error const& e)
        {
            std::cerr << "Connection Error: " << e.what() << std::endl;
        }

        return currentSession->is_open();
    }

    bool insert(SQLSchema* _schema, SQLEntry* _entry)
    {
        std::string columnsStr = "";
        std::string valuesStr = "";
        std::vector<SQLData> bindValues;

        bool first = true;
        for (const auto& col : _schema->getColumns())
        {
            // Insere apenas se o schema permite e se o usuário forneceu o dado
            if (col.insert && _entry->hasColumn(col.name))
            {
                if (!first)
                {
                    columnsStr += ", ";
                    valuesStr += ", ";
                }
                columnsStr += col.name;
                valuesStr += "?";
                bindValues.push_back(_entry->getValue(col.name));
                first = false;
            }
        }

        if (columnsStr.empty())
        {
            std::cerr << "Insert aborted: No valid columns to insert.\n";
            return false;
        }

        std::string statement = "INSERT INTO " + _schema->table() + " (" +
                                columnsStr + ") VALUES (" + valuesStr + ")";
        prepare(statement);

        for (const auto& val : bindValues)
        {
            if (!bindAny(val))
            {
                std::cerr << "Failed to bind value to insert statement!\n";
                return false;
            }
        }

        int affected = run();
        if (affected == 0)
        {
            std::cerr << "Update skipped: Entry not found or conditions didn't "
                         "match.\n";
            return false;
        }

        return affected > 0;
    }

    bool update(SQLSchema* _schema, SQLEntry* _entry,
                const std::unordered_map<std::string, SQLData>& conditions)
    {
        if (conditions.empty())
        {
            std::cerr << "Update aborted: conditions map is empty (prevents "
                         "accidental full table update).\n";
            return false;
        }

        std::string statement = "UPDATE " + _schema->table() + " SET ";
        std::vector<SQLData> bindValues;

        // Build SET
        bool firstSet = true;
        for (const auto& col : _schema->getColumns())
        {
            // Atualiza apenas se a coluna permite update e o SQLEntry contém
            // esse dado
            if (col.update && _entry->hasColumn(col.name))
            {
                if (!firstSet)
                {
                    statement += ", ";
                }
                statement += col.name + "=?";
                bindValues.push_back(_entry->getValue(col.name));
                firstSet = false;
            }
        }

        // Build WHERE
        statement += " WHERE ";
        bool firstWhere = true;
        for (const auto& [colName, val] : conditions)
        {
            if (!firstWhere)
            {
                statement += " AND ";
            }
            statement += colName + "=?";
            bindValues.push_back(val);
            firstWhere = false;
        }

        prepare(statement);
        for (const SQLData& val : bindValues)
        {
            if (!bindAny(val))
            {
                std::cerr << "Failed to bind value to update statement!\n";
                return false;
            }
        }
        int affected = run();
        if (affected == 0)
        {
            std::cerr << "Update skipped: Entry not found or conditions didn't "
                         "match.\n";
            return false;
        }

        return affected > 0;
    }

    bool remove(SQLSchema* _schema,
                const std::unordered_map<std::string, SQLData>& conditions)
    {
        if (conditions.empty())
        {
            std::cerr << "Remove aborted: conditions map is empty (prevents "
                         "accidental full table drop).\n";
            return false;
        }

        std::string statement = "DELETE FROM " + _schema->table() + " WHERE ";
        std::vector<SQLData> bindValues;

        bool first = true;
        for (const auto& [colName, val] : conditions)
        {
            if (!first)
                statement += " AND ";
            statement += colName + "=?";
            bindValues.push_back(val);
            first = false;
        }

        prepare(statement);
        for (const auto& val : bindValues)
        {
            if (!bindAny(val))
            {
                std::cerr << "Failed to bind value to delete statement!\n";
                return false;
            }
        }

        int affected = run();
        if (affected == 0)
        {
            std::cerr << "Delete skipped: Entry not found or conditions didn't "
                         "match.\n";
            return false;
        }

        return affected > 0;
    }

    std::vector<SQLEntry> get(SQLSchema* _schema,
                              const std::vector<std::string>& columnsToSelect,
                              const SQLConditionMap& conditions)
    {
        std::string selectCols = "";

        // Build SELECT
        if (columnsToSelect.empty())
        {
            selectCols = "*";
        }
        else
        {
            bool firstCol = true;
            for (const auto& c : columnsToSelect)
            {
                if (!firstCol)
                    selectCols += ", ";
                selectCols += c;
                firstCol = false;
            }
        }

        std::string statement =
            "SELECT " + selectCols + " FROM " + _schema->table();
        std::vector<SQLData> bindValues;

        // Build WHERE
        if (!conditions.empty())
        {
            statement += " WHERE ";
            bool firstCond = true;
            for (const auto& [colName, val] : conditions)
            {
                if (!firstCond)
                    statement += " AND ";
                statement += colName + "=?";
                bindValues.push_back(val);
                firstCond = false;
            }
        }

        return customQuery(statement, bindValues, _schema);
    }

    std::vector<SQLEntry> getAllEntries(SQLSchema* _schema)
    {
        return get(_schema, {}, {});
    }

    /**
     * @brief Realiza uma query customizável
     *
     * Exemplo:
     * @code
     *  std::vector<SQLEntry> cEntry = dbManager.customQuery(
     *      "SELECT ssid,password FROM wifi_profiles WHERE ssid=?",
     *      {std::string("SD")}, &schema);
     *  if (not cEntry.empty())
     *  {
     *      std::cout << "\tSSID: " << cEntry.at(0).getValue("ssid")
     *                << ", PWD: " << cEntry.at(0).getValue("password") << "\n";
     *  }
     * @endcode
     *
     * @param _queryStr SQL query.
     * @param _params Parâmetros para os placeholders.
     * @param _schema
     * @return Array com os resultados da query.
     */
    std::vector<SQLEntry> customQuery(const std::string& _queryStr,
                                      const std::vector<SQLData>& _params,
                                      SQLSchema* _schema)
    {
        std::vector<SQLEntry> entries;
        prepare(_queryStr);

        for (const SQLData& param : _params)
        {
            if (!bindAny(param))
            {
                std::cerr << "Failed to bind parameter to custom query!\n";
                return entries;
            }
        }

        cppdb::result res;
        try
        {
            res = stat.query();
        }
        catch (cppdb::cppdb_error const& e)
        {
            std::cerr << "Custom Query ERROR: " << e.what() << std::endl;
            stat.reset();
            return entries;
        }

        // Verify returned cols
        int num_cols = res.cols();
        std::vector<std::string> returned_cols;
        for (int i = 0; i < num_cols; ++i)
        {
            returned_cols.push_back(res.name(i));
        }

        // Map types
        std::unordered_map<std::string, SQLDataType> typeMap;
        for (const auto& col : _schema->getColumns())
        {
            typeMap[col.name] = col.type;
        }

        while (res.next())
        {
            SQLEntry data;
            for (const std::string& colName : returned_cols)
            {
                auto it = typeMap.find(colName);
                if (it == typeMap.end())
                {
                    std::cerr << "Warning: Returned column '" << colName
                              << "' is not defined in Schema.\n";
                    continue;
                }

                try
                {
                    switch (it->second)
                    {
                    case SQLDataType::Int:
                        data.setValue(colName, res.get<int>(colName));
                        break;
                    case SQLDataType::String:
                        data.setValue(colName, res.get<std::string>(colName));
                        break;
                    case SQLDataType::DateTime:
                        data.setValue(colName, res.get<std::tm>(colName));
                        break;
                    case SQLDataType::Short:
                        data.setValue(colName, res.get<short>(colName));
                        break;
                    case SQLDataType::Double:
                        data.setValue(colName, res.get<double>(colName));
                        break;
                    case SQLDataType::BigInt:
                        data.setValue(colName, res.get<long long>(colName));
                        break;
                    case SQLDataType::Blob: {
                        std::string blobStr = res.get<std::string>(colName);
                        std::vector<uint8_t> blobData(blobStr.begin(),
                                                      blobStr.end());
                        data.setValue(colName, blobData);
                        break;
                    }
                    }
                }
                catch (const cppdb::bad_value_cast& e)
                {
                    std::cerr << "Column conversion error for " << colName
                              << ": " << e.what() << "\n";
                }
            }
            entries.push_back(data);
        }

        stat.reset();
        return entries;
    }

private:
    std::unique_ptr<cppdb::session> currentSession;
    cppdb::statement stat;

    void prepare(std::string const& _statement)
    {
        // std::cout << "\t-- Preparing <" << _statement << ">\n";
        stat = currentSession->create_statement(_statement);
    }

    bool bindAny(SQLData const& _value)
    {
        try
        {
            std::visit(
                [this](auto&& arg) {
                    using T = std::decay_t<decltype(arg)>;
                    if constexpr (std::is_same_v<T, std::vector<uint8_t>>)
                    {
                        std::string blobStr(arg.begin(), arg.end());
                        this->stat.bind(blobStr);
                    }
                    else
                    {
                        this->stat.bind(arg);
                    }
                },
                _value);
            return true;
        }
        catch (const cppdb::cppdb_error& e)
        {
            std::cerr << "Bind error: " << e.what() << "\n";
            return false;
        }
    }

    cppdb::result runQuery(SQLData _arg)
    {
        SQLData dummy = 0;
        bindAny(_arg);
        return runQuery();
    }

    cppdb::result runQuery()
    {
        try
        {
            cppdb::result res = stat.query();
            // std::cout << "ID: " << stat.last_insert_id() << "\n";
            // std::cout << "Affected rows: " << stat.affected() << "\n";

            stat.reset();
            return res;
        }
        catch (cppdb::cppdb_error const& e)
        {
            std::cerr << "Statement Query ERROR: " << e.what() << std::endl;
            return cppdb::result();
        }
    }

    int run()
    {
        try
        {
            stat.exec();
            // std::cout << "ID: " << stat.last_insert_id() << "\n";
            int affectedRows = stat.affected();
            stat.reset();
            return affectedRows;
        }
        catch (cppdb::cppdb_error const& e)
        {
            std::cerr << "Statement Run ERROR: " << e.what() << std::endl;
            stat.reset();
            return -1;
        }
    }
};

class WiFiDB
{
public:
    WiFiDB(SQLManager* _manager) : db(_manager), sc("wifi_profiles", "ssid")
    {
        // insert, update, primary
        sc.addColumn("ssid", SQLDataType::String, true, true, true);
        sc.addColumn("password", SQLDataType::String, true, true, false);
        sc.addColumn("created_at", SQLDataType::DateTime, true, false, false);
        sc.addColumn("updated_at", SQLDataType::DateTime, true, true, false);
    }

    ~WiFiDB()
    {
    }

    bool saveConfig(std::string const& _ssid, std::string const& _pwd)
    {
        std::time_t now = std::time(0);
        SQLEntry profile;
        profile.setValue("ssid", _ssid);
        profile.setValue("password", _pwd);
        profile.setValue("created_at", *std::localtime(&now));
        profile.setValue("updated_at", *std::localtime(&now));

        if (db->insert(&sc, &profile) == false)
        {
            SQLConditionMap conditions;
            conditions["ssid"] = std::string(_ssid);
            return db->update(&sc, &profile, conditions);
        }
        return true;
    }

    bool removeConfig(std::string const& _ssid, std::string const& _pwd)
    {
        SQLConditionMap conditions;
        conditions["ssid"] = std::string(_ssid);
        if (_pwd != "")
        {
            conditions["password"] = std::string(_pwd);
        }
        return db->remove(&sc, conditions);
    }

    std::string getPassword(std::string const& _ssid)
    {
        std::vector<std::string> cols = {"password"};
        SQLConditionMap conditions;
        conditions["ssid"] = std::string(_ssid);
        std::vector<SQLEntry> results = db->get(&sc, cols, conditions);
        if (results.size() > 1)
        {
            std::cout << "More than one profile with the same SSID! Returning "
                         "first\n";
            return std::get<std::string>(results.at(0).getValue("password"));
        }
        return "";
    }

    typedef struct WiFiConfig
    {
        std::string ssid;
        std::string password;
    } WiFiConfig;

    std::vector<WiFiConfig> getWiFiEntries()
    {
        std::vector<WiFiConfig> entries;

        std::vector<SQLEntry> results = db->getAllEntries(&sc);
        for (const auto& entry : results)
        {
            // std::tm c = std::get<std::tm>(entry.getValue("created_at"));
            // std::tm u = std::get<std::tm>(entry.getValue("updated_at"));
            // std::cout << "--> Found:\n";
            // std::cout << "\tSSID: " << entry.getValue("ssid")
            //           << ", PWD: " << entry.getValue("password")
            //           << ",\n\tCREATED: " << asctime(&c)
            //           << "\tUPDATED: " << asctime(&u);
            // std::cout << "<--\n";
            entries.push_back(
                {std::get<std::string>(entry.getValue("ssid")),
                 std::get<std::string>(entry.getValue("password"))});
        }
        return entries;
    }

private:
    SQLManager* db;
    SQLSchema sc;
};

int main(int argc, char* argv[])
{
    SQLManager dbManager;
    bool ok = dbManager.init("setup_management", "postgres", "postgres");

    WiFiDB wifiDB(&dbManager);

    // wifiDB.saveConfig("SOCIADLROIDS2", "Slinky2027");
    wifiDB.removeConfig("SOCIADLROIDS2", "Slinky2027");
    for (const auto& config : wifiDB.getWiFiEntries())
    {
        std::cout << "Entry - SSID = " << config.ssid
                  << " | PWD = " << config.password << "\n";
    }

    return 0;
}

int test_main(int argc, char* argv[])
{
    SQLManager dbManager;
    bool ok = dbManager.init("setup_management", "postgres", "postgres");
    // std::cout << "Manager Init OK=" << ok << "\n";

    SQLSchema schema("wifi_profiles", "ssid");
    schema.addColumn("ssid", SQLDataType::String, true, true, true);
    schema.addColumn("password", SQLDataType::String, true, true, false);
    schema.addColumn("created_at", SQLDataType::DateTime, true, false, false);
    schema.addColumn("updated_at", SQLDataType::DateTime, true, true, false);

    // std::cout << "Schema: " << schema.table() << ", " << schema.uniqueKey()
    //           << "\n\t UPDATE= " << schema.updateStatement()
    //           << "\n\t INSERT= " << schema.insertStatement()
    //           << "\n\t DELETE= " << schema.deleteStatement() << "\n";

    if (ok)
    {
        std::time_t now = std::time(0);
        SQLEntry profile;
        profile.setValue("ssid", "SD");
        profile.setValue("password", "12345");
        profile.setValue("created_at", *std::localtime(&now));
        profile.setValue("updated_at", *std::localtime(&now));

        SQLConditionMap conditions;
        conditions["ssid"] = std::string("SD");
        if (dbManager.update(&schema, &profile, conditions))
        // if (dbManager.insert(&schema, &profile))
        {
            std::cerr << "Entry added to DB!\n";
            // dbManager.remove(&schema, conditions);
        }

        std::vector<std::string> cols = {"ssid", "password", "created_at",
                                         "updated_at"};

        std::vector<SQLEntry> results =
            dbManager.get(&schema, cols, conditions);
        for (const auto& entry : results)
        {
            std::tm c = std::get<std::tm>(entry.getValue("created_at"));
            std::tm u = std::get<std::tm>(entry.getValue("updated_at"));

            std::cout << "--- GET\n";
            std::cout << "\tSSID: " << entry.getValue("ssid")
                      << ", PWD: " << entry.getValue("password")
                      << ",\n\tCREATED: " << asctime(&c)
                      << "\tUPDATED: " << asctime(&u) << "\n\n";
        }

        results = dbManager.getAllEntries(&schema);
        std::cout << "--- GET ALL\n";
        for (const auto& entry : results)
        {
            std::tm c = std::get<std::tm>(entry.getValue("created_at"));
            std::tm u = std::get<std::tm>(entry.getValue("updated_at"));

            std::cout << "--> Found:\n";
            std::cout << "\tSSID: " << entry.getValue("ssid")
                      << ", PWD: " << entry.getValue("password")
                      << ",\n\tCREATED: " << asctime(&c)
                      << "\tUPDATED: " << asctime(&u);
            std::cout << "<--\n";
        }

        std::cout << "--- Teste Custom ---\n";

        std::vector<SQLEntry> cEntry = dbManager.customQuery(
            "SELECT ssid,password FROM wifi_profiles WHERE ssid=?",
            {std::string("SD")}, &schema);
        if (not cEntry.empty())
        {
            // std::tm c =
            // std::get<std::tm>(cEntry.at(0).getValue("created_at")); std::tm u
            // = std::get<std::tm>(cEntry.at(0).getValue("updated_at"));
            std::cout << "\tSSID: " << cEntry.at(0).getValue("ssid")
                      << ", PWD: " << cEntry.at(0).getValue("password") << "\n";
            // << ",\n\tCREATED: " << asctime(&c)
            // << "\tUPDATED: " << asctime(&u) << "\n\n";
        }
    }

    return 0;
}
