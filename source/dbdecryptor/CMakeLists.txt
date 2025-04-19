junuo_package(dbdecryptor SHARED)
junuo_include_directories(. ../include)
junuo_compile_definitions(_DB_DECRYPTOR_MODULE_)
junuo_sources(
    junuo_moc
    wxmemoryreader/wxmemoryreader.h
    wxmemoryreader/wxmemoryreader.cpp
    wxdbdecryptor/wxdbdecryptor.h
    wxdbdecryptor/wxdbdecryptor.cpp
    wxdbcombiner/wxdbcombiner.h
    wxdbcombiner/wxdbcombiner.cpp

    configs/versionconfig.json

    python/pymemutils.py
)
junuo_link_packages(junuobase)
junuo_use_packages(Qt6Core Qt6Sql python3 openssl)

add_custom_command(
    TARGET ${junuo_current_package_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cfgs
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/configs/versionconfig.json ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/cfgs
    COMMENT "Copying config files..."
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/configs/versionconfig.json
)

add_custom_command(
    TARGET ${junuo_current_package_name} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/python/pymemutils.py ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bin
    COMMENT "Copying python files..."
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/python/pymemutils.py
)
