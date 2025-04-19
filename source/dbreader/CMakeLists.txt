junuo_package(dbreader SHARED)
junuo_sources(
    junuo_moc
    dbthreadpool/databaseworker.h
    dbthreadpool/databaseworker.cpp
    dbthreadpool/dbthreadpool.h
    dbthreadpool/dbthreadpool.cpp
    wechatdbreader.h
    wechatdbreader.cpp
)
junuo_use_packages(Qt6Core Qt6Sql)
junuo_include_directories(. ../include)
junuo_compile_definitions(_DBREADER_MODULE_)
