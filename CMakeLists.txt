cmake_minimum_required(VERSION 3.16)

project(i2c_stmp_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appi2c_stmp_demo
    main.cpp
)

qt_add_qml_module(appi2c_stmp_demo
    URI i2c_stmp_demo
    QML_FILES
        Main.qml
        RESOURCES assets/BrushedMetal.jpg assets/dial_no_background.png assets/dial_original.jpg assets/dial_svg.svg go_fonts/Go-Bold-Italic.ttf go_fonts/Go-Bold.ttf go_fonts/Go-Italic.ttf go_fonts/Go-Medium-Italic.ttf go_fonts/Go-Medium.ttf go_fonts/Go-Regular.ttf go_fonts/README
)
target_link_libraries(appi2c_stmp_demo
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appi2c_stmp_demo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
