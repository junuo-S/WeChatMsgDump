junuo_package(msgmain SHARED)
junuo_sources(
    junuo_moc

    ../include/global_interface.h
    ../include/global_part_impl.cpp

    main.cpp
    stdafx.h
    stdafx.cpp
    global.h
    msgapplication.h
    msgapplication.cpp

    windows/decryptdlg/decryptdlg.h
    windows/decryptdlg/decryptdlg.cpp
    windows/decryptdlg/widgets/loadingpage.h
    windows/decryptdlg/widgets/loadingpage.cpp
    windows/decryptdlg/widgets/wxprocesslistpage.h
    windows/decryptdlg/widgets/wxprocesslistpage.cpp
    windows/decryptdlg/widgets/decryptingpage.h
    windows/decryptdlg/widgets/decryptingpage.cpp

    # windows/msgdlg/msgdlg.h
    # windows/msgdlg/msgdlg.cpp
    # windows/msgdlg/leftwidgets/verticalnavigationbar.h
    # windows/msgdlg/leftwidgets/verticalnavigationbar.cpp
    # windows/msgdlg/middlewidgets/middlepage.h
    # windows/msgdlg/middlewidgets/middlepage.cpp
    # windows/msgdlg/middlewidgets/sessionoverviewcard.h
    # windows/msgdlg/middlewidgets/sessionoverviewcard.cpp
    # windows/msgdlg/rightwidgets/chatpage.h
    # windows/msgdlg/rightwidgets/chatpage.cpp
    # windows/msgdlg/rightwidgets/messagecard.h
    # windows/msgdlg/rightwidgets/messagecard.cpp
    # windows/msgdlg/rightwidgets/messagecarddef.h
    # windows/msgdlg/rightwidgets/messagecarddef.cpp
    # windows/msgdlg/rightwidgets/messagecardfactory.h
    # windows/msgdlg/rightwidgets/messagecardfactory.cpp

    utils/utils.h
    utils/utils.cpp
    dbparser/MSGParser.h
    dbparser/MSGParser.cpp

    msgmanager/msgmanager.h
    msgmanager/msgmanager.cpp

    resources/resource.qrc
    mui/msgmain_zh_CN.ts
    protobuf/MSG.proto
)
junuo_use_packages(Qt6Core Qt6Widgets Qt6Gui Qt6Xml protobuf lz4)
junuo_link_packages(junuoui)
junuo_include_directories(. ../include ${CMAKE_CURRENT_BINARY_DIR})
junuo_compile_definitions(_MSGMAIN_MODULE_)
