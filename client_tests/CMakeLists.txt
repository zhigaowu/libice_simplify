cmake_minimum_required(VERSION 3.22)

PROJECT(client_tests VERSION 1.0.0 LANGUAGES CXX)

# 打印项目信息
MESSAGE(STATUS "===== ${PROJECT_NAME} ====")
MESSAGE(STATUS "--------------------------------------------------")
MESSAGE(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# 打印编译标志
PRINT_CPLUS_FLAGS()

# 打印空行，结束项目信息
MESSAGE(STATUS "--------------------------------------------------")

# 生成stub/*.cpp文件
ADD_CUSTOM_COMMAND(
    OUTPUT ${STUB_SOURCES}
    COMMAND ${Ice_SLICE2CPP_EXECUTABLE} --output-dir stub ${SLICE_SOURCES}
    DEPENDS ${SLICE_SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating stub sources ...")

# 将所有 src/*.cpp 文件列出来
file(GLOB SIMPLIFY_SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB MAIN_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# 所有的源文件+头文件需要在这个地方添加
ADD_EXECUTABLE(client_tests ${SIMPLIFY_SOURCES} ${STUB_SOURCES} ${MAIN_SOURCES})

TARGET_LINK_LIBRARIES(client_tests PRIVATE ${Ice_LIBRARIES})
TARGET_LINK_LIBRARIES(client_tests PRIVATE ${libglog_LIBRARIES} pthread)

IF (WIN32)
    TARGET_LINK_LIBRARIES(client_tests PRIVATE wsock32 ws2_32 iphlpapi)
ENDIF()

install(TARGETS client_tests
    RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/install COMPONENT client_tests)

