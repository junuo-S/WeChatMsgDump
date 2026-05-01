# 项目简介

## 项目目标

`WeChatMsgDump` 是一个 Windows 桌面工具，用于读取当前登录微信进程信息、解密微信数据库并提供消息浏览能力。

## 工程结构

- `source/wechatmsgdump`：程序启动入口（EXE）。
- `source/msgui`：UI 层（解密向导 + 消息浏览界面，DLL）。
- `source/msgcore`：核心业务层（进程读取、解密、合并、消息查询，DLL）。
- `source/junuoui`：通用 UI 组件库。
- `source/junuobase`：基础能力组件（进程/字符串等工具）。
- `source/include`：跨模块公共接口定义。
- `design`：PlantUML 设计草图。

## 运行主链路

1. `wechatmsgdump.exe` 加载 `msgui.dll` 并调用 `MsgMainExec`。
2. `msgui.dll` 再动态加载 `msgcore.dll`，获取 `IJCoreEntry`。
3. UI 先走解密流程，成功后切到消息浏览流程。

## 技术栈

- C++20
- Qt6（Core / Gui / Widgets / Network / Sql / LinguistTools）
- OpenSSL
- SQLite（通过 QtSql 驱动访问）
- WRL `ComPtr` + 自定义 COM 风格接口
