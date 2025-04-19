junuo_package(wechatmsgdump EXECUTABLE)
junuo_sources(startup.cpp)
junuo_link_packages(msgmain)
junuo_include_directories(. ../include)
