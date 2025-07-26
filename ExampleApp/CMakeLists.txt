add_executable(app main.cc)

target_link_libraries(app PRIVATE logger_static)
