cmake_minimum_required(VERSION 3.16)
project(SimpleWebServer VERSION 0.1 LANGUAGES CXX)

message(STATUS "Project: ${PROJECT_NAME} (version ${CMAKE_PROJECT_VERSION})")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMake Generator: ${CMAKE_GENERATOR}")

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_EXTENSIONS ON) 
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES RELATIVE ${PROJECT_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE HEADERS RELATIVE ${PROJECT_SOURCE_DIR} "include/*.h")
file(GLOB_RECURSE WRAPPERS RELATIVE ${PROJECT_SOURCE_DIR} "include/*.hpp")
file(COPY web/ DESTINATION "${EXECUTABLE_OUTPUT_PATH}")

set(PROJECT_SOURCES
        ${SOURCES}
        ${HEADERS}
        ${WRAPPERS}
)

find_package(Boost REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost Library not found !")
endif()

if(WIN32)
    include_directories(${PROJECT_SOURCE_DIR}/mysql-connector/include/jdbc)
    link_directories(${PROJECT_SOURCE_DIR}/mysql-connector/lib64)
    link_directories(${PROJECT_SOURCE_DIR}/mysql-connector/lib64/vs14)
else()
    find_package(mysql-connector-c++ REQUIRED)
endif()

add_executable(
    ${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

# Link Boost Libraries
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ${Boost_LIBRARIES}
)

# Link MySQL Libraries
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    mysqlcppconn
    libssl
    libcrypto
)

if(WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        ws2_32
    )
endif()
