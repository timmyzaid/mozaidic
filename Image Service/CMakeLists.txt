cmake_minimum_required(VERSION 3.5.1)
project(ImageService)
set(CPP_STANDARD "11")

find_package(aws-sdk-cpp REQUIRED)
add_definitions(-DUSE_IMPORT_EXPORT)

find_package(OpenCV REQUIRED)

find_package(Boost 1.61.0 COMPONENTS system REQUIRED)

include_directories(includes ${Boost_INCLUDE_DIRS})
file(GLOB SOURCES "sources/*.cpp")
add_executable(run ${SOURCES})

target_link_libraries(run aws-cpp-sdk-s3)
target_link_libraries(run ${OpenCV_LIBS})
target_link_libraries(run ${Boost_LIBRARIES})