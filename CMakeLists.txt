cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    gfx
    VERSION 0.1.0
    DESCRIPTION "SDL wrapper thingy"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---- Declare library ----

add_library(
    gfx_gfx
    src/gfx.cpp
    src/renderer.cpp
)
add_library(gfx::gfx ALIAS gfx_gfx)

include(GenerateExportHeader)
generate_export_header(
    gfx_gfx
    BASE_NAME gfx
    EXPORT_FILE_NAME export/gfx/gfx_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(gfx_gfx PUBLIC GFX_STATIC_DEFINE)
endif()

set_target_properties(
    gfx_gfx PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME gfx
    OUTPUT_NAME gfx
)

target_include_directories(
    gfx_gfx ${warning_guard}
    PUBLIC
    "${SDL2_INCLUDE_DIRS}"
    "${SDL2_IMAGE_INCLUDE_DIRS}"
    "${SDL2_TTF_INCLUDE_DIRS}"
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    gfx_gfx SYSTEM
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(gfx_gfx PUBLIC cxx_std_20)

# ---- fmt ----

find_package(fmt REQUIRED)
target_link_libraries(gfx_gfx PRIVATE fmt::fmt)

# ---- SDL2 ----

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

target_link_libraries(gfx_gfx PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT gfx_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of gfx"
  )
endif()

include(cmake/dev-mode.cmake)
