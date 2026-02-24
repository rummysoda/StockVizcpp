cmake_minimum_required(VERSION 3.29)

set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")

project(stockvizcpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "C:/msys64/mingw64")

set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/external/beast/boost")
include_directories(${BOOST_ROOT})

set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
set(OPENSSL_INCLUDE_DIR "C:/msys64/mingw64/include")
set(OPENSSL_CRYPTO_LIBRARY "C:/msys64/mingw64/lib/libcrypto.dll.a")
set(OPENSSL_SSL_LIBRARY "C:/msys64/mingw64/lib/libssl.dll.a")

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

include_directories(
        ${CMAKE_SOURCE_DIR}/src
        ${OPENSSL_INCLUDE_DIR}
        C:/msys64/mingw64/include
)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
add_subdirectory(external/imgui-sfml)

add_library(implot STATIC
        external/implot/implot.cpp
        external/implot/implot_items.cpp
        external/implot/implot_demo.cpp
        src/rapidcsv.h
)
target_include_directories(implot PUBLIC external/implot)
target_link_libraries(implot PUBLIC ImGui-SFML::ImGui-SFML)

add_executable(stockvizcpp
        src/main.cpp
        src/window.cpp
        src/window.h
        src/apiclient.cpp
        src/apiclient.h
        src/websocketclient.cpp
        src/websocketclient.h
        src/stocksgui.cpp
        src/stocksgui.h
        src/TextButton.h
        src/AddButton.h
        src/CloseButton.h
)

target_link_libraries(stockvizcpp
        PRIVATE
        ImGui-SFML::ImGui-SFML
        implot
        sfml-graphics
        sfml-window
        sfml-system
        ${OPENSSL_SSL_LIBRARY}
        ${OPENSSL_CRYPTO_LIBRARY}
        opengl32
        winmm
        gdi32
        ws2_32
        wsock32
        crypt32
)