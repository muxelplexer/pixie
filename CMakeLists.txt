cmake_minimum_required(VERSION 3.29)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
include(FetchContent)
project(pixie
    VERSION 0.0.1
)

find_package(SDL3 REQUIRED)

FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.0.0"
    GIT_SHALLOW ON
    SYSTEM
)
FetchContent_MakeAvailable(GSL)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY git@github.com:ocornut/imgui.git
    GIT_TAG v1.91.0
    GIT_SHALLOW ON
    SYSTEM
)
FetchContent_MakeAvailable(imgui)
FetchContent_GetProperties(imgui SOURCE_DIR IMGUI_DIR)
add_library(imgui STATIC
         ${imgui_SOURCE_DIR}/imgui.cpp
         ${imgui_SOURCE_DIR}/imgui_draw.cpp
         ${imgui_SOURCE_DIR}/imgui_demo.cpp
         ${imgui_SOURCE_DIR}/imgui_widgets.cpp
         ${imgui_SOURCE_DIR}/imgui_tables.cpp
         ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
         ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

add_executable(pixie
    src/main.cpp
    src/sdl/window.cpp
    src/ui/ui.cpp
    src/ui/widgets/fps_counter.cpp
)

target_link_libraries(pixie PRIVATE
    Microsoft.GSL::GSL
    imgui
    SDL3
)

target_compile_features(pixie PRIVATE 
    cxx_std_23
    c_std_23
)

set_target_properties(pixie PROPERTIES
    CXX_EXTENSIONS FALSE
    INTERPROCEDURAL_OPTIMIZATION TRUE
)

target_include_directories(pixie PRIVATE inc/)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}")
set_target_properties(pixie PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

target_compile_options(pixie PRIVATE
    -Wall -Wextra -Wpedantic -Wshadow -Weffc++ -Wconversion -Wsign-conversion -fms-extensions
)
target_compile_definitions(pixie
    PRIVATE GLFW_INCLUDE_NONE
)


