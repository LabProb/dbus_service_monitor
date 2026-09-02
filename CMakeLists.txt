cmake_minimum_required(VERSION 3.16)

project(vehicle_monitor_service VERSION 1.0 LANGUAGES CXX)

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
pkg_check_modules(SDBUS REQUIRED IMPORTED_TARGET sdbus-c++)
find_program(SDBUS_XML2CPP_EXECUTABLE sdbus-c++-xml2cpp REQUIRED)

# =========================
# DBus XML generation
# =========================

set(DBUS_XML
    ${CMAKE_SOURCE_DIR}/dbus/xml/vehicle.xml
)

set(GENERATED_DIR
    ${CMAKE_BINARY_DIR}/generated
)

add_custom_command(
    OUTPUT
        ${GENERATED_DIR}/vehicle-server-glue.h
        ${GENERATED_DIR}/vehicle-client-glue.h

    COMMAND ${CMAKE_COMMAND} -E make_directory ${GENERATED_DIR}
    COMMAND ${SDBUS_XML2CPP_EXECUTABLE}
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

function(configure_vehicle_target target)
    target_compile_features(${target} PRIVATE cxx_std_20)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    target_include_directories(${target} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/services
        ${GENERATED_DIR}
    )
    target_link_libraries(${target} PRIVATE PkgConfig::SDBUS)
    add_dependencies(${target} generate_dbus_bindings)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

# =========================
# Server
# =========================

if(BUILD_SERVER)

    add_executable(vehicle_server
        src/server/main.cpp
        src/services/VehicleService.cpp
    )

    configure_vehicle_target(vehicle_server)

endif()

# =========================
# Client
# =========================

if(BUILD_CLIENT)

    add_executable(vehicle_client
        src/client/main.cpp
    )

    configure_vehicle_target(vehicle_client)

endif()
