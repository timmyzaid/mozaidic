cmake_minimum_required(VERSION 3.5.1)
project(ImageService)
set(CMAKE_CXX_STANDARD 14)

find_package(aws-sdk-cpp REQUIRED)
add_definitions(-DUSE_IMPORT_EXPORT)

find_package(Boost 1.61.0 COMPONENTS system REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(VIPS REQUIRED glib-2.0 vips-cpp)

include_directories(includes ${Boost_INCLUDE_DIRS} ${VIPS_INCLUDE_DIRS})
link_directories(${VIPS_LIBRARY_DIRS})
file(GLOB SOURCES "sources/*.cpp")
add_executable(run ${SOURCES})
add_definitions(${VIPS_CFLAGS_OTHER})
target_link_libraries(run aws-cpp-sdk-s3 aws-cpp-sdk-transfer ${Boost_LIBRARIES} ${VIPS_LIBRARIES})