#include "VehicleService.hpp"

namespace
{
constexpr double cpu_usage_percent = 42.5;
constexpr double memory_usage_percent = 68.2;
constexpr std::int32_t vehicle_speed_kmh = 72;
constexpr char uptime[] = "1 day 12 hours";
} // namespace

VehicleService::VehicleService(
    sdbus::IConnection& connection,
    const std::string& objectPath)
    : AdaptorInterfaces(connection, objectPath.c_str())
{
    registerAdaptor();
}

VehicleService::~VehicleService()
{
    unregisterAdaptor();
}

double VehicleService::GetCpuUsage()
{
    return cpu_usage_percent;
}

double VehicleService::GetMemoryUsage()
{
    return memory_usage_percent;
}

int32_t VehicleService::GetVehicleSpeed()
{
    return vehicle_speed_kmh;
}

std::string VehicleService::GetUptime()
{
    return uptime;
}
