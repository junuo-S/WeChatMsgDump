junuo_package(msgcore SHARED)
junuo_sources(
    junuo_moc
    src/glue/decryptorwapper.h
    src/glue/decryptorwapper.cpp
    src/databus/databus.h
    src/databus/databus.cpp
    src/interface/interfacedecl.h
)
junuo_use_packages(Qt6Core Qt6Gui Qt6Network)
junuo_include_directories(. ./src ../include)
junuo_compile_definitions(MSGCORE_MODULE)
junuo_link_packages(dbdecryptor dbreader)
