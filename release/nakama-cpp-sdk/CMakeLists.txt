
cmake_minimum_required(VERSION 3.6)

set(ROOT_LIBS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/libs")

if(ANDROID)
    if("${ANDROID_STL}" STREQUAL "c++_static")
        set(LIBS_FOLDER "${ROOT_LIBS_FOLDER}/android/${ANDROID_ABI}")
    else()
        message(FATAL_ERROR "nakama-cpp: only c++_static STL is supported")
    endif()
elseif(WIN32)
    # vc140 - Visual Studio 2015
    # vc141 - Visual Studio 2017
    set(LIBS_FOLDER "${ROOT_LIBS_FOLDER}/win32/vc141")
elseif(APPLE AND NOT IOS_PLATFORM)
    # Mac
    set(LIBS_FOLDER "${ROOT_LIBS_FOLDER}/mac")
elseif(UNIX AND NOT APPLE)
    # Linux
    set(LIBS_FOLDER "${ROOT_LIBS_FOLDER}/linux/x86")
endif()

set(LIB_PREFIX "")
set(LIB_SUFFIX "")
set(LIB_EXT "")

if(WIN32)
    if(CMAKE_BUILD_TYPE EQUAL "Debug")
        set(LIB_SUFFIX "d")
    endif()
    set(LIB_EXT ".lib")
else()
    # Android, Mac, iOS, Linux
    set(LIB_PREFIX "lib")
    set(LIB_EXT ".a")
endif()

# grpc++
project(grpc++)
add_library(grpc++ STATIC IMPORTED GLOBAL)
set_target_properties(grpc++ PROPERTIES
    IMPORTED_LOCATION "${LIBS_FOLDER}/${LIB_PREFIX}grpc++${LIB_SUFFIX}${LIB_EXT}"
)

# grpc
project(grpc)
add_library(grpc STATIC IMPORTED GLOBAL)
set_target_properties(grpc PROPERTIES
    IMPORTED_LOCATION "${LIBS_FOLDER}/${LIB_PREFIX}grpc${LIB_SUFFIX}${LIB_EXT}"
)

# protobuf
project(protobuf)
add_library(protobuf STATIC IMPORTED GLOBAL)
set_target_properties(protobuf PROPERTIES
    IMPORTED_LOCATION "${LIBS_FOLDER}/${LIB_PREFIX}protobuf${LIB_SUFFIX}${LIB_EXT}"
)

# cares
project(cares)
add_library(cares STATIC IMPORTED GLOBAL)
set_target_properties(cares PROPERTIES
    IMPORTED_LOCATION "${LIBS_FOLDER}/${LIB_PREFIX}cares${LIB_SUFFIX}${LIB_EXT}"
)

# address_sorting
project(address_sorting)
add_library(address_sorting STATIC IMPORTED GLOBAL)
set_target_properties(address_sorting PROPERTIES
    IMPORTED_LOCATION "${LIBS_FOLDER}/${LIB_PREFIX}address_sorting${LIB_SUFFIX}${LIB_EXT}"
)

# gpr
project(gpr)
add_library(gpr STATIC IMPORTED GLOBAL)
set_target_properties(gpr PROPERTIES
    IMPORTED_LOCATION "${LIBS_FOLDER}/${LIB_PREFIX}gpr${LIB_SUFFIX}${LIB_EXT}"
)

# ssl
project(ssl)
add_library(ssl STATIC IMPORTED GLOBAL)
set_target_properties(ssl PROPERTIES
    IMPORTED_LOCATION "${LIBS_FOLDER}/${LIB_PREFIX}ssl${LIB_SUFFIX}${LIB_EXT}"
)

# crypto
project(crypto)
add_library(crypto STATIC IMPORTED GLOBAL)
set_target_properties(crypto PROPERTIES
    IMPORTED_LOCATION "${LIBS_FOLDER}/${LIB_PREFIX}crypto${LIB_SUFFIX}${LIB_EXT}"
)

# nakama-cpp
set(lib_name nakama-cpp)
set(target_name ext_${lib_name})

project(${lib_name})

add_library(${target_name} STATIC IMPORTED GLOBAL)

set_target_properties(${target_name} PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set_target_properties(${target_name} PROPERTIES
    IMPORTED_LOCATION "${LIBS_FOLDER}/${LIB_PREFIX}${lib_name}${LIB_SUFFIX}${LIB_EXT}"
)

target_link_libraries(${target_name} INTERFACE grpc++ grpc protobuf cares address_sorting gpr ssl crypto)