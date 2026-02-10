cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

project(stockvizcpp)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_PREFIX_PATH "C:/msys64/mingw64")

set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/beast/boost")
include_directories(${BOOST_ROOT})

set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
set(OPENSSL_INCLUDE_DIR "C:/msys64/mingw64/include")
set(OPENSSL_CRYPTO_LIBRARY "C:/msys64/mingw64/lib/libcrypto.dll.a")
set(OPENSSL_SSL_LIBRARY "C:/msys64/mingw64/lib/libssl.dll.a")

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(stockvizcpp
        main.cpp
        window.cpp
        window.h
        apiclient.cpp
        apiclient.h
        websocketclient.cpp
        websocketclient.h
)

target_include_directories(stockvizcpp PRIVATE
        ${OPENSSL_INCLUDE_DIR}
        C:/msys64/mingw64/include  # ← ADD THIS LINE for nlohmann/json
)

target_link_libraries(stockvizcpp
        PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        ${OPENSSL_SSL_LIBRARY}
        ${OPENSSL_CRYPTO_LIBRARY}
        # Boost::json  ← REMOVE THIS LINE
        opengl32
        winmm
        gdi32
        ws2_32
        wsock32
        crypt32
)