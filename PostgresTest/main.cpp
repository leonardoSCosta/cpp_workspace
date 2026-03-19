#include <any>
#include <cppdb/errors.h>
#include <cppdb/frontend.h>
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
using SQLData = std::variant<int, std::tm, std::string, short, double>;
enum class SQLDataType
{
    Int,      // int
    DateTime, // std::tm
    String,   // std::string
    Short,    // short
    Double    // double
};

std::ostream& operator<<(std::ostream& os, const SQLData& v)
{
    std::visit([&os](auto&& arg) { os << arg; }, v);
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

    std::string updateStatement() const
    {
        std::string statement = "UPDATE " + table() + " SET ";
        for (size_t n = 0; n < columns.size(); n++)
        {
            if (columns.at(n).update == true)
            {
                if (n < columns.size() - 1)
                {
                    statement += columns.at(n).name + "=?, ";
                }
                else
                {
                    statement += columns.at(n).name + "=? ";
                }
            }
        }
        statement += "WHERE " + uniqueKey() + "=?";
        return statement;
    }

    std::string insertStatement() const
    {
        std::string statement = "INSERT INTO " + table() + "(";
        std::string placeholders = " VALUES(";
        for (size_t n = 0; n < columns.size(); n++)
        {
            if (columns.at(n).insert == true)
            {
                if (n < columns.size() - 1)
                {
                    statement += columns.at(n).name + ", ";
                    placeholders += "?,";
                }
                else
                {
                    statement += columns.at(n).name + ")";
                    placeholders += "?)";
                }
            }
        }
        statement += placeholders;
        return statement;
    }

    std::string deleteStatement() const
    {
        std::string statement =
            "DELETE FROM " + table() + " WHERE " + uniqueKey() + "=?";
        return statement;
    }

    std::string selectStatement() const
    {
        std::string statement =
            "SELECT * FROM " + table() + " WHERE " + uniqueKey() + "=?";
        return statement;
    }

    std::string selectAllStatement() const
    {
        std::string statement = "SELECT * FROM " + table();
        return statement;
    }

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

    std::vector<SQLData> getValues(const SQLSchema* _schema,
                                   SQL_ENTRY_GET_TYPES _type)
    {
        std::vector<SQLData> values;

        for (const SQLSchema::Column_t& column : _schema->getColumns())
        {
            if ((_type == UPDATE && column.update) ||
                (_type == INSERT && column.insert))
            {
                values.push_back(getValue(column.name));
            }
        }

        // Valor extra para o placeholder do ...WHERE <UNIQUE_KEY> =?
        if (_type == UPDATE)
        {
            values.push_back(getValue(_schema->uniqueKey()));
        }

        return values;
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
        std::vector<SQLData> data;
        if (exists(_schema->table(), _schema->uniqueKey(),
                   _entry->getValue(_schema->uniqueKey())))
        {
            std::cerr << "Entry already exists in table!\n";
            return false;
        }
        else
        {
            prepare(_schema->insertStatement());
            for (SQLData const& _d :
                 _entry->getValues(_schema, SQLEntry::INSERT))
            {
                if (!bindAny(_d))
                {
                    std::cerr << "Failed to bind value to statement!\n";
                }
            }
        }

        return run();
    }

    bool update(SQLSchema* _schema, SQLEntry* _entry)
    {
        if (exists(_schema->table(), _schema->uniqueKey(),
                   _entry->getValue(_schema->uniqueKey())))
        {
            std::cout << "Entry exists, updating data in DB\n";
            prepare(_schema->updateStatement());
            for (SQLData const& _d :
                 _entry->getValues(_schema, SQLEntry::UPDATE))
            {
                if (!bindAny(_d))
                {
                    std::cerr << "Failed to bind value to statement!\n";
                }
            }
        }
        else
        {
            std::cerr << "Entry not found in table! Insert it first\n";
            return false;
        }

        return run();
    }

    bool remove(SQLSchema* _schema, SQLData _uniqueValue)
    {
        if (not exists(_schema->table(), _schema->uniqueKey(), _uniqueValue))
        {
            std::cerr << "Entry not found!\n";
            return false;
        }
        else
        {
            std::cout << "Found entry! Deleting from DB.\n";
            prepare(_schema->deleteStatement());
            bindAny(_uniqueValue);
            return run();
        }
    }

    bool remove(SQLSchema* _schema, SQLEntry* _entry)
    {
        // TODO: Permitir que a exclusão considere múltiplas chaves primárias
        if (not exists(_schema->table(), _schema->uniqueKey(),
                       _entry->getValue(_schema->uniqueKey())))
        {
            std::cerr << "Entry not found!\n";
            return false;
        }
        else
        {
            std::cout << "Found entry! Deleting from DB.\n";
            prepare(_schema->deleteStatement());
            bindAny(_entry->getValue(_schema->uniqueKey()));
            return run();
        }
    }

    SQLEntry getEntry(SQLSchema* _schema, SQLData _uniqueValue)
    {
        SQLEntry data;
        if (not exists(_schema->table(), _schema->uniqueKey(), _uniqueValue))
        {
            std::cerr << "Entry not found!\n";
        }
        else
        {
            std::cout << "Found entry! Returning from DB.\n";
            prepare(_schema->selectStatement());
            cppdb::result res = runQuery(_uniqueValue);

            if (res.next())
            {
                for (const auto& col : _schema->getColumns())
                {
                    try
                    {
                        switch (col.type)
                        {
                        case SQLDataType::Int:
                            data.setValue(col.name, res.get<int>(col.name));
                            break;

                        case SQLDataType::String:
                            data.setValue(col.name,
                                          res.get<std::string>(col.name));
                            break;

                        case SQLDataType::DateTime:
                            data.setValue(col.name, res.get<std::tm>(col.name));
                            break;

                        case SQLDataType::Short:
                            data.setValue(col.name, res.get<short>(col.name));
                            break;

                        case SQLDataType::Double:
                            data.setValue(col.name, res.get<double>(col.name));
                            break;
                        }
                    }
                    catch (const cppdb::bad_value_cast& e)
                    {
                        std::cerr << "Column data conversion error " << col.name
                                  << ". Database type different from expected."
                                  << e.what() << "\n";
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "Warning: Column '" << col.name
                                  << "' not found in SELECT result." << e.what()
                                  << "\n";
                    }
                }
            }
        }
        return data;
    }

    std::vector<SQLEntry> getAllEntries(SQLSchema* _schema)
    {
        std::vector<SQLEntry> entries;
        SQLEntry data;
        prepare(_schema->selectAllStatement());
        cppdb::result res = runQuery();

        while (res.next())
        {
            for (const auto& col : _schema->getColumns())
            {
                try
                {
                    switch (col.type)
                    {
                    case SQLDataType::Int:
                        data.setValue(col.name, res.get<int>(col.name));
                        break;

                    case SQLDataType::String:
                        data.setValue(col.name, res.get<std::string>(col.name));
                        break;

                    case SQLDataType::DateTime:
                        data.setValue(col.name, res.get<std::tm>(col.name));
                        break;

                    case SQLDataType::Short:
                        data.setValue(col.name, res.get<short>(col.name));
                        break;

                    case SQLDataType::Double:
                        data.setValue(col.name, res.get<double>(col.name));
                        break;
                    }
                }
                catch (const cppdb::bad_value_cast& e)
                {
                    std::cerr << "Column data conversion error " << col.name
                              << ". Database type different from expected."
                              << e.what() << "\n";
                }
                catch (const std::exception& e)
                {
                    std::cerr << "Warning: Column '" << col.name
                              << "' not found in SELECT result." << e.what()
                              << "\n";
                }
            }
            entries.push_back(data);
        }
        return entries;
    }

private:
    std::unique_ptr<cppdb::session> currentSession;
    cppdb::statement stat;

    bool exists(std::string const& _table, std::string const& _key,
                SQLData const& _value)
    {
        prepare("SELECT 1 FROM " + _table + " WHERE " + _key + "=?");
        cppdb::result res = runQuery(_value);

        try
        {
            if (res.next())
            {
                return true;
            }
        }
        catch (cppdb::cppdb_error const& e)
        {
            std::cerr << "Failed to check if key exists in table(" << _table
                      << ") at column(" << _key << ")\n";
            std::cerr << "Returning true for safety\n";
            return true;
        }
        return false;
    }

    void prepare(std::string const& _statement)
    {
        // std::cout << "\t-- Preparing <" << _statement << ">\n";
        stat = currentSession->create_statement(_statement);
    }

    bool bindAny(SQLData const& _value)
    {
        try
        {
            std::visit([this](auto&& arg) { this->stat.bind(arg); }, _value);
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

    bool run()
    {
        try
        {
            stat.exec();
            // std::cout << "ID: " << stat.last_insert_id() << "\n";
            // std::cout << "Affected rows: " << stat.affected() << "\n";
            stat.reset();
            return true;
        }
        catch (cppdb::cppdb_error const& e)
        {
            std::cerr << "Statement Run ERROR: " << e.what() << std::endl;
            stat.reset();
            return false;
        }

        return true;
    }
};

int main(int argc, char* argv[])
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

        if (!dbManager.update(&schema, &profile))
        {
            std::cerr << "Failed to add data!\n";
            // dbManager.remove(&schema, "SD");
        }

        SQLEntry dbEntry = dbManager.getEntry(&schema, "SD");
        if (dbEntry.hasColumn("ssid"))
        {
            std::tm c = std::get<std::tm>(dbEntry.getValue("created_at"));
            std::tm u = std::get<std::tm>(dbEntry.getValue("updated_at"));

            std::cout << "\tSSID: " << dbEntry.getValue("ssid")
                      << ", PWD: " << dbEntry.getValue("password")
                      << ",\n\tCREATED: " << asctime(&c)
                      << "\tUPDATED: " << asctime(&u) << "\n\n";
        }

        std::vector<SQLEntry> entries = dbManager.getAllEntries(&schema);
        for (const auto& entry : entries)
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
    }

    return 0;
}
