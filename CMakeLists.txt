cmake_minimum_required(VERSION 3.10)  
project(JSQt)  
  
# 找到QuickJS的头文件和库文件  
# 假设QuickJS的头文件位于/path/to/quickjs/include，库文件位于/path/to/quickjs/lib  
set(QUICKJS_INCLUDE_DIR "/usr/local/QuickJS/include/quickjs")  
set(QUICKJS_LIBRARY_DIR "/usr/local/QuickJS/lib/quickjs")
  
# 对于库文件，我们还需要找到具体的库文件名称（可能包含前缀和后缀）  
# 这里我们假设库文件名为libquickjs.a或libquickjs.so（取决于系统和构建方式）  
# 在Windows上可能是quickjs.dll或quickjs.lib，但这里我们主要处理类Unix系统  
find_library(QUICKJS_LIBRARIES NAMES libquickjs.a libquickjs.so  
             PATHS ${QUICKJS_LIBRARY_DIR}
             NO_DEFAULT_PATH)
  
# 检查是否找到了库  
if(NOT QUICKJS_LIBRARIES)
    message(FATAL_ERROR "Could not find the QuickJS library in ${QUICKJS_LIBRARY_DIR}")  
endif()
  
# 添加你的源文件  
add_executable(${PROJECT_NAME} a.cpp) # 假设你的主源文件是main.cpp  
  
# 为可执行文件指定QuickJS的头文件目录  
target_include_directories(${PROJECT_NAME} PRIVATE ${QUICKJS_INCLUDE_DIR})  
  
# 链接QuickJS库到你的可执行文件  
# 注意：这里我们直接使用QUICKJS_LIBRARIES变量，它应该包含库文件的完整路径  
target_link_libraries(${PROJECT_NAME} PRIVATE ${QUICKJS_LIBRARIES}) 