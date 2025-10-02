add_executable(vulnscan_gui WIN32
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

target_link_libraries(vulnscan_gui
    PRIVATE
        vulnscan_core
        Qt6::Widgets
)
