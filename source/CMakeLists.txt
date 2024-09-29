cmake_minimum_required(VERSION 3.16)

include(cmake/config.cmake)
include(cmake/junuo.cmake)

project(WeChatMsgDump)

add_subdirectory(dbdecryptor)
add_subdirectory(msgmain)
add_subdirectory(wechatmsgdump)
add_subdirectory(junuoui)
add_subdirectory(dbreader)
add_subdirectory(junuobase)
add_subdirectory(msgcore)