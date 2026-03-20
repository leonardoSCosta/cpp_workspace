#include "DBAbstractionLayer.hpp"
#include "MapsDB.hpp"
#include "NetworkDB.hpp"
#include "TimeScheduleDB.hpp"

int main(int argc, char* argv[])
{
    SQLManager dbManager;
    bool ok = dbManager.init("setup_management", "postgres", "postgres");

    WiFiDB wifiDB(&dbManager, "wifi_profiles", "ssid");
    HotspotSettingsDB hotspotDB(&dbManager, "hotspot_settings", "id");

    RobotWorkIntervalsDB workIntervalDB(
        &dbManager, "robot_working_allowed_intervals", "id");
    RobotTimeDB timeDB(&dbManager, "robot_time_settings", "id");

    MapWaypointsDB waypointsDB(&dbManager, "map_waypoints", "id");

    MapPolygonPointsDB polyPointsDB(&dbManager, "map_polygon_points",
                                    "polygon_id");
    MapPolygonsDB polygonsDB(&dbManager, "map_polygons", "id", &polyPointsDB);
    MapsDB mapsDB(&dbManager, "maps", "id", &waypointsDB, &polygonsDB);

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
