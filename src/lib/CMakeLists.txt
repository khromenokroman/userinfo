add_library(userinfo STATIC userinfo.cpp)
add_library(userinfo::userinfo ALIAS userinfo)

target_compile_features(userinfo PUBLIC cxx_std_20)
set_target_properties(userinfo PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(userinfo
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)

install(TARGETS userinfo
    EXPORT  userinfo-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
