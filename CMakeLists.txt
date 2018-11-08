cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(POLICY CMP0025)
    cmake_policy(SET CMP0025 NEW)
endif()

project(mongo-test LANGUAGES C CXX)

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

#Add here new executable files
set(EXECUTABLES_SRC
    mongo_test.cpp
    aggregate.cpp
)

foreach(EXECUTABLE_SRC ${EXECUTABLES_SRC})
    get_filename_component(EXAMPLE_TARGET ${EXECUTABLE_SRC} NAME_WE)
    add_executable(${EXAMPLE_TARGET} ${EXECUTABLE_SRC})
    target_include_directories(${EXAMPLE_TARGET}
            PRIVATE ${LIBBSONCXX_INCLUDE_DIRS}
            PRIVATE ${LIBMONGOCXX_INCLUDE_DIRS}
            )
    target_link_libraries(${EXAMPLE_TARGET}
            PRIVATE ${LIBBSONCXX_LIBRARIES}
            PRIVATE ${LIBMONGOCXX_LIBRARIES}
            )
    target_compile_definitions(${EXAMPLE_TARGET}
            PRIVATE ${LIBBSONCXX_DEFINITIONS}
            PRIVATE ${LIBMONGOCXX_DEFINITIONS}
            )
    set(MONGOCXX_EXAMPLE_EXECUTABLES ${MONGOCXX_EXAMPLE_EXECUTABLES} ${EXAMPLE_TARGET})
endforeach(EXECUTABLE_SRC)


# Sanity-check that static library macros are not set when building against the shared library.
# Users don't need to include this section in their projects.
list(FIND LIBBSONCXX_DEFINITIONS "BSONCXX_STATIC" LIST_IDX)
if (${LIST_IDX} GREATER -1)
    message(FATAL_ERROR "Expected BSONCXX_STATIC to not be defined")
endif()

