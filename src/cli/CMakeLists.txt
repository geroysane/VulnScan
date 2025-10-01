add_executable(vulnscan_cli
    main.cpp
    CommandLineParser.cpp
    CommandLineParser.h
)

target_link_libraries(vulnscan_cli
    PRIVATE
        vulnscan_core
        Qt6::Core
)
