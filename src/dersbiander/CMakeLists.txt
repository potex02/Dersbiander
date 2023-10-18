file(GLOB_RECURSE SOURCES "*.cpp" "*.h" "*.hpp")
add_executable(dersbiander ${SOURCES})

target_link_libraries(
  dersbiander
  PRIVATE Dersbiander::Dersbiander_options
          Dersbiander::Dersbiander_warnings)

target_link_system_libraries(
  dersbiander
  PRIVATE
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog)

target_include_directories(dersbiander PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
