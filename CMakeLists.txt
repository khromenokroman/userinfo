cmake_minimum_required(VERSION 3.20...4.2)
project(userinfo VERSION 1.0.0 LANGUAGES CXX)

# Определяем, является ли проект корневым или подключён через add_subdirectory/FetchContent
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(USERINFO_TOP_LEVEL ON)
else()
    set(USERINFO_TOP_LEVEL OFF)
endif()

# Пример собирается только когда проект является корневым
option(USERINFO_BUILD_APP "Собрать пример приложения" ${USERINFO_TOP_LEVEL})

add_subdirectory(src/lib)

if(USERINFO_BUILD_APP)
    add_subdirectory(src/app)
endif()

# Установка и конфигурационные файлы для find_package — только для корневого проекта
if(USERINFO_TOP_LEVEL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    configure_package_config_file(
        cmake/userinfoConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/userinfoConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/userinfo
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/userinfoConfigVersion.cmake
        VERSION     ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/userinfoConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/userinfoConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/userinfo
    )

    install(EXPORT userinfo-targets
        FILE      userinfoTargets.cmake
        NAMESPACE userinfo::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/userinfo
    )
endif()
