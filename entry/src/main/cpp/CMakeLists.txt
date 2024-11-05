# the minimum version of CMake.
cmake_minimum_required(VERSION 3.5.0)
project(TestProject)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})


set(CMAKE_BUILD_TYPE Debug) # 设置为调试模式
set(CMAKE_CXX_FLAGS_DEBUG "-g") # 启用调试符号



if(DEFINED PACKAGE_FIND_FILE)
    include(${PACKAGE_FIND_FILE})
endif()

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include)


## 设置 OpenSSL 库路径
#if(${OHOS_ARCH} STREQUAL "arm64-v8a")
#    set(OPENSSL_DIR ../ohos-openssl)
#elseif(${OHOS_ARCH} STREQUAL "armeabi-v7a")
#    set(OPENSSL_DIR $ENV{ARMV7_UNKNOWN_LINUX_OHOS_OPENSSL_DIR})
#elseif(${OHOS_ARCH} STREQUAL "x86_64")
#    set(OPENSSL_DIR $ENV{X86_64_UNKNOWN_LINUX_OHOS_OPENSSL_DIR})
#endif()

set(OPENSSL_DIR ${NATIVERENDER_ROOT_PATH}/ohos-openssl/prelude/${OHOS_ARCH}/lib)


set(LIB_SRC
    napi_init.cpp
    hello.cpp
    httpRequest.cpp
    )



add_library(entry SHARED ${LIB_SRC})

#set(LIB_LIB
#    ${NATIVERENDER_ROOT_PATH}/../../../libs/${OHOS_ARCH}/openssl/openssl.a
##    ${NATIVERENDER_ROOT_PATH}/../../../libs/${OHOS_ARCH}/openssl/libssl.a
#)
#
#add_library(openssl_3 SHARED IMPORTED)
#set_target_properties(openssl_3
#    PROPERTIES
#    IMPORTED_LOCATION ${NATIVERENDER_ROOT_PATH}/../../../libs/${OHOS_ARCH}/openssl/openssl.a
#)



target_link_libraries(entry PUBLIC libace_napi.z.so ${OPENSSL_DIR}/libssl.a ${OPENSSL_DIR}/libcrypto.a  libhilog_ndk.z.so libohhidebug.so)
#target_link_libraries(entry PUBLIC ${LIB_LIB})