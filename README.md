# dbus_service_monitor

`dbus_service_monitor` is a small C++20 example of exposing vehicle-monitoring
values over a session D-Bus service. It demonstrates a complete
`sdbus-c++-xml2cpp` workflow: the XML interface definition generates the
adaptor used by the server and the proxy used by the client.

The values currently returned by the service are fixed mock values. This keeps
the example focused on the D-Bus boundary rather than platform-specific metric
collection.

## Architecture

```text
                    session D-Bus
vehicle_client ------------------------> vehicle_server
                                            |
                                            v
                                      VehicleService
```

`vehicle_server` owns a session-bus connection, claims the service name, and
registers `VehicleService` at the configured object path. `VehicleService`
implements the generated adaptor methods. `vehicle_client` creates a generated
proxy on its own session-bus connection and makes synchronous method calls to
the server.

## Technologies

- C++20
- CMake 3.16 or later and GNU Make (the Makefile is a convenience wrapper)
- `pkg-config` for discovering `sdbus-c++`
- D-Bus session bus
- `sdbus-c++`
- `sdbus-c++-xml2cpp` to generate C++ adaptor and proxy headers from XML

## Repository layout

```text
CMakeLists.txt             CMake build configuration
Makefile                   Convenience targets for an out-of-source build
dbus/xml/vehicle.xml       Authoritative D-Bus interface definition
src/VehicleMonitorConstants.hpp  Shared service name and object path
src/services/              VehicleService adaptor implementation
src/server/                vehicle_server entry point
src/client/                vehicle_client generated-proxy consumer
```

## Requirements

Install a C++20-capable compiler, CMake, Make, `pkg-config`, D-Bus, and the
development package that provides `sdbus-c++` and `sdbus-c++-xml2cpp`.

On Debian/Ubuntu-derived systems the relevant packages are commonly named
`build-essential`, `cmake`, `pkg-config`, `libsystemd-dev`, and
`libsdbus-c++-dev`. Package names can differ by distribution.

## Build

Use an out-of-source build:

```sh
cmake -S . -B build
cmake --build build
```

Alternatively, `make` runs the same configure-and-build workflow. CMake
provides these options, both enabled by default:

```sh
cmake -S . -B build -DBUILD_CLIENT=OFF
cmake -S . -B build -DBUILD_SERVER=OFF
```

Generated headers are written to `build/generated/`; they are build artifacts
and must not be edited or committed. Regenerate them by rebuilding after
changing `dbus/xml/vehicle.xml`:

```sh
cmake --build build --target generate_dbus_bindings
```

## Running

Both programs use the **session** bus. Run the server in one terminal with an
active graphical/login session bus:

```sh
./build/vehicle_server
```

Then run the client from the same session-bus environment:

```sh
./build/vehicle_client
```

For a self-contained shell test, create a temporary session bus and keep the
server alive while the client runs:

```sh
dbus-run-session -- sh -c './build/vehicle_server & server_pid=$!; sleep 1; ./build/vehicle_client; status=$?; kill $server_pid; wait $server_pid 2>/dev/null; exit $status'
```

## D-Bus interface

The XML file defines one object and one interface:

| Item | Value |
| --- | --- |
| Bus name | `com.labprob.VehicleMonitor` |
| Object path | `/com/labprob/VehicleMonitor` |
| Interface | `com.labprob.VehicleMonitor` |

Methods:

| Method | Return value |
| --- | --- |
| `GetCpuUsage` | `usage` (`d`, double) |
| `GetMemoryUsage` | `usage` (`d`, double) |
| `GetUptime` | `uptime` (`s`, string) |
| `GetVehicleSpeed` | `speed` (`i`, 32-bit signed integer) |

The interface also declares `CpuUsageChanged` (`d`), `MemoryUsageChanged`
(`d`), and `VehicleSpeedChanged` (`i`) signals. The current mock service does
not emit them. It defines no D-Bus properties and none of its methods accept
input arguments.

## Development

`vehicle.xml` is the source of truth for the D-Bus method and signal contract.
Do not edit the generated headers. After changing the XML, rebuild the bindings
and update the corresponding `VehicleService` overrides and proxy use.

There is currently no unit-test framework or CTest suite. At minimum, rebuild
both executables and run the session-bus command above to validate their
end-to-end interaction. The programs report connection and D-Bus call failures
to standard error and exit with a nonzero status.
