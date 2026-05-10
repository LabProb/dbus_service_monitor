#include "VehicleService.hpp"

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
    return 42.5;
}

double VehicleService::GetMemoryUsage()
{
    return 68.2;
}

int32_t VehicleService::GetVehicleSpeed()
{
    return 72; // mock speed
}

std::string VehicleService::GetUptime()
{
    return "1 day 12 hours";
}