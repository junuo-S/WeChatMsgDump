cmake_minimum_required(VERSION 3.16)

include(cmake/config.cmake)
include(cmake/junuo.cmake)

project(WeChatMsgDump)

add_subdirectory(dbdecryptor)
add_subdirectory(msgviewer)
add_subdirectory(msglauncher)
add_subdirectory(junuoui)
