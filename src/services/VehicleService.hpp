#pragma once

#include "vehicle-server-glue.h"

#include <sdbus-c++/sdbus-c++.h>

#include <cstdint>
#include <string>

class VehicleService final
    : public sdbus::AdaptorInterfaces<com::labprob::VehicleMonitor_adaptor>
{
public:
    VehicleService(sdbus::IConnection& connection,
                   const std::string& objectPath);

    ~VehicleService();

    VehicleService(const VehicleService&) = delete;
    VehicleService& operator=(const VehicleService&) = delete;

protected:
    double GetCpuUsage() override;

    double GetMemoryUsage() override;

    int32_t GetVehicleSpeed() override;

    std::string GetUptime() override;
};
