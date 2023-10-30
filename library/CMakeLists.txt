cmake_minimum_required(VERSION 3.15)
project(hyperscan_regex_cpp)
set(CMAKE_CXX_STANDARD 14)
#add_link_options("-static-libgcc")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_link_options("-static-libstdc++")
find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../hyperscan/build/install/lib/pkgconfig")
pkg_check_modules(HPS REQUIRED libhs)
add_library(hsc SHARED hyperscan_cpp.h hyperscan_cpp.cpp)
target_link_directories(hsc PRIVATE ${HPS_LIBRARY_DIRS})
target_link_libraries(hsc PRIVATE ${HPS_LIBRARIES})
target_include_directories(hsc PRIVATE ${HPS_INCLUDE_DIRS})

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

set(hyperscan_regex_cpp_VERSION 0.0.1)

include(GenerateExportHeader)

generate_export_header(hsc)
set_property(TARGET hsc PROPERTY VERSION ${hyperscan_regex_cpp_VERSION})
set_property(TARGET hsc PROPERTY SOVERSION 0)
set_property(TARGET hsc PROPERTY
        INTERFACE_hsc_MAJOR_VERSION 0)
set_property(TARGET hsc APPEND PROPERTY
        COMPATIBLE_INTERFACE_STRING hsc_MAJOR_VERSION
)

install(TARGETS hsc EXPORT hscTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)
install(
        FILES
        hyperscan_cpp.h
        "${CMAKE_CURRENT_BINARY_DIR}/hsc_export.h"
        DESTINATION
        include
        COMPONENT
        Devel
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/hsc/hscConfigVersion.cmake"
        VERSION ${hyperscan_regex_cpp_VERSION}
        COMPATIBILITY AnyNewerVersion
)

export(EXPORT hscTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/hsc/hscTargets.cmake"
        NAMESPACE fe::
)
configure_file(cmake/hscConfig.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/hsc/hscConfig.cmake"
        COPYONLY
)

set(ConfigPackageLocation lib/cmake/hsc)
install(EXPORT hscTargets
        FILE
        hscTargets.cmake
        NAMESPACE
        fe::
        DESTINATION
        ${ConfigPackageLocation}
)
install(
        FILES
        cmake/hscConfig.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/hsc/hscConfigVersion.cmake"
        DESTINATION
        ${ConfigPackageLocation}
        COMPONENT
        Devel
)