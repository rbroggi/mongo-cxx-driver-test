cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif()

project(HELLO_WORLD LANGUAGES C CXX)

# Enforce the C++ standard, and disable extensions.
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

# NOTE: For this to work, the CMAKE_PREFIX_PATH variable must be set to point to the directory that
# was used as the argument to CMAKE_INSTALL_PREFIX when building libbsoncxx.
find_package(libbsoncxx REQUIRED)
find_package(libmongocxx REQUIRED)

message("LIBMONGOCXX_INCLUDE_DIRS: ${LIBMONGOCXX_INCLUDE_DIRS}")
message("LIBMONGOCXX_LIBRARIES: ${LIBMONGOCXX_LIBRARIES}")
message("LIBMONGOCXX_DEFINITIONS: ${LIBMONGOCXX_DEFINITIONS}")

add_executable(mongo_test mongo_test.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    find_package(Boost 1.56.0 REQUIRED)
    target_include_directories(mongo_test
      PRIVATE ${Boost_INCLUDE_DIRS}
    )
endif()

target_include_directories(mongo_test
  PRIVATE ${LIBBSONCXX_INCLUDE_DIRS}
  PRIVATE ${LIBMONGOCXX_INCLUDE_DIRS}
)

target_link_libraries(mongo_test
  PRIVATE ${LIBBSONCXX_LIBRARIES}
  PRIVATE ${LIBMONGOCXX_LIBRARIES}
)

target_compile_definitions(mongo_test
  PRIVATE ${LIBBSONCXX_DEFINITIONS}
  PRIVATE ${LIBMONGOCXX_DEFINITIONS}
)

add_custom_target(run
    COMMAND mongo_test
    DEPENDS mongo_test
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

# Sanity-check that static library macros are not set when building against the shared library.
# Users don't need to include this section in their projects.
list(FIND LIBBSONCXX_DEFINITIONS "BSONCXX_STATIC" LIST_IDX)
if (${LIST_IDX} GREATER -1)
    message(FATAL_ERROR "Expected BSONCXX_STATIC to not be defined")
endif()

