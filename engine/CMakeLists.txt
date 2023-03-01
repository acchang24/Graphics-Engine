file(GLOB source_file CONFIGURE_DEPENDS "*.cpp" "*.h" "*.hpp" "*.c")

add_executable(engine ${source_file})