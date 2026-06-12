cmake_minimum_required(VERSION 3.20)
project(OrderBook)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(orderbook
    src/main.cc
    src/order_book.cc
    src/data_feed.cc
)

target_include_directories(orderbook PRIVATE include/)

find_package(Threads REQUIRED)
target_link_libraries(orderbook Threads::Threads)

find_package(nlohmann_json REQUIRED)
target_link_libraries(orderbook nlohmann_json::nlohmann_json)
