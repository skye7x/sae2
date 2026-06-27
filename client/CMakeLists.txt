cmake_minimum_required(VERSION 3.10)
project(SAE2Client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(sae2_client main.cpp)

# --- Find and link libcurl ---
find_package(CURL REQUIRED)
target_include_directories(sae2_client PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(sae2_client PRIVATE ${CURL_LIBRARIES})

# --- Find or fetch nlohmann/json ---
# Try system install first (e.g. apt install nlohmann-json3-dev)
find_package(nlohmann_json QUIET)
if(nlohmann_json_FOUND)
  target_link_libraries(sae2_client PRIVATE nlohmann_json::nlohmann_json)
else()
  # Fall back to FetchContent if not installed
  include(FetchContent)
  FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
  )
  FetchContent_MakeAvailable(nlohmann_json)
  target_link_libraries(sae2_client PRIVATE nlohmann_json::nlohmann_json)
endif()
