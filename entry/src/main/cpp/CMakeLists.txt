# the minimum version of CMake.
cmake_minimum_required(VERSION 3.5.0)
project(TestProject)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})



if(DEFINED PACKAGE_FIND_FILE)
    include(${PACKAGE_FIND_FILE})
endif()

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include)


set(LIB_SRC
    napi_init.cpp
    hello.cpp
    cusSum.cpp
    )

add_library(entry SHARED ${LIB_SRC})
target_link_libraries(entry PUBLIC libace_napi.z.so)