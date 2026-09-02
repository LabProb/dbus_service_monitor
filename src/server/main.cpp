#include "VehicleService.hpp"
#include "VehicleMonitorConstants.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <iostream>

int main()
{
    try
    {
        auto connection = sdbus::createSessionBusConnection();

        connection->requestName(vehicle_monitor::service_name);

        VehicleService service(*connection, vehicle_monitor::object_path);

        std::cout << "Vehicle DBus service started\n";

        connection->enterEventLoop();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Vehicle D-Bus service failed: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
