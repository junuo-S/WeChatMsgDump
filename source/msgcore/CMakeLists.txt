junuo_package(msgcore SHARED)
junuo_sources(
    ../include/global_interface.h
    ../include/global_part_impl.cpp
    main.cpp
    msgcore.def
    resource/resource.qrc
    src/dbdecrypt/jwechatdbdecryptmanager.h
    src/dbdecrypt/jwechatdbdecryptmanager.cpp
    src/dbdecrypt/processreader/jabstractwechatprocessreader.h
    src/dbdecrypt/processreader/jwechatprocessreaderfactory.h
    src/dbdecrypt/processreader/jwechatprocessreaderfactory.cpp
    src/dbdecrypt/processreader/jwechatprocessreaderv3.h
    src/dbdecrypt/processreader/jwechatprocessreaderv3.cpp
    src/dbdecrypt/decryptor/jabstractwechatdbdecryptor.h
    src/dbdecrypt/decryptor/jabstractwechatdbdecryptor.cpp
    src/dbdecrypt/decryptor/jwechatdbdecryptorv3.h
    src/dbdecrypt/decryptor/jwechatdbdecryptorv3.cpp
    src/dbdecrypt/decryptor/jwechatdbdecryptorfactory.h
    src/dbdecrypt/decryptor/jwechatdbdecryptorfactory.cpp
    src/dbdecrypt/combiner/jwechatdbcombiner.h
    src/dbdecrypt/combiner/jwechatdbcombiner.cpp
)
junuo_use_packages(Qt6Core Qt6Gui Qt6Network Qt6Sql openssl)
junuo_include_directories(. ./src ../include)
junuo_link_packages(junuobase)
