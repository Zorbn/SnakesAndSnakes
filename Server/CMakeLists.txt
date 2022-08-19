cmake_minimum_required (VERSION 3.8)

add_executable (${CMAKE_PROJECT_NAME} "Main.cpp" "Main.h" "WebSocketController.cpp" "WebSocketController.h" "SnakeGame.cpp" "SnakeGame.h" "Snake.h" "Point.h" "RoomList.h" "RoomList.cpp" "Room.h" "Room.cpp" "Snake.cpp" "Utils.h" "Utils.cpp")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Drogon::Drogon)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()
