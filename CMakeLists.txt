cmake_minimum_required(VERSION 3.22)

PROJECT(libice_simplify VERSION 1.0.0 LANGUAGES CXX)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED TRUE)

MESSAGE(STATUS "===== ${PROJECT_NAME} ====")
MESSAGE(STATUS "--------------------------------------------------")

# 设置 CMAKE_BUILD_TYPE
IF ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

#设置Release版本的编译参数
SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# 编译器参数
IF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    ADD_COMPILE_OPTIONS(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
    IF ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_COMPILE_OPTIONS(-g)
    ENDIF()
    IF ("${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS 9)
        SET(GCC8OL TRUE)
    ENDIF()
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    # note: not working for current version because C++17 support is poor
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -inline-forceinline")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    ADD_COMPILE_OPTIONS(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
    IF ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_COMPILE_OPTIONS(-g)
    ELSEIF ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        # C99 是允许使用的，但 Clang 依然可能会给出警告，因此屏蔽
        ADD_COMPILE_OPTIONS(-Wno-c99-extensions)
    ENDIF()
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    ADD_COMPILE_OPTIONS(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
    IF ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        ADD_COMPILE_OPTIONS(-g)
    ELSEIF ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        # C99 是允许使用的，但 Clang 依然可能会给出警告，因此屏蔽
        ADD_COMPILE_OPTIONS(-Wno-c99-extensions)
    ENDIF()
ELSE()
    MESSAGE(WARNING "Unsupported CXX compiler ${CMAKE_CXX_COMPILER_ID}")
ENDIF()

# 通用外部库依赖导入函数
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
INCLUDE(${CMAKE_SOURCE_DIR}/cmake/PrintFlags.cmake)

# glog
FIND_PACKAGE(libglog REQUIRED)
PRINT_LIB_INFO("libglog" "${libglog_INCLUDE_DIRS}" "${libglog_LIBRARIES}")

# libIce
FIND_PACKAGE(Ice REQUIRED COMPONENTS Ice++11)
PRINT_LIB_INFO("libIce" "${Ice_INCLUDE_DIRS}" "${Ice_LIBRARIES}")

FILE(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/slice")
FILE(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/stub")

# 将所有 slice/*.ice 文件列出来
FILE(GLOB SLICE_SOURCES ${CMAKE_SOURCE_DIR}/slice/*.ice)

ADD_DEFINITIONS(-DICE_CPP11_MAPPING)

FOREACH(SLICE_SOURCE ${SLICE_SOURCES})
    GET_FILENAME_COMPONENT(SLICE_FILE_NAME ${SLICE_SOURCE} NAME_WE)

    LIST(APPEND STUB_SOURCES "${CMAKE_SOURCE_DIR}/stub/${SLICE_FILE_NAME}.h")
    LIST(APPEND STUB_SOURCES "${CMAKE_SOURCE_DIR}/stub/${SLICE_FILE_NAME}.cpp")
ENDFOREACH()

# include 3rd headers
INCLUDE_DIRECTORIES(${Ice_INCLUDE_DIRS} ${libglog_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/stub ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/3rd/include)

# windows 下额外的库文件
IF (WIN32)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES .dll .dll.a .a)
    FIND_LIBRARY(WSOCk "wsock32")
    FIND_LIBRARY(IPHLPAPI "iphlpapi")

    PRINT_LIB_INFO("Win32lib" "" "${WSOCk} ${IPHLPAPI}")
ENDIF()

# 打包
SET(CPACK_GENERATOR "ZIP")
SET(CPACK_ARCHIVE_COMPONENT_INSTALL ON)

IF (NOT "${PACKAGE_NAME_SUFFIX}" STREQUAL "")
    MESSAGE(STATUS "Append '${PACKAGE_NAME_SUFFIX}' to the package file name")
    SET(CPACK_ARCHIVE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${PACKAGE_NAME_SUFFIX}")
ENDIF()

INCLUDE(CPack)

MESSAGE(STATUS "--------------------------------------------------")

ADD_SUBDIRECTORY(server_tests)
ADD_SUBDIRECTORY(client_tests)


