set(target_name dbreader)

set(sources
    dbthreadpool/databaseworker.h
    dbthreadpool/databaseworker.cpp
    dbthreadpool/dbthreadpool.h
    dbthreadpool/dbthreadpool.cpp
    wechatdbreader.h
    wechatdbreader.cpp
)

junuo_add_library(${target_name} SHARED ${sources})
junuo_use_Qt(${target_name} Core Sql)
junuo_auto_moc(${target_name} ${sources})
junuo_include_directories(${target_name} . ../include)
junuo_compile_definitions(${target_name} _DBREADER_MODULE_)
