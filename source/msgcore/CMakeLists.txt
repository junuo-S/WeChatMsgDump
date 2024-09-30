set(sources
    src/glue/decryptorwapper.h
    src/glue/decryptorwapper.cpp
    src/databus/databus.h
    src/databus/databus.cpp
    src/interface/interfacedecl.h
)

set(target_name msgcore)

junuo_add_library(${target_name} SHARED ${sources})
junuo_use_Qt(${target_name} Core Gui)
junuo_include_directories(${target_name} . ./src ../include)
junuo_compile_definitions(${target_name} MSGCORE_MODULE)
junuo_link_libraries(${target_name} dbdecryptor dbreader)
junuo_auto_moc(${target_name} ${sources})
