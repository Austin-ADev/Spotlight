cmake_minimum_required(VERSION 3.19)
project(Spotlight LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(Spotlight
    WIN32 MACOSX_BUNDLE
    main.cpp
    spotlightwindow.cpp
    spotlightwindow.h
    searchengine.h searchengine.cpp
    settingsdialog.h settingsdialog.cpp
    Spotlight.md
    fileindexer.h fileindexer.cpp
    styles.h
)

target_link_libraries(Spotlight
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt6::Network
)

install(TARGETS Spotlight
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Spotlight
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
