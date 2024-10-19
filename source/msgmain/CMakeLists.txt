set(sources
    entry.h
    entry.cpp
    stdafx.h
    stdafx.cpp
    global.h

    windows/decryptdlg/decryptdlg.h
    windows/decryptdlg/decryptdlg.cpp
    windows/decryptdlg/widgets/loadingpage.h
    windows/decryptdlg/widgets/loadingpage.cpp
    windows/decryptdlg/widgets/wxprocesslistpage.h
    windows/decryptdlg/widgets/wxprocesslistpage.cpp
    windows/decryptdlg/widgets/decryptingpage.h
    windows/decryptdlg/widgets/decryptingpage.cpp

    windows/msgdlg/msgdlg.h
    windows/msgdlg/msgdlg.cpp
    windows/msgdlg/leftwidgets/verticalnavigationbar.h
    windows/msgdlg/leftwidgets/verticalnavigationbar.cpp
    windows/msgdlg/middlewidgets/middlepage.h
    windows/msgdlg/middlewidgets/middlepage.cpp
    windows/msgdlg/middlewidgets/sessionoverviewcard.h
    windows/msgdlg/middlewidgets/sessionoverviewcard.cpp

    utils/utils.h
    utils/utils.cpp
    dbparser/MSGParser.h
    dbparser/MSGParser.cpp

    msgmanager/msgmanager.h
    msgmanager/msgmanager.cpp
)

set(qrc_files
    resources/resource.qrc
)

set(resource_files
    resources/icon_svg/loading-light.svg
    resources/icon_svg/refresh.svg
    resources/stylesheet/decryptdlg.qss
    resources/icon_svg/wxchat.svg
    resources/icon_svg/wechat-friend-gray.svg
    resources/icon_svg/wechat-friend-green.svg
    resources/icon_svg/wechat-msg-gray.svg
    resources/icon_svg/wechat-msg-green.svg
    resources/stylesheet/wechatmsgdialog.qss
    resources/icon_svg/head-image-none.svg
)

set(ts_files 
    mui/msgmain_zh_CN.ts
)

set(proto_files protobuf/MSG.proto)

set(target_name msgmain)

junuo_add_library(${target_name} SHARED ${sources} ${ts_files} ${qrc_files} ${resource_files} ${proto_files})
junuo_use_Qt(${target_name} Core Widgets Gui Xml)
junuo_use_lz4(${target_name})
junuo_use_protobuf(${target_name})
junuo_auto_moc(${target_name} ${sources})
junuo_auto_rcc(${target_name} ${qrc_files} ${resource_files})
junuo_link_libraries(${target_name} msgcore junuoui)
junuo_include_directories(${target_name} . ../include ${CMAKE_CURRENT_BINARY_DIR})
junuo_compile_definitions(${target_name} _MSGMAIN_MODULE_)
junuo_add_translation(${target_name} ${ts_files})
junuo_generate_protobuf_files(${target_name} ${proto_files})
