cmake_minimum_required(VERSION 3.31)
project(Voxl)

set(CMAKE_CXX_STANDARD 20)

# Ensure vcpkg toolchain is loaded before the project() call
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# Gather headers and sources
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(GLAD_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/src/glad.c")
set(STB_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb_image.cpp")

# Add the executable
add_executable(Voxl ${HEADERS} ${SOURCE} ${GLAD_SOURCE} ${STB_SOURCE})

# Include directories
target_include_directories(Voxl PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/noise/"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include"
        "${VCPKG_INCLUDE_DIRS}"
)

# Dependencies via vcpkg
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Platform-specific OpenGL linking
if(WIN32)
    target_link_libraries(Voxl PRIVATE opengl32)
elseif(APPLE)
    target_link_libraries(Voxl PRIVATE "-framework OpenGL")
endif()

# Final link targets
target_link_libraries(Voxl
        PRIVATE
        OpenGL::GL
        glfw
        glm::glm
        spdlog::spdlog
)