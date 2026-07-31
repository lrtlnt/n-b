cmake_minimum_required(VERSION 3.16)
project(XPSimulator CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 编译选项
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -fexceptions)
endif()

# 源文件列表
set(SOURCES
    src/main.cpp
    src/core/Application.cpp
    src/core/Graphics.cpp
    src/core/Event.cpp
    src/core/Window.cpp
    src/core/FileSystem.cpp
    src/ui/Widget.cpp
    src/ui/Button.cpp
    src/ui/ContextMenu.cpp
    src/ui/TaskBar.cpp
    src/ui/Desktop.cpp
    src/apps/Notepad.cpp
    src/apps/Calculator.cpp
    src/apps/Paint.cpp
    src/apps/ImageViewer.cpp
    src/apps/About.cpp
    src/apps/RunDialog.cpp
    src/apps/FileBrowser.cpp
)

# 头文件目录
include_directories(src)

# 查找依赖包
if(ANDROID)
    # Android NDK 下 SDL2 是通过预编译库提供的
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")
    find_library(SDL2_LIB SDL2)
    find_library(SDL2_IMAGE_LIB SDL2_image)
    find_library(SDL2_TTF_LIB SDL2_ttf)
    find_library(ANDROID_LOG_LIB log)
    find_library(ANDROID_LIB android)
    
    add_library(XPSimulator SHARED ${SOURCES})
    target_link_libraries(XPSimulator
        ${SDL2_LIB}
        ${SDL2_IMAGE_LIB}
        ${SDL2_TTF_LIB}
        ${ANDROID_LOG_LIB}
        ${ANDROID_LIB}
    )
else()
    # 桌面平台查找SDL2
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
    
    add_executable(XPSimulator WIN32 ${SOURCES})
    target_link_libraries(XPSimulator
        ${SDL2_LIBRARIES}
        SDL2_image::SDL2_image
        SDL2_ttf::SDL2_ttf
    )
    
    # Windows下复制SDL2 DLL到输出目录
    if(WIN32)
        add_custom_command(TARGET XPSimulator POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE:SDL2_image::SDL2_image>
            $<TARGET_FILE:SDL2_ttf::SDL2_ttf>
            $<TARGET_FILE_DIR:XPSimulator>
        )
    endif()
endif()

# 安装规则
install(TARGETS XPSimulator DESTINATION bin)
