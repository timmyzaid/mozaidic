cmake_minimum_required(VERSION 3.5.1)
project(ImageService)
set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost 1.61.0 COMPONENTS system REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(VIPS REQUIRED glib-2.0 vips-cpp)

find_package(Threads REQUIRED)

include_directories(includes ${Boost_INCLUDE_DIRS} ${VIPS_INCLUDE_DIRS})
link_directories(${VIPS_LIBRARY_DIRS})
file(GLOB SOURCES "sources/*.cpp")
add_executable(run ${SOURCES})
add_definitions(${VIPS_CFLAGS_OTHER})
target_link_libraries(run Threads::Threads ${VIPS_LIBRARIES} ${Boost_LIBRARIES})