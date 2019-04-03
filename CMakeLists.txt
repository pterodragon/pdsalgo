cmake_minimum_required(VERSION 3.5)
project(pdsalgo VERSION 1.0.0 LANGUAGES CXX)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

add_library(pdsalgo INTERFACE)
target_compile_features(pdsalgo INTERFACE cxx_std_17)

target_include_directories(pdsalgo INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:pdsalgo/include>
)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR pdsalgo/${CMAKE_INSTALL_LIBDIR}/cmake/pdsalgo)

install(TARGETS pdsalgo
    EXPORT pdsalgo-targets
    LIBRARY DESTINATION pdsalgo/${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION pdsalgo/${CMAKE_INSTALL_LIBDIR}
)

set_target_properties(pdsalgo PROPERTIES EXPORT_NAME pdsalgo)
install(DIRECTORY include/ DESTINATION pdsalgo/${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT pdsalgo-targets
  FILE
    pdsalgo-targets.cmake
  NAMESPACE
    pdsalgo::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/pdsalgo-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/pdsalgo-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/pdsalgo-config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/pdsalgo-config-version.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/pdsalgo-config.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

export(EXPORT pdsalgo-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/pdsalgo-targets.cmake NAMESPACE pdsalgo::)

export(PACKAGE pdsalgo)

if (${BUILD_TESTS})
    enable_testing()
    add_subdirectory(test)
endif()

