cmake_minimum_required(VERSION 3.16)

project(vehicle_monitor_service VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================
# Options
# =========================

option(BUILD_CLIENT "Build DBus client" ON)
option(BUILD_SERVER "Build DBus server" ON)

# =========================
# Dependencies
# =========================

find_package(PkgConfig REQUIRED)

pkg_check_modules(SDBUS REQUIRED sdbus-c++)

# =========================
# Include directories
# =========================

include_directories(
    ${SDBUS_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)

link_directories(
    ${SDBUS_LIBRARY_DIRS}
)

# =========================
# DBus XML generation
# =========================

set(DBUS_XML
    ${CMAKE_SOURCE_DIR}/dbus/xml/vehicle.xml
)

set(GENERATED_DIR
    ${CMAKE_BINARY_DIR}/generated
)

file(MAKE_DIRECTORY ${GENERATED_DIR})

add_custom_command(
    OUTPUT
        ${GENERATED_DIR}/vehicle-server-glue.h
        ${GENERATED_DIR}/vehicle-client-glue.h

    COMMAND sdbus-c++-xml2cpp
        ${DBUS_XML}
        --adaptor=${GENERATED_DIR}/vehicle-server-glue.h
        --proxy=${GENERATED_DIR}/vehicle-client-glue.h

    DEPENDS ${DBUS_XML}

    COMMENT "Generating DBus bindings from XML"
)

add_custom_target(generate_dbus_bindings DEPENDS
    ${GENERATED_DIR}/vehicle-server-glue.h
    ${GENERATED_DIR}/vehicle-client-glue.h
)

include_directories(${GENERATED_DIR})

# =========================
# Server
# =========================

if(BUILD_SERVER)

    add_executable(vehicle_server
        src/server/main.cpp
        src/services/VehicleService.cpp
    )

    target_include_directories(vehicle_server PRIVATE
        ${CMAKE_SOURCE_DIR}/src/services
    )

    add_dependencies(vehicle_server generate_dbus_bindings)

    target_link_libraries(vehicle_server
        ${SDBUS_LIBRARIES}
    )

endif()

# =========================
# Client
# =========================

if(BUILD_CLIENT)

    add_executable(vehicle_client
        src/client/main.cpp
    )

    add_dependencies(vehicle_client generate_dbus_bindings)

    target_link_libraries(vehicle_client
        ${SDBUS_LIBRARIES}
    )

endif()
