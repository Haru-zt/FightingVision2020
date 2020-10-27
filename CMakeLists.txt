# CMakeList.txt:FYT2021 的 CMake 项目，在此处包括源代码并定义项目特定的逻辑。

cmake_minimum_required (VERSION 3.5)

project(FightingVision2020)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${CMAKE_SYSTEM_NAME}")

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(OpenCV_DIR D:/opencv/build)
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
endif()

find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/Camera/Include)

set(SRC
    "FightingVision2020.cpp"
    
    "Camera/FightingDahengCapture.cpp"
    "Camera/FightingVideoCapture.cpp"
    "Camera/FightingUSBCapture.cpp"
    "Armor/ArmorDetector.cpp"
)

add_executable (FightingVision2020 ${SRC})

target_link_libraries( FightingVision2020 ${OpenCV_LIBS} )

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_link_libraries( FightingVision2020 "${PROJECT_SOURCE_DIR}/Camera/lib/x64/GxIAPI.lib" )
    target_link_libraries( FightingVision2020 "${PROJECT_SOURCE_DIR}/Camera/lib/x64/DxImageProc.lib" )
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries( FightingVision2020 "libgxiapi.so" )  
endif()
