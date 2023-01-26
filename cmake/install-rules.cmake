if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/gfx-${PROJECT_VERSION}"
      CACHE PATH ""
  )
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package gfx)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT gfx_Development
)

install(
    TARGETS gfx_gfx
    EXPORT gfxTargets
    RUNTIME #
    COMPONENT gfx_Runtime
    LIBRARY #
    COMPONENT gfx_Runtime
    NAMELINK_COMPONENT gfx_Development
    ARCHIVE #
    COMPONENT gfx_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    gfx_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE PATH "CMake package config location relative to the install prefix"
)
mark_as_advanced(gfx_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${gfx_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT gfx_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${gfx_INSTALL_CMAKEDIR}"
    COMPONENT gfx_Development
)

install(
    EXPORT gfxTargets
    NAMESPACE gfx::
    DESTINATION "${gfx_INSTALL_CMAKEDIR}"
    COMPONENT gfx_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
