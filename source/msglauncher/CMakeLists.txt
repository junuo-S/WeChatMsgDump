set(sources
    startup.cpp
)

set(target_name msglauncher)

junuo_add_executable(${target_name} ${sources})
junuo_link_libraries(${target_name} msgviewer)
junuo_include_directories(${target_name}
    .
    ../include
)

