# the minimum version of CMake.
cmake_minimum_required(VERSION 3.5.0)
project(TestProject)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})


set(CMAKE_BUILD_TYPE Debug) # 设置为调试模式
set(CMAKE_CXX_FLAGS_DEBUG "-g") # 启用调试符号



if(DEFINED PACKAGE_FIND_FILE)
    include(${PACKAGE_FIND_FILE})
endif()

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include
#                    ${NATIVERENDER_ROOT_PATH}/3rd/libcurl/include
                    ${NATIVERENDER_ROOT_PATH}/3rd/openssl/include
                    )

set(LIB_SRC
    napi_init.cpp
    hello.cpp
    httpRequest.cpp
    )



add_library(entry SHARED ${LIB_SRC})

set(ZLIB_DIR ${NATIVERENDER_ROOT_PATH}/3rd/zlib/lib/${OHOS_ARCH})
set(LIBCURL_DIR ${NATIVERENDER_ROOT_PATH}/3rd/libcurl/lib/${OHOS_ARCH})
set(OPENSSL_DIR ${NATIVERENDER_ROOT_PATH}/3rd/openssl/lib/${OHOS_ARCH})
target_link_libraries(entry PUBLIC
    libace_napi.z.so
    libhilog_ndk.z.so
    libohhidebug.so
#    ${ZLIB_DIR}/libz.a
#    ${LIBCURL_DIR}/libcurl.a
    ${OPENSSL_DIR}/openssl_gm.a
#    ${OPENSSL_DIR}/libssl.a
#    ${OPENSSL_DIR}/libcrypto.a
)
#target_link_libraries(entry PUBLIC ${LIB_LIB})