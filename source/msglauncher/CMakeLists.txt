set(sources
    startup.cpp
)

set(target_name msglauncher)

include_directories(
    .
)

junuo_add_executable(${target_name} ${sources})
target_link_libraries(${target_name} PRIVATE dbdecryptor)
