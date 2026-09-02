#include "VehicleMonitorConstants.hpp"
#include "vehicle-client-glue.h"

#include <sdbus-c++/sdbus-c++.h>

#include <cstdint>
#include <exception>
#include <iostream>

namespace
{
class VehicleMonitorProxy final
    : public sdbus::ProxyInterfaces<com::labprob::VehicleMonitor_proxy>
{
public:
    VehicleMonitorProxy(sdbus::IConnection& connection,
                        const char* serviceName,
                        const char* objectPath)
        : ProxyInterfaces(connection, serviceName, objectPath)
    {
        registerProxy();
    }

    ~VehicleMonitorProxy()
    {
        unregisterProxy();
    }

    VehicleMonitorProxy(const VehicleMonitorProxy&) = delete;
    VehicleMonitorProxy& operator=(const VehicleMonitorProxy&) = delete;

private:
    void onCpuUsageChanged(const double&) override {}
    void onMemoryUsageChanged(const double&) override {}
    void onVehicleSpeedChanged(const std::int32_t&) override {}
};
} // namespace

int main()
{
    try
    {
        auto connection = sdbus::createSessionBusConnection();
        VehicleMonitorProxy proxy(*connection,
                                  vehicle_monitor::service_name,
                                  vehicle_monitor::object_path);

        std::cout << "CPU usage: " << proxy.GetCpuUsage() << "%\n"
                  << "Memory usage: " << proxy.GetMemoryUsage() << "%\n"
                  << "Uptime: " << proxy.GetUptime() << '\n'
                  << "Vehicle speed: " << proxy.GetVehicleSpeed() << " km/h\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Vehicle D-Bus client failed: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
