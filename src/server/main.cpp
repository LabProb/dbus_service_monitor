#include "VehicleService.hpp"

#include <sdbus-c++/sdbus-c++.h>

#include <iostream>

constexpr const char* SERVICE_NAME =
    "com.labprob.VehicleMonitor";

constexpr const char* OBJECT_PATH =
    "/com/labprob/VehicleMonitor";

int main()
{
    try
    {
        auto connection =
            sdbus::createSessionBusConnection();

        connection->requestName(SERVICE_NAME);

        VehicleService service(*connection, OBJECT_PATH);

        std::cout << "Vehicle DBus service started\n";

        connection->enterEventLoop();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}