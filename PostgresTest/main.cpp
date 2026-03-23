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

    // ------------------------------------------------------------------------
    // WIFI TEST
    // wifiDB.removeConfig("SOCIADLROIDS2", "Slinky2027");
    // wifiDB.saveConfig("SOCIADLROIDS2", "Slinky2027");
    // for (const auto& config : wifiDB.getWiFiEntries())
    // {
    //     std::cout << config;
    // }

    // ------------------------------------------------------------------------
    // WORK INTERVAL TEST
    // workIntervalDB.add(RobotWorkIntervalsDB::TUESDAY, "06:00:00",
    // "23:00:00"); workIntervalDB.clear(RobotWorkIntervalsDB::TUESDAY); for
    // (auto &n : workIntervalDB.getIntervals(RobotWorkIntervalsDB::TUESDAY))
    // {
    //      std::cout << n;
    // }

    // ------------------------------------------------------------------------
    // TIME TEST
    // RobotTimeDB::TimeSettings settings, ret;
    // settings.id = 1;
    //
    // std::time_t now = std::time(0);
    // settings.configured_datetime = *std::localtime(&now);
    // settings.configured_datetime_use_utc = false;
    // settings.automatic_time_sync_enabled = false;
    //
    // timeDB.remove(1);
    // timeDB.save(settings);
    // ret = timeDB.getSettings(1);
    // if (ret.id != -1)
    //     std::cout << ret;
    //
    // for (auto &n : timeDB.getSettings())
    // {
    //      std::cout << n;
    // }

    // ------------------------------------------------------------------------
    // HOTSPOT TEST
    // hotspotDB.save(1, "RobotHS-2", "SD2026");
    // // hotspotDB.remove(1);
    // HotspotSettingsDB::HotspotSettings ret = hotspotDB.getSettings(1);
    // if (ret.id != -1)
    //     std::cout << ret;
    //
    // for (auto &n : hotspotDB.getHotspotEntries())
    // {
    //      std::cout << n;
    // }

    // ------------------------------------------------------------------------
    // Maps TEST
    MapsDB::MapData mData;
    // mData.pgm_path = "/home/socialdroids/map.pgm";
    // mData.yaml_path = "/home/socialdroids/map.yaml";
    // mData.stl_path = "/home/socialdroids/map_3d.stl";
    // mData.obstacles_pgm_path = "/home/socialdroids/obstacles.pgm";
    //
    // mapsDB.save(mData);
    // mapsDB.update(1, mData);
    // mData = mapsDB.getMap(17);
    // std::cout << mData;
    for (auto const& n : mapsDB.getAll())
    {
        std::cout << n;
    }
    // mapsDB.remove(16);

    // ------------------------------------------------------------------------
    // Waypoints TEST
    // MapWaypointsDB::MapWaypoints wp;
    // wp.map_id = 16;
    // wp.identifier = "copa";
    // wp.x = 1.;
    // wp.y = 0.23;
    // waypointsDB.add(wp);
    //
    // wp.identifier = "base";
    // wp.x = 0.;
    // wp.y = 0.5;
    // waypointsDB.add(wp);
    //
    // wp.map_id = 15;
    // wp.identifier = "copa";
    // wp.x = 1.;
    // wp.y = 0.23;
    // waypointsDB.add(wp);
    // wp.identifier = "copa";
    // wp.x = 1.;
    // wp.y = 1.5;
    // wp.angle.euler.yaw = 0.5;
    // wp.angle.is_euler = true;
    // // waypointsDB.add(wp);
    //
    // wp.map_id = 15;
    // wp.setID(3);
    // // waypointsDB.add(wp);
    //
    // waypointsDB.removeAll(15);
    //
    // for (auto &n : waypointsDB.getByMap(15)) {
    //     std::cout << n;
    // }

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
