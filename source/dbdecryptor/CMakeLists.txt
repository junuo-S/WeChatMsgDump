set(sources
    wxmemoryreader/wxmemoryreader.h
    wxmemoryreader/wxmemoryreader.cpp
    wxdbdecryptor/wxdbdecryptor.h
    wxdbdecryptor/wxdbdecryptor.cpp
    wxdbcombiner/wxdbcombiner.h
    wxdbcombiner/wxdbcombiner.cpp
    utils/winprocessutils.h
    utils/winprocessutils.cpp
    threads/wxmemorythread.h
    threads/wxmemorythread.cpp
    threads/wxdbdecryptthread.h
    threads/wxdbdecryptthread.cpp
    defines.h

    configs/versionconfig.json

    python/pymemutils.py
)

set(target_name dbdecryptor)

junuo_add_library(${target_name} SHARED ${sources})
junuo_include_directories(${target_name} .)
junuo_compile_definitions(${target_name} _DB_DECRYPTOR_MODULE_)
junuo_use_Qt(${target_name} Core Sql)
junuo_auto_moc(${target_name} ${sources})
junuo_use_Python3(${target_name})
junuo_use_OpenSSL(${target_name})

add_custom_command(
    TARGET ${target_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cfgs
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/configs/versionconfig.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cfgs
    COMMENT "Copying config files..."
)

add_custom_command(
    TARGET ${target_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/python/pymemutils.py ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bin
    COMMENT "Copying python files..."
)
