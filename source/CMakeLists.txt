﻿cmake_minimum_required(VERSION 3.16)
project(WeChatMsgDump)
set(CMAKE_SYSTEM_VERSION 10.0.22621.0)
set(CMAKE_CXX_STANDARD 20)

include(cmake/config.cmake)
include(cmake/junuo.cmake)

add_subdirectory(msgmain)
add_subdirectory(wechatmsgdump)
add_subdirectory(junuoui)
add_subdirectory(dbreader)
add_subdirectory(junuobase)
add_subdirectory(msgcore)