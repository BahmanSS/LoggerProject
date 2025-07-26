set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(LOGGER_SRC
    ${SRC_DIR}/FileLogger.cpp
    ${SRC_DIR}/LogLevel.cpp
)

set(LOGGER_INCLUDE
    ${INCLUDE_DIR}/FileLogger.hpp
    ${INCLUDE_DIR}/Logger.hpp
    ${INCLUDE_DIR}/LogLevel.hpp
 )

#include_directories(${INCLUDE_DIR})

# Статическая библиотека.
add_library(logger_static STATIC ${LOGGER_SRC} ${LOGGER_INCLUDE})
set_target_properties(logger_static PROPERTIES OUTPUT_NAME "logger")

# Динамическая библиотека.
add_library(logger_shared SHARED ${LOGGER_SRC} ${LOGGER_INCLUDE})
set_target_properties(logger_shared PROPERTIES OUTPUT_NAME "logger")

target_include_directories(logger_static PUBLIC ${INCLUDE_DIR})
target_include_directories(logger_shared PUBLIC ${INCLUDE_DIR})