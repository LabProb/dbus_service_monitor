#pragma once

#include "vehicle-server-glue.h"

#include <sdbus-c++/sdbus-c++.h>

#include <string>

class VehicleService
    : public sdbus::AdaptorInterfaces<com::labprob::VehicleMonitor_adaptor>
{
public:
    VehicleService(sdbus::IConnection& connection,
                   const std::string& objectPath);

    ~VehicleService();

protected:
    double GetCpuUsage() override;

    double GetMemoryUsage() override;

    int32_t GetVehicleSpeed() override;

    std::string GetUptime() override;
};
