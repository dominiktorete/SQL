cmake_minimum_required(VERSION 3.14)
project(sql_plus VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
set(CMAKE_LIBRARY_PATH "C:/msys64/mingw64/lib" ${CMAKE_LIBRARY_PATH})
set(CMAKE_INCLUDE_PATH "C:/msys64/mingw64/include" ${CMAKE_INCLUDE_PATH})
# ХАК для CMake 3.29+ (если возникает ошибка с CMakeDetermineCompileFeatures)
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.29)
    set(CMAKE_CXX_COMPILE_FEATURES "cxx_std_17" CACHE INTERNAL "")
endif()

FetchContent_Declare(
    libpqxx
    GIT_REPOSITORY https://github.com/jtv/libpqxx.git
    GIT_TAG 7.10.0
)

FetchContent_MakeAvailable(libpqxx)
add_executable(${PROJECT_NAME} main.cpp)
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_INCLUDE_PATH} ${CMAKE_LIBRARY_PATH})


target_link_libraries(${PROJECT_NAME} PRIVATE pqxx   pq)

#pacman -S mingw-w64-x86_64-libpqxx

#pacman -S mingw-w64-x86_64-postgresql
