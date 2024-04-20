set(sources
    wxmemoryreader/wxmemoryreader.h
    wxmemoryreader/wxmemoryreader.cpp
    winprocessutils/winprocessutils.h
    winprocessutils/winprocessutils.cpp

    configs/versionconfig.json
)

set(target_name dbdecryptor)

junuo_add_library(${target_name} SHARED ${sources})
junuo_include_directories(${target_name} .)
junuo_compile_definitions(${target_name} _DB_DECRYPTOR_MODULE_)
junuo_use_Qt(${target_name} Core)

add_custom_command(
    TARGET ${target_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cfgs
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/configs/versionconfig.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cfgs
    COMMENT "Copying config files..."
)
