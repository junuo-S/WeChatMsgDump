set(sources
    startup.cpp
)

set(target_name wechatmsgdump)

junuo_add_executable(${target_name} WIN32 ${sources})
junuo_link_libraries(${target_name} msgmain)
junuo_include_directories(${target_name}
    .
    ../include
)

