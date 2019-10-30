cmake_minimum_required(VERSION 3.9)

add_definitions(-std=c++11)
add_definitions(-g)

# 多线程
find_package(Threads)

include_directories(/usr/local/include)

# 添加生成对象
file(GLOB_RECURSE SAMPLELIST RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src/" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

foreach(curr_file ${SAMPLELIST})
    string( REPLACE ".cpp" "" program ${curr_file})
    # 将动态库链接到目标
    add_executable(${program} ${CMAKE_CURRENT_SOURCE_DIR}/src/${curr_file})
    target_link_libraries(${program}
        ${CMAKE_THREAD_LIBS_INIT} 
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libpaho-mqtt3c.so)
endforeach(curr_file)