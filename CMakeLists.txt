cmake_minimum_required(VERSION 3.10)

project(GenerateLicense)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory("${CMAKE_SOURCE_DIR}/src" pb_files)
add_executable(test ${pb_files} ${CMAKE_SOURCE_DIR}/src/test/test.cpp)
add_executable(generate_RSA ${CMAKE_SOURCE_DIR}/src/test/generate_RSA.cpp)
add_executable(generate_license ${CMAKE_SOURCE_DIR}/src/test/generate_license.cpp)
add_executable(test_license_api ${CMAKE_SOURCE_DIR}/src/test/test_license_api.cpp)
add_executable(generate_hardwarecode ${CMAKE_SOURCE_DIR}/src/test/generate_hardwarecode.cpp ${CMAKE_SOURCE_DIR}/src/hardwarecode.cpp ${CMAKE_SOURCE_DIR}/src/md5.cpp)
add_library(license_gen SHARED ${pb_files} ${CMAKE_SOURCE_DIR}/src/api/license_generate_api.cpp)
add_library(license SHARED ${pb_files} ${CMAKE_SOURCE_DIR}/src/api/license_api.cpp)

target_compile_definitions(license_gen PRIVATE -DEXPORT_LICENSE_GEN)
target_compile_definitions(license PRIVATE -DEXPORT_LICENSE)
target_link_libraries(generate_license license_gen)
target_link_libraries(generate_RSA license_gen)
target_link_libraries(test_license_api license)

include_directories(${CMAKE_SOURCE_DIR}/src)












