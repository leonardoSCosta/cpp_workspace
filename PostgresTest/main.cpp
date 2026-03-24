#include "DBAbstractionLayer.hpp"
#include "MapsDB.hpp"
#include "NetworkDB.hpp"
#include "TimeScheduleDB.hpp"

int main(int argc, char* argv[])
{
    SQLManager dbManager;
    bool ok = dbManager.init("setup_management", "postgres", "postgres");

    WiFiDB wifiDB(&dbManager, "wifi_profiles");
    HotspotSettingsDB hotspotDB(&dbManager, "hotspot_settings");

    RobotWorkIntervalsDB workIntervalDB(
        &dbManager, "robot_working_allowed_intervals");
    RobotTimeDB timeDB(&dbManager, "robot_time_settings");

    MapWaypointsDB waypointsDB(&dbManager, "map_waypoints");

    MapPolygonPointsDB polyPointsDB(&dbManager, "map_polygon_points");
    MapPolygonsDB polygonsDB(&dbManager, "map_polygons", &polyPointsDB);
    MapsDB mapsDB(&dbManager, "maps", &waypointsDB, &polygonsDB);

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
    //
    // workIntervalDB.add(RobotWorkIntervalsDB::TUESDAY, "06:00:00",
    //                    "23:00:00");
    // for (auto &n :
    //      workIntervalDB.getIntervals(RobotWorkIntervalsDB::TUESDAY))
    // {
    //      std::cout << n;
    // }
    // workIntervalDB.clear(RobotWorkIntervalsDB::TUESDAY);

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
    // MapsDB::MapData mData;
    // mData.pgm_path = "/home/socialdroids/map.pgm";
    // mData.yaml_path = "/home/socialdroids/map.yaml";
    // mData.stl_path = "/home/socialdroids/map_3d.stl";
    // mData.obstacles_pgm_path = "/home/socialdroids/obstacles.pgm";
    //
    // // mapsDB.remove(1);
    // // mapsDB.remove(2);
    // // mapsDB.remove(3);
    // // mapsDB.remove(4);
    // // mapsDB.remove(5);
    // int64_t mapID = 1;
    // // mapsDB.save(mData, mapID);
    // // mapsDB.update(1, mData);
    // // mData = mapsDB.getMap(17);
    // // std::cout << mData;
    // for (auto const& n : mapsDB.getAll())
    // {
    //     std::cout << n;
    // }

    // ------------------------------------------------------------------------
    // Waypoints TEST
    // MapWaypointsDB::MapWaypoints wp;
    // wp.setID(10);
    // wp.map_id = mapID;
    // wp.identifier = "João José 2";
    // wp.x = 1.5;
    // wp.y = 0.73;
    // wp.angle.euler.yaw = 0.75;
    // if(waypointsDB.update(wp))
    //     std::cout << "Waypoint added! ID = " << wp.getID() << "\n";
    // else
    //     std::cout << "Waypoint add failed! ID = " << wp.getID() << "\n";
    //
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
    // for (auto &n : waypointsDB.getByMap(mapID)) {
    //     std::cout << n;
    // }

    // ------------------------------------------------------------------------
    // Polygons TEST
    //
    // MapPolygonsDB::MapPolygon poly;
    // poly.map_id = mapID;
    // poly.setType(MapPolygonsDB::PolygonAllowedTypes::BORDER);
    // poly.identifier = "Map Border";
    // poly.createPoint(0, 0);
    // poly.createPoint(1, 1);
    // poly.createPoint(2, 2);
    // poly.createPoint(3, 3);
    //
    // // polygonsDB.remove(44, mapID);
    // // return 0;
    // if (polygonsDB.add(poly))
    // {
    //     poly.resetPoints();
    //     poly.createPoint(5, 5);
    //     poly.createPoint(1, 1);
    //     poly.createPoint(2, 2);
    //     poly.createPoint(3, 3);
    //     poly.createPoint(25, 5*poly.getID());
    //     polygonsDB.update(poly);
    //
    //     for (MapPolygonsDB::MapPolygon& n : polygonsDB.getByMap(mapID))
    //     {
    //         std::cout << n;
    //         n.createPoint(10, 10);
    //         std::cout << "Updated: " << n;
    //         polygonsDB.update(n);
    //     }
    //
    //     // polygonsDB.remove(poly.getID(), mapID);
    // }
    //
    // std::cout << "\n--- Final ---\n";
    // for (MapPolygonsDB::MapPolygon& n : polygonsDB.getByMap(mapID))
    // {
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
