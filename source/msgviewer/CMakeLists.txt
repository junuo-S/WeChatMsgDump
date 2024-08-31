set(sources
    entry.h
    entry.cpp
    stdafx.h
    stdafx.cpp

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
)

set(ts_files 
    mui/msgviewer_zh_CN.ts
)

set(target_name msgviewer)

junuo_add_library(${target_name} SHARED ${sources} ${ts_files} ${qrc_files} ${resource_files})
junuo_use_Qt(${target_name} Core Widgets Gui)
junuo_auto_moc(${target_name} ${sources})
junuo_auto_rcc(${target_name} ${qrc_files} ${resource_files})
junuo_link_libraries(${target_name} dbdecryptor)
junuo_link_libraries(${target_name} junuoui)
junuo_link_libraries(${target_name} dbreader)
junuo_include_directories(${target_name} . ../include)
junuo_compile_definitions(${target_name} _MSGVIEWER_MODULE_)
junuo_add_translation(${target_name} ${ts_files})
