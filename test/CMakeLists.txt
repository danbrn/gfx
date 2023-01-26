cmake_minimum_required(VERSION 3.14)

project(gfxTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(gfx REQUIRED)
  enable_testing()
endif()

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(gfx_test src/gfx_test.cpp)
target_link_libraries(
    gfx_test PRIVATE
    gfx::gfx
    Catch2::Catch2WithMain
)
target_compile_features(gfx_test PRIVATE cxx_std_20)

target_include_directories(gfx_test ${warning_guard}
                           PUBLIC
                           "${SDL2_INCLUDE_DIRS}"
                           "${SDL2_IMAGE_INCLUDE_DIRS}"
                           "${SDL2__TTFINCLUDE_DIRS}")
                           
catch_discover_tests(gfx_test)

# ---- End-of-file commands ----

add_folders(Test)
