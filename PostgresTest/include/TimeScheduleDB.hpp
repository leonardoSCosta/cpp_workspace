#ifndef INCLUDE_INCLUDE_TIMESCHEDULEDB_HPP_
#define INCLUDE_INCLUDE_TIMESCHEDULEDB_HPP_

#include "DBAbstractionLayer.hpp"

using namespace std;
class RobotWorkIntervalsDB : public DBInterface
{

public:
    typedef struct WorkInterval
    {
        int64_t id;
        string weekday;
        tm start_time;
        tm end_time;
        tm created_at;
        tm updated_at;

    } WorkInterval;

    RobotWorkIntervalsDB(SQLManager* _manager, const string& _table,
                         const string& _primaryKey)
        : DBInterface(_manager, _table, _primaryKey)
    {
        // insert, update, primary
        sc.addColumn("id", SQLDataType::BigInt, false, false, true);
        sc.addColumn("weekday", SQLDataType::Text, true, true, false);
        sc.addColumn("start_time", SQLDataType::Time, true, true, false);
        sc.addColumn("end_time", SQLDataType::Time, true, true, false);
        sc.addColumn("created_at", SQLDataType::TimestampTZ, true, false,
                     false);
        sc.addColumn("updated_at", SQLDataType::TimestampTZ, true, true, false);
    }

    ~RobotWorkIntervalsDB()
    {
    }

    bool add(const string& _weekday, const tm& _start, const tm& _end)
    {
        time_t now = time(0);
        SQLEntry entry;
        entry.setValue("weekday", _weekday);
        entry.setValue("start_time", _start);
        entry.setValue("end_time", _end);

        entry.setValue("created_at", *localtime(&now));
        entry.setValue("updated_at", *localtime(&now));

        return db->insert(&sc, &entry);
    }

    bool clear(const string& _weekday)
    {
        SQLConditionMap conditions;
        conditions["weekday"] = _weekday;
        return db->remove(&sc, conditions);
    }

    std::vector<WorkInterval> getIntervals(const string& _weekday)
    {
        vector<string> cols = sc.getColumnsNames();
        SQLConditionMap conditions;
        conditions["weekday"] = _weekday;
        std::vector<WorkInterval> intervals;

        for (SQLEntry const& data : db->get(&sc, cols, conditions))
        {
            intervals.push_back({get<int64_t>(data.getValue("id")),
                                 get<string>(data.getValue("weekday")),
                                 get<tm>(data.getValue("start_time")),
                                 get<tm>(data.getValue("end_time")),
                                 get<tm>(data.getValue("created_at")),
                                 get<tm>(data.getValue("updated_at"))});
        }
        return intervals;
    }

private:
};

class RobotTimeDB : public DBInterface
{
public:
    typedef struct TimeSettings
    {
        int16_t id;
        tm configured_datetime;
        bool configured_datetime_use_utc;
        bool automatic_time_sync_enabled;
        tm created_at;
        tm updated_at;
    } TimeSettings;

    RobotTimeDB(SQLManager* _manager, const string& _table,
                const string& _primaryKey)
        : DBInterface(_manager, _table, _primaryKey)
    {
        // TODO: Confirmar se não seria melhor deixar como SERIAL, para o DB
        // determinar o id correto

        // insert, update, primary
        sc.addColumn("id", SQLDataType::SmallInt, false, false, true);
        sc.addColumn("configured_datetime", SQLDataType::TimestampTZ, true,
                     true, false);
        sc.addColumn("configured_datetime_use_utc", SQLDataType::Boolean, true,
                     true, false);
        sc.addColumn("automatic_time_sync_enabled", SQLDataType::Boolean, true,
                     true, false);
        sc.addColumn("created_at", SQLDataType::TimestampTZ, true, false,
                     false);
        sc.addColumn("updated_at", SQLDataType::TimestampTZ, true, true, false);
    }

    ~RobotTimeDB()
    {
    }

    bool save(const TimeSettings& _settings)
    {
        time_t now = time(0);
        SQLEntry entry;
        entry.setValue("id", _settings.id);
        entry.setValue("configured_datetime", _settings.configured_datetime);
        entry.setValue("configured_datetime_use_utc",
                       _settings.configured_datetime_use_utc);
        entry.setValue("automatic_time_sync_enabled",
                       _settings.automatic_time_sync_enabled);

        entry.setValue("created_at", *localtime(&now));
        entry.setValue("updated_at", *localtime(&now));

        if (db->insert(&sc, &entry) == false)
        {
            SQLConditionMap conditions;
            conditions["id"] = _settings.id;
            return db->update(&sc, &entry, conditions);
        }
        return true;
    }

    bool remove(int16_t _id)
    {
        SQLConditionMap conditions;
        conditions["id"] = _id;
        return db->remove(&sc, conditions);
    }

    TimeSettings getSettings(int16_t _id)
    {
        vector<string> cols = sc.getColumnsNames();
        SQLConditionMap conditions;
        conditions["id"] = _id;
        vector<SQLEntry> results = db->get(&sc, cols, conditions);
        TimeSettings out;
        out.id = -1;

        if (!results.empty())
        {
            out.id = get<int16_t>(results.at(0).getValue("id"));
            out.configured_datetime =
                get<tm>(results.at(0).getValue("configured_datetime"));
            out.configured_datetime_use_utc = get<short>(
                results.at(0).getValue("configured_datetime_use_utc"));
            out.automatic_time_sync_enabled = get<short>(
                results.at(0).getValue("automatic_time_sync_enabled"));
            out.created_at = get<tm>(results.at(0).getValue("created_at"));
            out.updated_at = get<tm>(results.at(0).getValue("updated_at"));
        }
        return out;
    }

    std::vector<TimeSettings> getSettings()
    {
        vector<SQLEntry> results = db->getAllEntries(&sc);
        TimeSettings aux;
        std::vector<TimeSettings> out;

        if (!results.empty())
        {
            aux.id = get<int16_t>(results.at(0).getValue("id"));

            aux.configured_datetime =
                get<tm>(results.at(0).getValue("configured_datetime"));

            aux.configured_datetime_use_utc = get<short>(
                results.at(0).getValue("configured_datetime_use_utc"));

            aux.automatic_time_sync_enabled = get<short>(
                results.at(0).getValue("automatic_time_sync_enabled"));

            aux.created_at = get<tm>(results.at(0).getValue("created_at"));
            aux.updated_at = get<tm>(results.at(0).getValue("updated_at"));
            out.push_back(aux);
        }
        return out;
    }

private:
};

#endif  // INCLUDE_INCLUDE_TIMESCHEDULEDB_HPP_
